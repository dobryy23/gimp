/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpimagefile.h
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

#ifndef __GIMP_IMAGEFILE_H__
#define __GIMP_IMAGEFILE_H__


#include "gimpviewable.h"


#define GIMP_TYPE_IMAGEFILE            (gimp_imagefile_get_type ())
#define GIMP_IMAGEFILE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_IMAGEFILE, GimpImagefile))
#define GIMP_IMAGEFILE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_IMAGEFILE, GimpImagefileClass))
#define GIMP_IS_IMAGEFILE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_IMAGEFILE))
#define GIMP_IS_IMAGEFILE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_IMAGEFILE))
#define GIMP_IMAGEFILE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_IMAGEFILE, GimpImagefileClass))


typedef struct _GimpImagefileClass GimpImagefileClass;

struct _GimpImagefile
{
  GimpViewable  parent_instance;

  gchar        *filename;

  gint          width;
  gint          height;
  gint          size;
};

struct _GimpImagefileClass
{
  GimpViewableClass  parent_class;
};


GType          gimp_imagefile_get_type (void);
GimpImagefile *gimp_imagefile_new      (const gchar *filename);


#endif /* __GIMP_IMAGEFILE_H__ */
