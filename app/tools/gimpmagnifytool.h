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

#ifndef __GIMP_MAGNIFY_TOOL_H__
#define __GIMP_MAGNIFY_TOOL_H__


#include "gimpdrawtool.h"


#define GIMP_TYPE_MAGNIFY_TOOL            (gimp_magnify_tool_get_type ())
#define GIMP_MAGNIFY_TOOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_MAGNIFY_TOOL, GimpMagnifyTool))
#define GIMP_MAGNIFY_TOOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_MAGNIFY_TOOL, GimpMagnifyToolClass))
#define GIMP_IS_MAGNIFY_TOOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_MAGNIFY_TOOL))
#define GIMP_IS_MAGNIFY_TOOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_MAGNIFY_TOOL))
#define GIMP_MAGNIFY_TOOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_MAGNIFY_TOOL, GimpMagnifyToolClass))


typedef struct _GimpMagnifyTool      GimpMagnifyTool;
typedef struct _GimpMagnifyToolClass GimpMagnifyToolClass;

struct _GimpMagnifyTool
{
  GimpDrawTool  parent_instance;

  gint          x, y;       /*  upper left hand coordinate  */
  gint          w, h;       /*  width and height            */

  GimpZoomType  op;         /*  magnify operation           */
};

struct _GimpMagnifyToolClass
{
  GimpDrawToolClass parent_class;
};


void    gimp_magnify_tool_register (GimpToolRegisterCallback  callback,
                                    gpointer                  data);

GType   gimp_magnify_tool_get_type (void) G_GNUC_CONST;


#endif  /*  __GIMP_MAGNIFY_TOOL_H__  */
