/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcontext.h
 * Copyright (C) 1999-2001 Michael Natterer
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

#ifndef __GIMP_CONTEXT_H__
#define __GIMP_CONTEXT_H__


#include "gimpobject.h"


#define GIMP_TYPE_CONTEXT            (gimp_context_get_type ())
#define GIMP_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_CONTEXT, GimpContext))
#define GIMP_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST (klass, GIMP_TYPE_CONTEXT, GimpContextClass))
#define GIMP_IS_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_CONTEXT))
#define GIMP_IS_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_CONTEXT))
#define GIMP_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS (obj, GIMP_TYPE_CONTEXT, GimpContextClass))


typedef struct _GimpContextClass GimpContextClass;

struct _GimpContext
{
  GimpObject	        parent_instance;

  Gimp                 *gimp;

  GimpContext	       *parent;

  guint32               defined_props;

  GimpImage	       *image;
  gpointer              display;

  GimpToolInfo         *tool_info;
  gchar                *tool_name;

  GimpRGB               foreground;
  GimpRGB               background;

  gdouble	        opacity;
  GimpLayerModeEffects  paint_mode;

  GimpBrush            *brush;
  gchar                *brush_name;

  GimpPattern          *pattern;
  gchar                *pattern_name;

  GimpGradient         *gradient;
  gchar                *gradient_name;

  GimpPalette          *palette;
  gchar                *palette_name;

  GimpFont             *font;
  gchar                *font_name;

  GimpBuffer           *buffer;
  GimpImagefile        *imagefile;
  GimpTemplate         *template;
};

struct _GimpContextClass
{
  GimpObjectClass  parent_class;

  void (* image_changed)      (GimpContext          *context,
			       GimpImage            *image);
  void (* display_changed)    (GimpContext          *context,
			       gpointer              display);

  void (* tool_changed)       (GimpContext          *context,
			       GimpToolInfo         *tool_info);

  void (* foreground_changed) (GimpContext          *context,
			       GimpRGB              *color);
  void (* background_changed) (GimpContext          *context,
			       GimpRGB              *color);
  void (* opacity_changed)    (GimpContext          *context,
			       gdouble               opacity);
  void (* paint_mode_changed) (GimpContext          *context,
			       GimpLayerModeEffects  paint_mode);
  void (* brush_changed)      (GimpContext          *context,
			       GimpBrush            *brush);
  void (* pattern_changed)    (GimpContext          *context,
			       GimpPattern          *pattern);
  void (* gradient_changed)   (GimpContext          *context,
			       GimpGradient         *gradient);
  void (* palette_changed)    (GimpContext          *context,
			       GimpPalette          *palette);
  void (* font_changed)       (GimpContext          *context,
			       GimpFont             *font);
  void (* buffer_changed)     (GimpContext          *context,
			       GimpBuffer           *buffer);
  void (* imagefile_changed)  (GimpContext          *context,
			       GimpImagefile        *imagefile);
  void (* template_changed)   (GimpContext          *context,
			       GimpTemplate         *template);
};


GType         gimp_context_get_type          (void) G_GNUC_CONST;

GimpContext * gimp_context_new               (Gimp              *gimp,
					      const gchar       *name,
					      GimpContext       *template);

const gchar * gimp_context_get_name          (const GimpContext *context);
void          gimp_context_set_name          (GimpContext       *context,
					      const gchar       *name);

GimpContext * gimp_context_get_parent        (const GimpContext *context);
void          gimp_context_set_parent        (GimpContext       *context,
					      GimpContext       *parent);

/*  define / undefinine context properties
 *
 *  the value of an undefined property will be taken from the parent context.
 */
void          gimp_context_define_property   (GimpContext         *context,
					      GimpContextPropType  prop,
					      gboolean             defined);

gboolean      gimp_context_property_defined  (GimpContext         *context,
					      GimpContextPropType  prop);

void          gimp_context_define_properties (GimpContext         *context,
					      GimpContextPropMask  props_mask,
					      gboolean             defined);

/*  copying context properties
 */
void          gimp_context_copy_property     (GimpContext         *src,
					      GimpContext         *dest,
					      GimpContextPropType  prop);

void          gimp_context_copy_properties   (GimpContext         *src,
					      GimpContext         *dest,
					      GimpContextPropMask  props_mask);


/*  manipulate by GType  */
GimpContextPropType   gimp_context_type_to_property    (GType     type);
const gchar         * gimp_context_type_to_prop_name   (GType     type);
const gchar         * gimp_context_type_to_signal_name (GType     type);

