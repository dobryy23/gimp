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

#ifndef __GDISPLAY_H__
#define __GDISPLAY_H__


/* FIXME: don't include gdisplay.h from core/ */
#include "widgets/widgets-types.h"


typedef enum
{
  SELECTION_OFF,
  SELECTION_LAYER_OFF,
  SELECTION_ON,
  SELECTION_PAUSE,
  SELECTION_RESUME
} SelectionControl;


/*  some useful macros  */

/* unpacking the user scale level (char) */
#define  SCALESRC(g)    (g->scale & 0x00ff)
#define  SCALEDEST(g)   (g->scale >> 8)

/* finding the effective screen resolution (double) */
#define  SCREEN_XRES(g) (g->dot_for_dot? g->gimage->xresolution : gimprc.monitor_xres)
#define  SCREEN_YRES(g) (g->dot_for_dot? g->gimage->yresolution : gimprc.monitor_yres)

/* calculate scale factors (double) */
#define  SCALEFACTOR_X(g)  ((SCALEDEST(g) * SCREEN_XRES(g)) /          \
			    (SCALESRC(g) * g->gimage->xresolution))
#define  SCALEFACTOR_Y(g)  ((SCALEDEST(g) * SCREEN_YRES(g)) /          \
			    (SCALESRC(g) * g->gimage->yresolution))

/* scale values */
#define  SCALEX(g,x)    ((int)(x * SCALEFACTOR_X(g)))
#define  SCALEY(g,y)    ((int)(y * SCALEFACTOR_Y(g)))

/* unscale values */
#define  UNSCALEX(g,x)  ((int)(x / SCALEFACTOR_X(g)))
#define  UNSCALEY(g,y)  ((int)(y / SCALEFACTOR_Y(g)))
/* (and float-returning versions) */
#define  FUNSCALEX(g,x)  ((x / SCALEFACTOR_X(g)))
#define  FUNSCALEY(g,y)  ((y / SCALEFACTOR_Y(g)))




#define LOWPASS(x) ((x>0) ? x : 0)
/* #define HIGHPASS(x,y) ((x>y) ? y : x) */ /* unused - == MIN */


/* maximal width of the string holding the cursor-coordinates for
   the status line */
#define CURSOR_STR_LENGTH 256

/* maximal length of the format string for the cursor-coordinates */
#define CURSOR_FORMAT_LENGTH 32


typedef struct _IdleRenderStruct IdleRenderStruct;

struct _IdleRenderStruct
{
  gint      width;
  gint      height;
  gint      x;
  gint      y;
  gint      basex;
  gint      basey;
  guint     idleid;
  gboolean  active;
  GSList   *update_areas;   /*  flushed update areas */
};


struct _GDisplay
{
  GtkWidget  *shell;              /*  shell widget for this gdisplay          */
  GimpImage  *gimage;	          /*  pointer to the associated gimage struct */
				  /*
  				   *  these need to be first in the sturuct
				   *  because of an ugly hack in
				   *  core/gimpcontext.c
				   */

  gint ID;                        /*  unique identifier for this gdisplay     */

  GtkItemFactory *ifactory;       /*  factory for popup menu                  */

  GtkWidget  *canvas;             /*  canvas widget for this gdisplay         */
  GtkWidget  *hsb;                /*  widgets for scroll bars                 */
  GtkWidget  *vsb;
  GtkWidget  *qmaskoff;           /*  widgets for qmask buttons               */
  GtkWidget  *qmaskon;
  GtkWidget  *hrule;              /*  widgets for rulers                      */
  GtkWidget  *vrule;
  GtkWidget  *origin;             /*  widgets for rulers                      */
  GtkWidget  *statusarea;         /*  hbox holding the statusbar and stuff    */
  GtkWidget  *statusbar;          /*  widget for statusbar                    */
  GtkWidget  *progressbar;        /*  widget for progressbar                  */
  GtkWidget  *cursor_label;       /*  widget for cursor position              */
  gchar       cursor_format_str[CURSOR_FORMAT_LENGTH];
                                  /* we need a variable format string because
				   * different units have different number
				   * of decimals
				   */
  GtkWidget  *cancelbutton;       /*  widget for cancel button                */
  guint       progressid;         /*  id of statusbar message for progress    */

