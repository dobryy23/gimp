/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * GimpText
 * Copyright (C) 2002-2003  Sven Neumann <sven@gimp.org>
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

#ifndef __GIMP_TEXT_LAYER_H__
#define __GIMP_TEXT_LAYER_H__


#include "core/gimplayer.h"


#define GIMP_TYPE_TEXT_LAYER            (gimp_text_layer_get_type ())
#define GIMP_TEXT_LAYER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_TEXT_LAYER, GimpTextLayer))
#define GIMP_TEXT_LAYER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_TEXT_LAYER, GimpTextLayerClass))
#define GIMP_IS_TEXT_LAYER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_TEXT_LAYER))
#define GIMP_IS_TEXT_LAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_TEXT_LAYER))
#define GIMP_TEXT_LAYER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_TEXT_LAYER, GimpTextLayerClass))


typedef struct _GimpTextLayerClass GimpTextLayerClass;

struct _GimpTextLayer
{
  GimpLayer     layer;

  GimpText     *text;
  PangoContext *context;  /* should move to GimpImage ? */
};

struct _GimpTextLayerClass
{
  GimpLayerClass  parent_class;
};


/*  function declarations  */

GType       gimp_text_layer_get_type (void) G_GNUC_CONST;

GimpLayer * gimp_text_layer_new      (GimpImage *gimage,
				      GimpText  *text);


#endif /* __GIMP_TEXT_LAYER_H__ */
