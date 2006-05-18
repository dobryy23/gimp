/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"


#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"
#include "gimppdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimplayer.h"
#include "text/gimptext-compat.h"


static GValueArray *
text_fontname_invoker (GimpProcedure     *procedure,
                       Gimp              *gimp,
                       GimpContext       *context,
                       GimpProgress      *progress,
                       const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  const gchar *text;
  gint32 border;
  gboolean antialias;
  gdouble size;
  gint32 size_type;
  const gchar *fontname;
  GimpLayer *text_layer = NULL;

  image = gimp_value_get_image (&args->values[0], gimp);
  drawable = gimp_value_get_drawable (&args->values[1], gimp);
  x = g_value_get_double (&args->values[2]);
  y = g_value_get_double (&args->values[3]);
  text = g_value_get_string (&args->values[4]);
  border = g_value_get_int (&args->values[5]);
  antialias = g_value_get_boolean (&args->values[6]);
  size = g_value_get_double (&args->values[7]);
  size_type = g_value_get_enum (&args->values[8]);
  fontname = g_value_get_string (&args->values[9]);

  if (success)
    {
      if (drawable && ! gimp_item_is_attached (GIMP_ITEM (drawable)))
        success = FALSE;

      if (success)
        {
          gchar *real_fontname = g_strdup_printf ("%s %d", fontname, (gint) size);

          text_layer = text_render (image, drawable, context,
                                    x, y, real_fontname, text,
                                    border, antialias);

          g_free (real_fontname);
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_layer (&return_vals->values[1], text_layer);

  return return_vals;
}

static GValueArray *
text_get_extents_fontname_invoker (GimpProcedure     *procedure,
                                   Gimp              *gimp,
                                   GimpContext       *context,
                                   GimpProgress      *progress,
                                   const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *text;
  gdouble size;
  gint32 size_type;
  const gchar *fontname;
  gint32 width = 0;
  gint32 height = 0;
  gint32 ascent = 0;
  gint32 descent = 0;

  text = g_value_get_string (&args->values[0]);
  size = g_value_get_double (&args->values[1]);
  size_type = g_value_get_enum (&args->values[2]);
  fontname = g_value_get_string (&args->values[3]);

  if (success)
    {
      gchar *real_fontname = g_strdup_printf ("%s %d", fontname, (gint) size);

      success = text_get_extents (real_fontname, text,
                                  &width, &height,
                                  &ascent, &descent);

      g_free (real_fontname);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], width);
      g_value_set_int (&return_vals->values[2], height);
      g_value_set_int (&return_vals->values[3], ascent);
      g_value_set_int (&return_vals->values[4], descent);
    }

  return return_vals;
}

static GValueArray *
text_invoker (GimpProcedure     *procedure,
              Gimp              *gimp,
              GimpContext       *context,
              GimpProgress      *progress,
              const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  const gchar *text;
  gint32 border;
  gboolean antialias;
  gdouble size;
  gint32 size_type;
  const gchar *foundry;
  const gchar *family;
  const gchar *weight;
  const gchar *slant;
  const gchar *set_width;
  const gchar *spacing;
  const gchar *registry;
  const gchar *encoding;
  GimpLayer *text_layer = NULL;

  image = gimp_value_get_image (&args->values[0], gimp);
  drawable = gimp_value_get_drawable (&args->values[1], gimp);
  x = g_value_get_double (&args->values[2]);
  y = g_value_get_double (&args->values[3]);
  text = g_value_get_string (&args->values[4]);
  border = g_value_get_int (&args->values[5]);
  antialias = g_value_get_boolean (&args->values[6]);
  size = g_value_get_double (&args->values[7]);
  size_type = g_value_get_enum (&args->values[8]);
  foundry = g_value_get_string (&args->values[9]);
  family = g_value_get_string (&args->values[10]);
  weight = g_value_get_string (&args->values[11]);
  slant = g_value_get_string (&args->values[12]);
  set_width = g_value_get_string (&args->values[13]);
  spacing = g_value_get_string (&args->values[14]);
  registry = g_value_get_string (&args->values[15]);
  encoding = g_value_get_string (&args->values[16]);

  if (success)
    {
      if (drawable && ! gimp_item_is_attached (GIMP_ITEM (drawable)))
        success = FALSE;

      if (success)
        {
          gchar *real_fontname = g_strdup_printf ("%s %d", family, (gint) size);

          text_layer = text_render (image, drawable, context,
                                    x, y, real_fontname, text,
                                    border, antialias);

          g_free (real_fontname);
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_layer (&return_vals->values[1], text_layer);

  return return_vals;
}

static GValueArray *
text_get_extents_invoker (GimpProcedure     *procedure,
                          Gimp              *gimp,
                          GimpContext       *context,
                          GimpProgress      *progress,
                          const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *text;
  gdouble size;
  gint32 size_type;
  const gchar *foundry;
  const gchar *family;
  const gchar *weight;
  const gchar *slant;
  const gchar *set_width;
  const gchar *spacing;
  const gchar *registry;
  const gchar *encoding;
  gint32 width = 0;
  gint32 height = 0;
  gint32 ascent = 0;
  gint32 descent = 0;

  text = g_value_get_string (&args->values[0]);
  size = g_value_get_double (&args->values[1]);
  size_type = g_value_get_enum (&args->values[2]);
  foundry = g_value_get_string (&args->values[3]);
  family = g_value_get_string (&args->values[4]);
  weight = g_value_get_string (&args->values[5]);
  slant = g_value_get_string (&args->values[6]);
  set_width = g_value_get_string (&args->values[7]);
  spacing = g_value_get_string (&args->values[8]);
  registry = g_value_get_string (&args->values[9]);
  encoding = g_value_get_string (&args->values[10]);

  if (success)
    {
      gchar *real_fontname = g_strdup_printf ("%s %d", family, (gint) size);

      success = text_get_extents (real_fontname, text,
                                  &width, &height,
                                  &ascent, &descent);

      g_free (real_fontname);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], width);
      g_value_set_int (&return_vals->values[2], height);
      g_value_set_int (&return_vals->values[3], ascent);
      g_value_set_int (&return_vals->values[4], descent);
    }

  return return_vals;
}