  InfoDialog       *window_info_dialog; /*  dialog box for image information  */
  NavigationDialog *window_nav_dialog;  /*  dialog box for image navigation   */
  NavigationDialog *nav_popup;          /*  the popup navigation window       */
  GtkWidget  *warning_dialog;     /* "Changes were made to %s. Close anyway?" */

  GtkAdjustment *hsbdata;         /*  horizontal data information             */
  GtkAdjustment *vsbdata;         /*  vertical data information               */

  GdkPixmap *icon;		  /*  Pixmap for the icon                     */
  GdkBitmap *iconmask;		  /*  Bitmap for the icon mask                */
  guint      iconsize;            /*  The size of the icon pixmap             */
  gboolean   icon_needs_update;   /*  Do we need to render a new icon?        */
  guint      icon_timeout_id;     /*  The ID of the timeout-function          */
  guint      icon_idle_id;        /*  The ID of the idle-function             */

  gint       instance;            /*  the instance # of this gdisplay as      */
                                  /*  taken from the gimage at creation       */

  gint       disp_width;          /*  width of drawing area in the window     */
  gint       disp_height;         /*  height of drawing area in the window    */
  gint       disp_xoffset;
  gint       disp_yoffset;

  gint       offset_x;            /*  offset of display image into raw image  */
  gint       offset_y;
  gint       scale;               /*  scale factor from original raw image    */
  gboolean   dot_for_dot;         /*  is monitor resolution being ignored?    */
  gboolean   draw_guides;         /*  should the guides be drawn?             */
  gboolean   snap_to_guides;      /*  should the guides be snapped to?        */

  Selection *select;              /*  Selection object                        */

  GdkGC     *scroll_gc;           /*  GC for scrolling                        */

  GSList    *update_areas;        /*  Update areas list                       */
  GSList    *display_areas;       /*  Display areas list                      */

  GdkCursorType       current_cursor;  /*  Currently installed main cursor    */
  GimpToolCursorType  tool_cursor;     /*  Current Tool cursor                */
  GimpCursorModifier  cursor_modifier; /*  Cursor modifier (plus, minus, ...) */

  GdkCursorType override_cursor;  /*  Overriding cursor (ie. hourglass)       */
  gboolean using_override_cursor; /* is the cursor overridden? (ie. hourglass)*/

  gboolean draw_cursor;	          /* should we draw software cursor ?         */
  gint     cursor_x;              /* software cursor X value                  */
  gint     cursor_y;              /* software cursor Y value                  */
  gboolean proximity;             /* is a device in proximity of gdisplay ?   */
  gboolean have_cursor;		  /* is cursor currently drawn ?              */

  IdleRenderStruct idle_render;   /* state of this gdisplay's render thread   */

#ifdef DISPLAY_FILTERS
  GList     *cd_list;             /* color display conversion stuff           */
  GtkWidget *cd_ui;		  /* color display filter dialog              */
#endif /* DISPLAY_FILTERS */
};



/* member function declarations */

GDisplay * gdisplay_new                      (GimpImage          *gimage,
					      guint               scale);
void       gdisplay_reconnect                (GDisplay           *gdisp,
					      GimpImage          *gimage);
void       gdisplay_remove_and_delete        (GDisplay           *gdisp);
gint       gdisplay_mask_value               (GDisplay           *gdisp,
					      gint                x,
					      gint                y);
gint       gdisplay_mask_bounds              (GDisplay           *gdisp,
					      gint               *x1,
					      gint               *y1,
					      gint               *x2,
					      gint               *y2);
void       gdisplay_transform_coords         (GDisplay           *gdisp,
					      gint                x,
					      gint                y,
					      gint               *nx,
					      gint               *ny,
					      gboolean            use_offsets);
void       gdisplay_untransform_coords       (GDisplay           *gdisp,
					      gint                x,
					      gint                y,
					      gint               *nx,
					      gint               *ny,
					      gboolean            round,
					      gboolean            use_offsets);
void       gdisplay_transform_coords_f       (GDisplay           *gdisp,
					      gdouble             x,
					      gdouble             y,
					      gdouble            *nx,
					      gdouble            *ny,
					      gboolean            use_offsets);
void       gdisplay_untransform_coords_f     (GDisplay           *gdisp,
					      gdouble             x,
					      gdouble             y,
					      gdouble            *nx,
					      gdouble            *ny,
					      gboolean            use_offsets);

void       gdisplay_real_install_tool_cursor (GDisplay           *gdisp,
					      GdkCursorType       cursor_type,
					      GimpToolCursorType  tool_cursor,
					      GimpCursorModifier  modifier,
					      gboolean            always_install);
