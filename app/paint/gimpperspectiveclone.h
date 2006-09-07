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

#ifndef __GIMP_PERSPECTIVE_CLONE_H__
#define __GIMP_PERSPECTIVE_CLONE_H__


#include "gimpclone.h"


#define GIMP_TYPE_PERSPECTIVE_CLONE            (gimp_perspective_clone_get_type ())
#define GIMP_PERSPECTIVE_CLONE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_PERSPECTIVE_CLONE, GimpPerspectiveClone))
#define GIMP_PERSPECTIVE_CLONE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PERSPECTIVE_CLONE, GimpPerspectiveCloneClass))
#define GIMP_IS_PERSPECTIVE_CLONE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_PERSPECTIVE_CLONE))
#define GIMP_IS_PERSPECTIVE_CLONE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PERSPECTIVE_CLONE))
#define GIMP_PERSPECTIVE_CLONE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_PERSPECTIVE_CLONE, GimpPerspectiveCloneClass))


typedef struct _GimpPerspectiveClone      GimpPerspectiveClone;
typedef struct _GimpPerspectiveCloneClass GimpPerspectiveCloneClass;

struct _GimpPerspectiveClone
{
  GimpClone     parent_instance;

  gboolean      set_source;

  GimpDrawable *src_drawable;
  gdouble       src_x;          /* coordinates where the cross that indicate where is copying each moment is painted */
  gdouble       src_y;

  gdouble       orig_src_x;
  gdouble       orig_src_y;

  gdouble       dest_x;         /* coords where the stroke starts */
  gdouble       dest_y;

  gdouble       src_x_fv;       /* source coords in front_view perspective */
  gdouble       src_y_fv;

  gdouble       dest_x_fv;      /* destination coords in front_view perspective */
  gdouble       dest_y_fv;

  gdouble       offset_x;
  gdouble       offset_y;
  gboolean      first_stroke;

  GimpMatrix3   transform;
  GimpMatrix3   transform_inv;
};

struct _GimpPerspectiveCloneClass
{
  GimpCloneClass parent_class;
};


void    gimp_perspective_clone_register   (Gimp                      *gimp,
                                           GimpPaintRegisterCallback  callback);

GType   gimp_perspective_clone_get_type   (void) G_GNUC_CONST;

void   gimp_perspective_clone_get_source_point  (GimpPerspectiveClone   *clone,
                                                        gdouble                 x,
                                                        gdouble                 y,
                                                        gdouble                 *newx,
                                                        gdouble                 *newy);

#endif  /*  __GIMP_CLONE_VANISHIGN_H__  */