void
register_text_tool_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-text-fontname
   */
  procedure = gimp_procedure_new (text_fontname_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-text-fontname");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-text-fontname",
                                     "Add text at the specified location as a floating selection or a new layer.",
                                     "This tool requires a fontname matching an installed PangoFT2 font. You can specify the fontsize in units of pixels or points, and the appropriate metric is specified using the size_type argument. The x and y parameters together control the placement of the new text by specifying the upper left corner of the text bounding box. If the specified drawable parameter is valid, the text will be created as a floating selection attached to the drawable. If the drawable parameter is not valid (-1), the text will appear as a new layer. Finally, a border can be specified around the final rendered text. The border is measured in pixels.",
                                     "Martin Edlman & Sven Neumann",
                                     "Spencer Kimball & Peter Mattis",
                                     "1998- 2001",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_drawable_id ("drawable",
                                                            "drawable",
                                                            "The affected drawable: (-1 for a new text layer)",
                                                            pdb->gimp, TRUE,
                                                            GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("x",
                                                    "x",
                                                    "The x coordinate for the left of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("y",
                                                    "y",
                                                    "The y coordinate for the top of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("border",
                                                      "border",
                                                      "The size of the border",
                                                      -1, G_MAXINT32, -1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_boolean ("antialias",
                                                     "antialias",
                                                     "Antialiasing",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("fontname",
                                                       "fontname",
                                                       "The name of the font",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_layer_id ("text-layer",
                                                             "text layer",
                                                             "The new text layer or -1 if no layer was created.",
                                                             pdb->gimp, FALSE,
                                                             GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-text-get-extents-fontname
   */
  procedure = gimp_procedure_new (text_get_extents_fontname_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-text-get-extents-fontname");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-text-get-extents-fontname",
                                     "Get extents of the bounding box for the specified text.",
                                     "This tool returns the width and height of a bounding box for the specified text string with the specified font information. Ascent and descent for the specified font are returned as well.",
                                     "Martin Edlman & Sven Neumann",
                                     "Spencer Kimball & Peter Mattis",
                                     "1998- 2001",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("fontname",
                                                       "fontname",
                                                       "The name of the font",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The width of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The height of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("ascent",
                                                          "ascent",
                                                          "The ascent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("descent",
                                                          "descent",
                                                          "The descent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-text
   */
  procedure = gimp_procedure_new (text_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-text");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-text",
                                     "This procedure is deprecated! Use 'gimp-text-fontname' instead.",
                                     "This procedure is deprecated! Use 'gimp-text-fontname' instead.",
                                     "",
                                     "",
                                     "",
                                     "gimp-text-fontname");
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_drawable_id ("drawable",
                                                            "drawable",
                                                            "The affected drawable: (-1 for a new text layer)",
                                                            pdb->gimp, TRUE,
                                                            GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("x",
                                                    "x",
                                                    "The x coordinate for the left of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("y",
                                                    "y",
                                                    "The y coordinate for the top of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("border",
                                                      "border",
                                                      "The size of the border",
                                                      -1, G_MAXINT32, -1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_boolean ("antialias",
                                                     "antialias",
                                                     "Antialiasing",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("foundry",
                                                       "foundry",
                                                       "The font foundry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("family",
                                                       "family",
                                                       "The font family",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("weight",
                                                       "weight",
                                                       "The font weight",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("slant",
                                                       "slant",
                                                       "The font slant",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("set-width",
                                                       "set width",
                                                       "The font set-width",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("spacing",
                                                       "spacing",
                                                       "The font spacing",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("registry",
                                                       "registry",
                                                       "The font registry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("encoding",
                                                       "encoding",
                                                       "The font encoding",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_layer_id ("text-layer",
                                                             "text layer",
                                                             "The new text layer or -1 if no layer was created.",
                                                             pdb->gimp, FALSE,
                                                             GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-text-get-extents
   */
  procedure = gimp_procedure_new (text_get_extents_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-text-get-extents");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-text-get-extents",
                                     "This procedure is deprecated! Use 'gimp-text-get-extents-fontname' instead.",
                                     "This procedure is deprecated! Use 'gimp-text-get-extents-fontname' instead.",
                                     "",
                                     "",
                                     "",
                                     "gimp-text-get-extents-fontname");
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("foundry",
                                                       "foundry",
                                                       "The font foundry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("family",
                                                       "family",
                                                       "The font family",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("weight",
                                                       "weight",
                                                       "The font weight",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("slant",
                                                       "slant",
                                                       "The font slant",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("set-width",
                                                       "set width",
                                                       "The font set-width",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("spacing",
                                                       "spacing",
                                                       "The font spacing",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("registry",
                                                       "registry",
                                                       "The font registry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("encoding",
                                                       "encoding",
                                                       "The font encoding",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The width of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The height of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("ascent",
                                                          "ascent",
                                                          "The ascent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("descent",
                                                          "descent",
                                                          "The descent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
