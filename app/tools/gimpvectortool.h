/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#ifndef __GIMP_VECTOR_TOOL_H__
#define __GIMP_VECTOR_TOOL_H__


#include "gimpselectiontool.h"


/*  possible vector functions  */
typedef enum 
{
  VECTORS_CREATING,
  VECTORS_ADDING,
  VECTORS_MOVING,
  VECTORS_MOVING_ALL,
  VECTORS_GUIDING,
  VECTORS_FINISHED
} VectorFunction;


#define GIMP_TYPE_VECTOR_TOOL            (gimp_vector_tool_get_type ())
#define GIMP_VECTOR_TOOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_VECTOR_TOOL, GimpVectorTool))
#define GIMP_VECTOR_TOOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_VECTOR_TOOL, GimpVectorToolClass))
#define GIMP_IS_VECTOR_TOOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_VECTOR_TOOL))
#define GIMP_IS_VECTOR_TOOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_VECTOR_TOOL))
#define GIMP_VECTOR_TOOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_VECTOR_TOOL, GimpVectorToolClass))


typedef struct _GimpVectorTool      GimpVectorTool;
typedef struct _GimpVectorToolClass GimpVectorToolClass;

struct _GimpVectorTool
{
  GimpSelectionTool  parent_instance;

  VectorFunction        function;       /*  function we're performing     */
  GimpAnchorFeatureType restriction;    /*  movement restriction          */
  gint                  last_x;         /*  last x coordinate             */
  gint                  last_y;         /*  last y coordinate             */
                                                                   
  GimpAnchor        *cur_anchor;     /*  The current Anchor            */
  GimpStroke        *cur_stroke;     /*  The current Stroke            */
  GimpVectors       *vectors;        /*  The current Vector data       */
  GList             *active_anchors; /*  The currently active anchors  */
};

struct _GimpVectorToolClass
{
  GimpSelectionToolClass  parent_class;
};


void    gimp_vector_tool_register (GimpToolRegisterCallback  callback,
                                   gpointer                  data);

GType   gimp_vector_tool_get_type (void) G_GNUC_CONST;


void    gimp_vector_tool_set_vectors (GimpVectorTool *vector_tool,
                                      GimpVectors    *vectors);

void    gimp_vector_tool_clear_vectors (GimpVectorTool *vector_tool);


#endif  /*  __GIMP_VECTOR_TOOL_H__  */
