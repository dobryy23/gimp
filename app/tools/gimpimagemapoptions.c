/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2001 Spencer Kimball, Peter Mattis, and others
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

#include <gtk/gtk.h>

#include "tools-types.h"

#include "config/gimpconfig-params.h"

#include "gimpimagemapoptions.h"


enum
{
  PROP_0,
  PROP_PREVIEW,
  PROP_SETTINGS
};


static void   gimp_image_map_options_class_init   (GimpImageMapOptionsClass *klass);
static void   gimp_image_map_options_finalize     (GObject      *object);

static void   gimp_image_map_options_set_property (GObject      *object,
                                                   guint         property_id,
                                                   const GValue *value,
                                                   GParamSpec   *pspec);
static void   gimp_image_map_options_get_property (GObject      *object,
                                                   guint         property_id,
                                                   GValue       *value,
                                                   GParamSpec   *pspec);


static GimpToolOptionsClass *parent_class = NULL;


GType
gimp_image_map_options_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo info =
      {
        sizeof (GimpImageMapOptionsClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_image_map_options_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpImageMapOptions),
	0,              /* n_preallocs    */
	NULL            /* instance_init  */
      };

      type = g_type_register_static (GIMP_TYPE_TOOL_OPTIONS,
                                     "GimpImageMapOptions",
                                     &info, 0);
    }

  return type;
}

static void
gimp_image_map_options_class_init (GimpImageMapOptionsClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize     = gimp_image_map_options_finalize;
  object_class->set_property = gimp_image_map_options_set_property;
  object_class->get_property = gimp_image_map_options_get_property;

  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_PREVIEW,
                                    "preview", NULL,
                                    TRUE,
                                    0);
  g_object_class_install_property (object_class, PROP_SETTINGS,
                                   g_param_spec_string ("settings",
                                                        NULL, NULL,
                                                        NULL,
                                                        G_PARAM_READWRITE));
}

static void
gimp_image_map_options_finalize (GObject *object)
{
  GimpImageMapOptions *options = GIMP_IMAGE_MAP_OPTIONS (object);

  if (options->settings)
    {
      g_free (options->settings);
      options->settings = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}


static void
gimp_image_map_options_set_property (GObject      *object,
                                     guint         property_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
  GimpImageMapOptions *options = GIMP_IMAGE_MAP_OPTIONS (object);

  switch (property_id)
    {
    case PROP_PREVIEW:
      options->preview = g_value_get_boolean (value);
      break;
    case PROP_SETTINGS:
      g_free (options->settings);
      options->settings = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_image_map_options_get_property (GObject    *object,
                                     guint       property_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
  GimpImageMapOptions *options = GIMP_IMAGE_MAP_OPTIONS (object);

  switch (property_id)
    {
    case PROP_PREVIEW:
      g_value_set_boolean (value, options->preview);
      break;
    case PROP_SETTINGS:
      g_value_set_string (value, options->settings);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