void       gdisplay_install_tool_cursor      (GDisplay           *gdisp,
					      GdkCursorType       cursor_type,
					      GimpToolCursorType  tool_cursor,
					      GimpCursorModifier  modifier);
void       gdisplay_remove_tool_cursor       (GDisplay           *gdisp);
void       gdisplay_install_override_cursor  (GDisplay           *gdisp,
					      GdkCursorType       cursor_type);
void       gdisplay_remove_override_cursor   (GDisplay           *gdisp);

void       gdisplay_set_menu_sensitivity     (GDisplay           *gdisp);
void       gdisplay_expose_area              (GDisplay           *gdisp,
					      gint                x,
					      gint                y,
					      gint                w,
					      gint                h);
void       gdisplay_expose_guide             (GDisplay           *gdisp,
					      GimpGuide          *guide);
void       gdisplay_expose_full              (GDisplay           *gdisp);
void       gdisplay_selection_visibility     (GDisplay           *gdisp,
					      SelectionControl    function);
void       gdisplay_flush                    (GDisplay           *gdisp);
void       gdisplay_flush_now                (GDisplay           *gdisp);
void       gdisplay_update_icon              (GDisplay           *gdisp);
gboolean   gdisplay_update_icon_timer        (gpointer            data);
gboolean   gdisplay_update_icon_invoker      (gpointer            data);
void       gdisplay_update_icon_scheduler    (GimpImage          *gimage,
					      gpointer            data);
void       gdisplay_draw_guides              (GDisplay           *gdisp);
void       gdisplay_draw_guide               (GDisplay           *gdisp,
					      GimpGuide          *guide,
					      gboolean            active);
GimpGuide *gdisplay_find_guide               (GDisplay           *gdisp,
					      gdouble             x,
					      double              y);
gboolean   gdisplay_snap_point               (GDisplay           *gdisp,
					      gdouble             x,
					      gdouble             y,
					      gdouble            *tx,
					      gdouble            *ty);
void       gdisplay_snap_rectangle           (GDisplay           *gdisp,
					      gdouble             x1,
					      gdouble             y1,
					      gdouble             x2,
					      gdouble             y2,
					      gdouble            *tx1,
					      gdouble            *ty1);
void	   gdisplay_update_cursor	     (GDisplay           *gdisp,
					      gint                x,
					      gint                y);
void	   gdisplay_set_dot_for_dot	     (GDisplay           *gdisp,
					      gboolean            dot_for_dot);
void       gdisplay_resize_cursor_label      (GDisplay           *gdisp);
void       gdisplay_update_title             (GDisplay           *gdisp);
void       gdisplay_flush_displays_only      (GDisplay           *gdisp); /* no rerender! */

GDisplay * gdisplay_active                   (void);
GDisplay * gdisplay_get_by_ID                (Gimp               *gimp,
					      gint                ID);


/*  function declarations  */

GDisplay * gdisplays_check_valid             (GDisplay           *gdisp,
					      GimpImage          *gimage);
void       gdisplays_reconnect               (GimpImage          *old,
					      GimpImage          *new);
void       gdisplays_update_title            (GimpImage          *gimage);
void       gdisplays_resize_cursor_label     (GimpImage          *gimage);
void       gdisplays_setup_scale             (GimpImage          *gimage);
void       gdisplays_update_area             (GimpImage          *gimage,
					      gint                x,
					      gint                y,
					      gint                w,
					      gint                h);
void       gdisplays_expose_guides           (GimpImage          *gimage);
void       gdisplays_expose_guide            (GimpImage          *gimage,
					      GimpGuide          *guide);
void       gdisplays_update_full             (GimpImage          *gimage);
void       gdisplays_shrink_wrap             (GimpImage          *gimage);
void       gdisplays_expose_full             (void);
void       gdisplays_selection_visibility    (GimpImage          *gimage,
					      SelectionControl    function);
gboolean   gdisplays_dirty                   (void);
void       gdisplays_delete                  (void);
void       gdisplays_flush                   (void);
void       gdisplays_flush_now               (void);
void       gdisplays_finish_draw             (void);
void       gdisplays_nav_preview_resized     (void);
void       gdisplays_foreach                 (GFunc               func,
					      gpointer            user_data);


#endif /*  __GDISPLAY_H__  */
