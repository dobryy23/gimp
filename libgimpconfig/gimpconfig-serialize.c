/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Object propoerties serialization routines
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
#include <unistd.h>

#include <glib-object.h>

#include "gimpconfig.h"
#include "gimpconfig-serialize.h"
#include "gimpconfig-types.h"


static gboolean gimpconfig_serialize_value (const GValue *value,
                                            GString      *str);

static void     serialize_unknown_token    (const gchar  *key,
                                            const gchar  *value,
                                            gpointer      data);


void
gimp_config_serialize_properties (GObject *object,
                                  gint     fd)
{
  GObjectClass  *klass;
  GParamSpec   **property_specs;
  guint          n_property_specs;
  guint          i;
  GString       *str;

  g_return_if_fail (G_IS_OBJECT (object));
  
  klass = G_OBJECT_GET_CLASS (object);

  property_specs = g_object_class_list_properties (klass, &n_property_specs);

  if (!property_specs)
    return;

  str = g_string_new (NULL);

  for (i = 0; i < n_property_specs; i++)
    {
      GParamSpec  *prop_spec;
      GValue       value = { 0, };

      prop_spec = property_specs[i];

      if (! (prop_spec->flags & G_PARAM_READWRITE))
        continue;

      g_value_init (&value, prop_spec->value_type);
      g_object_get_property (object, prop_spec->name, &value);

      g_string_assign (str, "(");
      g_string_append (str, prop_spec->name);
      
      if (gimpconfig_serialize_value (&value, str))
        {
          g_string_append (str, ")\n");
          write (fd, str->str, str->len);
        }
      else if (prop_spec->value_type != G_TYPE_STRING)
        {
          g_warning ("couldn't serialize property %s::%s of type %s",
                     g_type_name (G_TYPE_FROM_INSTANCE (object)),
                     prop_spec->name, 
                     g_type_name (prop_spec->value_type));
        }

      g_value_unset (&value);
    }

  g_free (property_specs);
  g_string_free (str, TRUE);
}

static gboolean
gimpconfig_serialize_value (const GValue *value,
                            GString      *str)
{
  if (G_VALUE_HOLDS_BOOLEAN (value))
    {
      gboolean bool;
      
      bool = g_value_get_boolean (value);
      g_string_append (str, bool ? " yes" : " no");
      return TRUE;
    }

  if (G_VALUE_HOLDS_ENUM (value))
    {
      GEnumClass *enum_class;
      GEnumValue *enum_value;
      
      enum_class = g_type_class_peek (G_VALUE_TYPE (value));
      enum_value = g_enum_get_value (G_ENUM_CLASS (enum_class),
                                     g_value_get_enum (value));

      if (enum_value && enum_value->value_nick)
        {
          g_string_append_c (str, ' ');
          g_string_append (str, enum_value->value_nick);
          return TRUE;
        }
      else
        {
          g_warning ("Couldn't get nick for enum_value of %s", 
                     G_ENUM_CLASS_TYPE_NAME (enum_class));
          return FALSE;
        }
    }
  
  if (G_VALUE_HOLDS_STRING (value))
    {
      gchar       *escaped;
      const gchar *cstr = g_value_get_string (value);

      if (!cstr)
        return FALSE;

      escaped = g_strescape (cstr, NULL);
      g_string_append_printf (str, " \"%s\"", escaped);
      g_free (escaped);
      return TRUE;
    }

  if (g_value_type_transformable (G_VALUE_TYPE (value), G_TYPE_STRING))
    {
      GValue  tmp_value = { 0, };
      
      g_value_init (&tmp_value, G_TYPE_STRING);
      g_value_transform (value, &tmp_value);

      g_string_append_c (str, ' ');
      g_string_append (str, g_value_get_string (&tmp_value));

      g_value_unset (&tmp_value);
      return TRUE;
    }
  
  return FALSE;
}


void
gimp_config_serialize_unknown_tokens (GObject *object,
                                      gint     fd)
{
  GString *str;

  g_return_if_fail (G_IS_OBJECT (object));

  str = g_string_new (NULL);

  gimp_config_foreach_unknown_token (object, serialize_unknown_token, str);

  write (fd, str->str, str->len);
}

static void
serialize_unknown_token (const gchar *key,
                         const gchar *value,
                         gpointer     data)
{
  gchar *escaped = g_strescape (value, NULL);

  g_string_append_printf ((GString *) data, "(%s \"%s\")\n", key, escaped);

  g_free (escaped);
}
