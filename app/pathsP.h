/* The GIMP -- an image manipulation program
 * Copyright (C) 1999 Andy Thomas alt@picnic.demon.co.uk
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
#ifndef  __PATHSP_H__
#define  __PATHSP_H__

#include "libgimp/gimpmatrix.h"

#include "gdisplayF.h"    /* GDisplay */
#include "gimpimageF.h"   /* Tattoo   */


typedef struct 
{
  guint32 type;
  gdouble x;
  gdouble y;
} PathPoint;

typedef struct 
{
  GSList    *path_details;
  guint32    pathtype; /* Only beziers to start with */
  gboolean   closed;
  guint32    state;
  guint32    locked;  /* Only bottom bit used */
  Tattoo     tattoo;  /* The tattoo for the path */
  gchar     *name;
} Path;

typedef struct 
{
  GimpImage *gimage;
  GDisplay  *gdisp; /* This is a hack.. Needed so we can get back to 
                     * the same display that these curves were added
		     * too. That way when an entry in the paths dialog
		     * is clicked the bezier tool can be targeted at 
		     * correct display. Note this display could have been
		     * deleted (ie different view), but gdisplays_check_valid()
		     * function will take care of that.. In this case we just
		     * pick a display that the gimage is rendered in.
		     */
  GSList    *bz_paths;  /* list of BZPATHP */
  guint      sig_id;
  gint32     last_selected_row;
} PathList;

typedef enum 
{
  BEZIER = 1
} PathType;

PathPoint *   path_point_new  (gint, gdouble, gdouble);
void          path_point_free (PathPoint *pathpoint);

Path *        path_new        (GimpImage *, PathType, GSList *,
			       gint, gint, gint, gint, gchar *);
Path*         path_copy       (GimpImage *gimage, Path* p);
void          path_free       (Path *path);

Tattoo        path_get_tattoo             (Path*);
Path *        path_get_path_by_tattoo     (GimpImage *, Tattoo);

void          path_stroke     (GimpImage *, PathList *, Path *);
gint          path_distance   (Path *, gdouble, gint *, gint *, gdouble *);

PathList *    path_list_new   (GimpImage *, gint, GSList *);
void          path_list_free  (PathList *);

gboolean      paths_set_path        (GimpImage *, gchar *);
gboolean      paths_set_path_points (GimpImage *, gchar *, gint, gint, gint, gdouble *);

void *        paths_transform_start_undo   (GimpImage *);
void          paths_transform_free_undo    (void *);
void          paths_transform_do_undo      (GimpImage *,void *);
void          paths_transform_current_path (GimpImage *, GimpMatrix3, gboolean);

gboolean      paths_delete_path            (GimpImage *, gchar *);

GSList *      pathpoints_copy (GSList *list);
void          pathpoints_free (GSList *list);

#endif  /*  __PATHSP_H__  */



