/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Object properties deserialization routines
 * Copyright (C) 2001  Sven Neumann <sven@gimp.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <glib-object.h>

#include "gimpconfig.h"
#include "gimpconfig-deserialize.h"
#include "gimpconfig-substitute.h"
#include "gimpconfig-types.h"

#include "libgimp/gimpintl.h"


/*  
 *  All functions return G_TOKEN_RIGHT_PAREN on success,
 *  the GTokenType they would have expected but didn't get
 *  or G_TOKEN_NONE if they got the expected token but
 *  couldn't parse it.
 */
  
static GTokenType  gimp_config_deserialize_unknown     (GObject    *object,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_property    (GObject    *object,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_fundamental (GValue     *value,
                                                        GParamSpec *prop_spec,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_enum        (GValue     *value,
                                                        GParamSpec *prop_spec,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_memsize     (GValue     *value,
                                                        GParamSpec *prop_spec,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_path        (GValue     *value,
                                                        GObject    *object,
                                                        GParamSpec *prop_spec,
                                                        GScanner   *scanner);
static GTokenType  gimp_config_deserialize_any         (GValue     *value,
                                                        GParamSpec *prop_spec,
                                                        GScanner   *scanner);

static inline gboolean  scanner_string_utf8_valid (GScanner    *scanner, 
                                                   const gchar *token_name);


gboolean
gimp_config_deserialize_properties (GObject  *object,
                                    GScanner *scanner,
                                    gboolean  store_unknown_tokens)
{
  GObjectClass  *klass;
  GParamSpec   **property_specs;
  guint          n_property_specs;
  guint          i;
  guint          scope_id;
  guint          old_scope_id;
  GTokenType	 token;
  GTokenType	 next;

  g_return_val_if_fail (G_IS_OBJECT (object), FALSE);

  klass = G_OBJECT_GET_CLASS (object);
  property_specs = g_object_class_list_properties (klass, &n_property_specs);

  if (!property_specs)
    return TRUE;

  scope_id = g_quark_from_static_string ("gimp_config_deserialize_properties");
  old_scope_id = g_scanner_set_scope (scanner, scope_id);

  for (i = 0; i < n_property_specs; i++)
    {
      GParamSpec *prop_spec = property_specs[i];

      if (prop_spec->flags & G_PARAM_READWRITE)
        {
          g_scanner_scope_add_symbol (scanner, scope_id, 
                                      prop_spec->name, prop_spec);
        }
    }

  g_free (property_specs);

  token = G_TOKEN_LEFT_PAREN;
  
  do
    {
      next = g_scanner_peek_next_token (scanner);

      if (next != token &&
         ! (store_unknown_tokens &&
            token == G_TOKEN_SYMBOL && next == G_TOKEN_IDENTIFIER))
        {
          break;
        }

      token = g_scanner_get_next_token (scanner);
      
      switch (token)
        {
        case G_TOKEN_LEFT_PAREN:
          token = G_TOKEN_SYMBOL;
          break;
          
        case G_TOKEN_IDENTIFIER:
          token = gimp_config_deserialize_unknown (object, scanner);
          break;

        case G_TOKEN_SYMBOL:
          token = gimp_config_deserialize_property (object, scanner);
          break;

        case G_TOKEN_RIGHT_PAREN:
          token = G_TOKEN_LEFT_PAREN;
          break;

        default: /* do nothing */
          break;
        }
    }
  while (token != G_TOKEN_EOF);

  if (token != G_TOKEN_LEFT_PAREN && token != G_TOKEN_NONE)
    {
      g_scanner_get_next_token (scanner);
      g_scanner_unexp_token (scanner, token, NULL, NULL, NULL,
                             _("fatal parse error"), TRUE);
    }

  g_scanner_set_scope (scanner, old_scope_id);

  return (token == G_TOKEN_EOF);
}

static GTokenType
gimp_config_deserialize_unknown (GObject  *object,
                                 GScanner *scanner)
{
  gchar *key;

  if (g_scanner_peek_next_token (scanner) != G_TOKEN_STRING)
    return G_TOKEN_STRING;

  key = g_strdup (scanner->value.v_identifier);

  g_scanner_get_next_token (scanner);
  
  if (!scanner_string_utf8_valid (scanner, key))
    {
      g_free (key);
      return G_TOKEN_NONE;
    }

  gimp_config_add_unknown_token (object, key, scanner->value.v_string);
  g_free (key);

  return G_TOKEN_RIGHT_PAREN;
}

static GTokenType
gimp_config_deserialize_property (GObject    *object,
                                  GScanner   *scanner)
{
  GParamSpec *prop_spec;
  GTokenType  token;
  GValue      value = { 0, };

  prop_spec = G_PARAM_SPEC (scanner->value.v_symbol);  

  g_value_init (&value, prop_spec->value_type);

  if (G_TYPE_FUNDAMENTAL (prop_spec->value_type) == G_TYPE_ENUM)
    {
      token = gimp_config_deserialize_enum (&value, prop_spec, scanner);
    }
  else if (G_TYPE_IS_FUNDAMENTAL (prop_spec->value_type))
    {
      token = gimp_config_deserialize_fundamental (&value, prop_spec, scanner);
    }
  else if (prop_spec->value_type == GIMP_TYPE_MEMSIZE)
    {
      token = gimp_config_deserialize_memsize (&value, prop_spec, scanner);
    }
  else if (prop_spec->value_type == GIMP_TYPE_PATH)
    {
      token = gimp_config_deserialize_path (&value,
                                            object, prop_spec, scanner);
    }
  else  /*  This fallback will only work for value_types that  */
    {   /*  can be transformed from a string value.            */
      token = gimp_config_deserialize_any (&value, prop_spec, scanner);
    }

  if (token == G_TOKEN_RIGHT_PAREN &&
      g_scanner_peek_next_token (scanner) == token)
    {
      g_object_set_property (object, prop_spec->name, &value);
    }
  else
    {
      g_warning ("couldn't deserialize property %s::%s of type %s",
                 g_type_name (G_TYPE_FROM_INSTANCE (object)),
                 prop_spec->name, 
                 g_type_name (prop_spec->value_type));
    }

  g_value_unset (&value);
  
  return token;
}

static GTokenType
gimp_config_deserialize_fundamental (GValue     *value,
                                     GParamSpec *prop_spec,
                                     GScanner   *scanner)
{
  GTokenType token;

  switch (G_TYPE_FUNDAMENTAL (prop_spec->value_type))
    {
    case G_TYPE_STRING:
      token = G_TOKEN_STRING;
      break;
      
    case G_TYPE_BOOLEAN:
      token = G_TOKEN_IDENTIFIER;
      break;
      
    case G_TYPE_INT:
    case G_TYPE_UINT:
    case G_TYPE_LONG:
    case G_TYPE_ULONG:
      token = G_TOKEN_INT;
      break;
      
    case G_TYPE_FLOAT:
    case G_TYPE_DOUBLE:
      token = G_TOKEN_FLOAT;
      break;
      
    default:
      token = G_TOKEN_NONE;
      g_assert_not_reached ();
      break;
    }

  if (g_scanner_peek_next_token (scanner) != token)
    return token;

  g_scanner_get_next_token (scanner);

  switch (G_TYPE_FUNDAMENTAL (prop_spec->value_type))
    {
    case G_TYPE_STRING:
      if (scanner_string_utf8_valid (scanner, prop_spec->name))
        g_value_set_static_string (value, scanner->value.v_string);
      else
        return G_TOKEN_NONE;
      break;
      
    case G_TYPE_BOOLEAN:
      if (! g_ascii_strcasecmp (scanner->value.v_identifier, "yes") ||
          ! g_ascii_strcasecmp (scanner->value.v_identifier, "true"))
        g_value_set_boolean (value, TRUE);
      else if (! g_ascii_strcasecmp (scanner->value.v_identifier, "no") ||
               ! g_ascii_strcasecmp (scanner->value.v_identifier, "false"))
        g_value_set_boolean (value, FALSE);
      else
        {
          /* don't translate 'yes' and 'no' */
          g_scanner_warn 
            (scanner, 
             _("expected 'yes' or 'no' for boolean token %s, got '%s'"), 
             prop_spec->name, scanner->value.v_identifier);
          return G_TOKEN_NONE;
        }
      break;

    case G_TYPE_INT:
      g_value_set_int (value, scanner->value.v_int);
      break;
    case G_TYPE_UINT:
      g_value_set_uint (value, scanner->value.v_int);
      break;
    case G_TYPE_LONG:
      g_value_set_int (value, scanner->value.v_int);
      break;
    case G_TYPE_ULONG:
      g_value_set_uint (value, scanner->value.v_int);
      break;      
    case G_TYPE_FLOAT:
      g_value_set_float (value, scanner->value.v_float);
      break;
    case G_TYPE_DOUBLE:
      g_value_set_double (value, scanner->value.v_float);
      break;
      
    default:
      g_assert_not_reached ();
      break;
    }

  return G_TOKEN_RIGHT_PAREN;
}

static GTokenType
gimp_config_deserialize_enum (GValue     *value,
                              GParamSpec *prop_spec,
                              GScanner   *scanner)
{
  GEnumClass *enum_class;
  GEnumValue *enum_value;

  if (g_scanner_peek_next_token (scanner) != G_TOKEN_IDENTIFIER)
    return G_TOKEN_IDENTIFIER;

  g_scanner_get_next_token (scanner);

  enum_class = g_type_class_peek (G_VALUE_TYPE (value));
  enum_value = g_enum_get_value_by_nick (G_ENUM_CLASS (enum_class), 
                                         scanner->value.v_identifier);
  if (!enum_value)
    enum_value = g_enum_get_value_by_name (G_ENUM_CLASS (enum_class), 
                                           scanner->value.v_identifier);
      
  if (!enum_value)
    {
      g_scanner_warn (scanner, 
                      _("invalid value '%s' for token %s"), 
                      scanner->value.v_identifier, prop_spec->name);
      return G_TOKEN_NONE;
    }

  g_value_set_enum (value, enum_value->value);

  return G_TOKEN_RIGHT_PAREN;
}

static GTokenType
gimp_config_deserialize_memsize (GValue     *value,
                                 GParamSpec *prop_spec,
                                 GScanner   *scanner)
{
  GTokenType  token;
  gchar      *orig_cset_first = NULL;
  gchar      *orig_cset_nth   = NULL;

  orig_cset_first = scanner->config->cset_identifier_first;
  orig_cset_nth   = scanner->config->cset_identifier_nth;
  
  scanner->config->cset_identifier_first = G_CSET_DIGITS;
  scanner->config->cset_identifier_nth   = G_CSET_DIGITS "gGmMkKbB";
  
  token = gimp_config_deserialize_any (value, prop_spec, scanner);

  if (orig_cset_first)
    scanner->config->cset_identifier_first = orig_cset_first;
  if (orig_cset_nth)
    scanner->config->cset_identifier_nth = orig_cset_nth;
  
  return token;
}

static GTokenType
gimp_config_deserialize_path (GValue     *value,
                              GObject    *object,
                              GParamSpec *prop_spec,
                              GScanner   *scanner)
{
  gchar *path;

  if (g_scanner_peek_next_token (scanner) != G_TOKEN_STRING)
    return G_TOKEN_STRING;

  g_scanner_get_next_token (scanner);

  if (!scanner_string_utf8_valid (scanner, prop_spec->name))
    return G_TOKEN_NONE;

  path = gimp_config_substitute_path (object, scanner->value.v_string, TRUE);

  if (!path)
    return G_TOKEN_NONE;

  g_value_set_string_take_ownership (value, path);

  return G_TOKEN_RIGHT_PAREN;
}

static GTokenType
gimp_config_deserialize_any (GValue     *value,
                             GParamSpec *prop_spec,
                             GScanner   *scanner)
{
  GValue src = { 0, };

  if (!g_value_type_transformable (G_TYPE_STRING, prop_spec->value_type))
    {
      g_warning ("%s: %s can not be transformed from a string",
                 G_STRLOC, g_type_name (prop_spec->value_type));
      return G_TOKEN_NONE;
    }

  if (g_scanner_peek_next_token (scanner) != G_TOKEN_IDENTIFIER)
    return G_TOKEN_IDENTIFIER;

  g_scanner_get_next_token (scanner);

  g_value_init (&src, G_TYPE_STRING);
  g_value_set_static_string (&src, scanner->value.v_identifier);
  g_value_transform (&src, value);
  g_value_unset (&src);

  return G_TOKEN_RIGHT_PAREN;
}

static inline gboolean
scanner_string_utf8_valid (GScanner    *scanner, 
                           const gchar *token_name)
{
  if (g_utf8_validate (scanner->value.v_string, -1, NULL))
    return TRUE;

  g_scanner_warn (scanner, 
                  _("value for token %s is not a valid UTF-8 string"), 
                  token_name);

  return FALSE;
}
