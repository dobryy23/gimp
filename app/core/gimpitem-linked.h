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

#ifndef __GIMP_ITEM_LINKED_H__
#define __GIMP_ITEM_LINKED_H__


typedef enum
{
  GIMP_ITEM_LINKED_LAYERS   = 1 << 0,
  GIMP_ITEM_LINKED_CHANNELS = 1 << 1,
  GIMP_ITEM_LINKED_VECTORS  = 1 << 2,

  GIMP_ITEM_LINKED_ALL      = (GIMP_ITEM_LINKED_LAYERS   |
                               GIMP_ITEM_LINKED_CHANNELS |
                               GIMP_ITEM_LINKED_VECTORS)
} GimpItemLinkedMask;


void   gimp_item_linked_translate (GimpItem               *item,
                                   gint                    offset_x,
                                   gint                    offset_y,
                                   gboolean                push_undo);
void   gimp_item_linked_flip      (GimpItem               *item,
                                   GimpOrientationType     flip_type,
                                   gdouble                 axis,
                                   gboolean                clip_result);
void   gimp_item_linked_rotate    (GimpItem               *item,
                                   GimpRotationType        rotate_type,
                                   gdouble                 center_x,
                                   gdouble                 center_y,
                                   gboolean                clip_result);
void   gimp_item_linked_transform (GimpItem               *item,
                                   const GimpMatrix3      *matrix,
                                   GimpTransformDirection  direction,
                                   GimpInterpolationType   interpolation_type,
                                   gboolean                supersample,
                                   gint                    recursion_level,
                                   gboolean                clip_result,
                                   GimpProgressFunc        progress_callback,
                                   gpointer                progress_data);

GList * gimp_item_linked_get_list (GimpImage              *gimage,
                                   GimpItem               *item,
                                   GimpItemLinkedMask      which);


#endif /* __GIMP_ITEM_LINKED_H__ */