GimpObject    * gimp_context_get_by_type        (GimpContext     *context,
                                                 GType            type);
void            gimp_context_set_by_type        (GimpContext     *context,
                                                 GType            type,
                                                 GimpObject      *object);
void            gimp_context_changed_by_type    (GimpContext     *context,
                                                 GType            type);


/*  image  */
GimpImage     * gimp_context_get_image          (GimpContext     *context);
void            gimp_context_set_image          (GimpContext     *context,
                                                 GimpImage       *image);
void            gimp_context_image_changed      (GimpContext     *context);


/*  display  */
gpointer        gimp_context_get_display        (GimpContext     *context);
void            gimp_context_set_display        (GimpContext     *context,
                                                 gpointer         display);
void            gimp_context_display_changed    (GimpContext     *context);


/*  tool  */
GimpToolInfo  * gimp_context_get_tool           (GimpContext     *context);
void            gimp_context_set_tool           (GimpContext     *context,
                                                 GimpToolInfo    *tool_info);
void            gimp_context_tool_changed       (GimpContext     *context);


/*  foreground color  */
void            gimp_context_get_foreground     (GimpContext     *context,
                                                 GimpRGB         *color);
void            gimp_context_set_foreground     (GimpContext     *context,
                                                 const GimpRGB   *color);
void            gimp_context_foreground_changed (GimpContext     *context);


/*  background color  */
void            gimp_context_get_background     (GimpContext     *context,
                                                 GimpRGB         *color);
void            gimp_context_set_background     (GimpContext     *context,
                                                 const GimpRGB   *color);
void            gimp_context_background_changed (GimpContext     *context);


/*  color utility functions  */
void            gimp_context_set_default_colors (GimpContext     *context);
void            gimp_context_swap_colors        (GimpContext     *context);


/*  opacity  */
gdouble         gimp_context_get_opacity        (GimpContext     *context);
void            gimp_context_set_opacity        (GimpContext     *context,
                                                 gdouble          opacity);
void            gimp_context_opacity_changed    (GimpContext     *context);


/*  paint mode  */
GimpLayerModeEffects
                gimp_context_get_paint_mode     (GimpContext     *context);
void            gimp_context_set_paint_mode     (GimpContext     *context,
                                            GimpLayerModeEffects  paint_mode);
void            gimp_context_paint_mode_changed (GimpContext     *context);


/*  brush  */
GimpBrush     * gimp_context_get_brush          (GimpContext     *context);
void            gimp_context_set_brush          (GimpContext     *context,
                                                 GimpBrush       *brush);
void            gimp_context_brush_changed      (GimpContext     *context);


/*  pattern  */
GimpPattern   * gimp_context_get_pattern        (GimpContext     *context);
void            gimp_context_set_pattern        (GimpContext     *context,
                                                 GimpPattern     *pattern);
void            gimp_context_pattern_changed    (GimpContext     *context);


/*  gradient  */
GimpGradient  * gimp_context_get_gradient       (GimpContext     *context);
void            gimp_context_set_gradient       (GimpContext     *context,
                                                 GimpGradient    *gradient);
void            gimp_context_gradient_changed   (GimpContext     *context);


/*  palette  */
GimpPalette   * gimp_context_get_palette        (GimpContext     *context);
void            gimp_context_set_palette        (GimpContext     *context,
                                                 GimpPalette     *palette);
void            gimp_context_palette_changed    (GimpContext     *context);


/*  font  */
GimpFont      * gimp_context_get_font           (GimpContext     *context);
void            gimp_context_set_font           (GimpContext     *context,
                                                 GimpFont        *font);
void            gimp_context_font_changed       (GimpContext     *context);


/*  buffer  */
GimpBuffer    * gimp_context_get_buffer         (GimpContext     *context);
void            gimp_context_set_buffer         (GimpContext     *context,
                                                 GimpBuffer      *palette);
void            gimp_context_buffer_changed     (GimpContext     *context);


/*  imagefile  */
GimpImagefile * gimp_context_get_imagefile      (GimpContext     *context);
void            gimp_context_set_imagefile      (GimpContext     *context,
                                                 GimpImagefile   *imagefile);
void            gimp_context_imagefile_changed  (GimpContext     *context);


/*  template  */
GimpTemplate  * gimp_context_get_template       (GimpContext     *context);
void            gimp_context_set_template       (GimpContext     *context,
                                                 GimpTemplate    *template);
void            gimp_context_template_changed   (GimpContext     *context);


#endif /* __GIMP_CONTEXT_H__ */
