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

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "base/pixel-region.h"
#include "base/temp-buf.h"
#include "base/tile.h"
#include "base/tile-manager.h"

#include "paint-funcs/paint-funcs.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimpimage-mask.h"
#include "core/gimptoolinfo.h"

#include "paint/gimppaintoptions.h"

#include "display/gimpdisplay.h"

#include "gimpinktool.h"
#include "gimpinktool-blob.h"
#include "paint_options.h"
#include "tool_manager.h"

#include "undo.h"

#include "libgimp/gimpintl.h"


#define SUBSAMPLE 8

typedef Blob * (* BlobFunc) (gdouble,
			     gdouble,
			     gdouble,
			     gdouble,
			     gdouble,
			     gdouble);


typedef struct _BrushWidget BrushWidget;
typedef struct _InkOptions  InkOptions;

struct _BrushWidget
{
  GtkWidget  *widget;
  gboolean    state;

  /* EEK */
  InkOptions *ink_options;
};

struct _InkOptions
{
  GimpPaintOptions  paint_options;

  gdouble           size;
  gdouble           size_d;
  GtkObject        *size_w;

  gdouble           sensitivity;
  gdouble           sensitivity_d;
  GtkObject        *sensitivity_w;

  gdouble           vel_sensitivity;
  gdouble           vel_sensitivity_d;
  GtkObject        *vel_sensitivity_w;

  gdouble           tilt_sensitivity;
  gdouble           tilt_sensitivity_d;
  GtkObject        *tilt_sensitivity_w;

  gdouble           tilt_angle;
  gdouble           tilt_angle_d;
  GtkObject        *tilt_angle_w;

  BlobFunc          function;
  BlobFunc          function_d;
  GtkWidget        *function_w[3];  /* 3 radio buttons */

  gdouble           aspect;
  gdouble           aspect_d;
  gdouble           angle;
  gdouble           angle_d;
  BrushWidget      *brush_w;
};


/*  local function prototypes  */

static void        gimp_ink_tool_class_init      (GimpInkToolClass *klass);
static void        gimp_ink_tool_init            (GimpInkTool      *tool);

static void        gimp_ink_tool_finalize        (GObject          *object);

static void        gimp_ink_tool_control         (GimpTool         *tool,
                                                  GimpToolAction    action,
                                                  GimpDisplay      *gdisp);
static void        gimp_ink_tool_button_press    (GimpTool         *tool,
                                                  GimpCoords       *coords,
                                                  guint32           time,
                                                  GdkModifierType   state,
                                                  GimpDisplay      *gdisp);
static void        gimp_ink_tool_button_release  (GimpTool         *tool,
                                                  GimpCoords       *coords,
                                                  guint32           time,
                                                  GdkModifierType   state,
                                                  GimpDisplay      *gdisp);
static void        gimp_ink_tool_motion          (GimpTool         *tool,
                                                  GimpCoords       *coords,
                                                  guint32           time,
                                                  GdkModifierType   state,
                                                  GimpDisplay      *gdisp);
static void        gimp_ink_tool_cursor_update   (GimpTool         *tool,
                                                  GimpCoords       *coords,
                                                  GdkModifierType   state,
                                                  GimpDisplay      *gdisp);

static Blob *      ink_pen_ellipse      (InkOptions      *options,
                                         gdouble          x_center,
                                         gdouble          y_center,
                                         gdouble          pressure,
                                         gdouble          xtilt,
                                         gdouble          ytilt,
                                         gdouble          velocity);

static void        time_smoother_add    (GimpInkTool     *ink_tool,
					 guint32          value);
static gdouble     time_smoother_result (GimpInkTool     *ink_tool);
static void        time_smoother_init   (GimpInkTool     *ink_tool,
					 guint32          initval);
static void        dist_smoother_add    (GimpInkTool     *ink_tool,
					 gdouble          value);
static gdouble     dist_smoother_result (GimpInkTool     *ink_tool);
static void        dist_smoother_init   (GimpInkTool     *ink_tool,
					 gdouble          initval);

static void        ink_init             (GimpInkTool     *ink_tool, 
					 GimpDrawable    *drawable, 
					 gdouble          x, 
					 gdouble          y);
static void        ink_finish           (GimpInkTool     *ink_tool, 
					 GimpDrawable    *drawable);
static void        ink_cleanup          (void);

static gboolean    blob_button_expose   (GtkWidget       *widget,
                                         GdkEventExpose  *event,
                                         BlobFunc         function);
static void        paint_blob           (GdkDrawable     *drawable, 
					 GdkGC           *gc,
					 Blob            *blob);

/*  Rendering functions  */
static void        ink_set_paint_area   (GimpInkTool     *ink_tool, 
					 GimpDrawable    *drawable, 
					 Blob            *blob);
static void        ink_paste            (GimpInkTool     *ink_tool, 
					 GimpDrawable    *drawable,
					 Blob            *blob);

static void        ink_to_canvas_tiles  (GimpInkTool     *ink_tool,
					 Blob            *blob,
					 guchar          *color);

static void        ink_set_undo_tiles   (GimpDrawable    *drawable,
					 gint             x, 
					 gint             y,
					 gint             w, 
					 gint             h);
static void        ink_set_canvas_tiles (gint             x, 
					 gint             y,
					 gint             w, 
					 gint             h);

/*  Brush pseudo-widget callbacks  */
static void     brush_widget_active_rect    (BrushWidget    *brush_widget,
                                             GtkWidget      *widget,
                                             GdkRectangle   *rect);
static void     brush_widget_realize        (GtkWidget      *widget);
static gboolean brush_widget_expose         (GtkWidget      *widget,
                                             GdkEventExpose *event,
                                             BrushWidget    *brush_widget);
static gboolean brush_widget_button_press   (GtkWidget      *widget,
                                             GdkEventButton *event,
                                             BrushWidget    *brush_widget);
static gboolean brush_widget_button_release (GtkWidget      *widget,
                                             GdkEventButton *event,
                                             BrushWidget    *brush_widget);
static gboolean brush_widget_motion_notify  (GtkWidget      *widget,
                                             GdkEventMotion *event,
                                             BrushWidget    *brush_widget);

static GimpToolOptions * ink_options_new   (GimpToolInfo    *tool_info);
static void              ink_options_reset (GimpToolOptions *tool_options);
static void              ink_type_update   (GtkWidget       *radio_button,
                                            InkOptions      *options);


/* local variables */

/*  undo blocks variables  */
static TileManager *undo_tiles = NULL;

/* Tiles used to render the stroke at 1 byte/pp */
static TileManager *canvas_tiles = NULL;

/* Flat buffer that is used to used to render the dirty region
 * for composition onto the destination drawable
 */
static TempBuf *canvas_buf = NULL;

static GimpToolClass *parent_class = NULL;


/*  public functions  */

void
gimp_ink_tool_register (GimpToolRegisterCallback  callback,
                        gpointer                  data)
{
  (* callback) (GIMP_TYPE_INK_TOOL,
                ink_options_new,
                TRUE,
                "gimp-ink-tool",
                _("Ink"),
                _("Draw in ink"),
                N_("/Tools/Paint Tools/Ink"), "K",
                NULL, "tools/ink.html",
                GIMP_STOCK_TOOL_INK,
                data);
}

GType
gimp_ink_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpInkToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_ink_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpInkTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_ink_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_TOOL,
					  "GimpInkTool", 
                                          &tool_info, 0);
    }

  return tool_type;
}


/*  private functions  */

static void
gimp_ink_tool_class_init (GimpInkToolClass *klass)
{
  GObjectClass   *object_class;
  GimpToolClass  *tool_class;

  object_class = G_OBJECT_CLASS (klass);
  tool_class   = GIMP_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize     = gimp_ink_tool_finalize;

  tool_class->control        = gimp_ink_tool_control;
  tool_class->button_press   = gimp_ink_tool_button_press;
  tool_class->button_release = gimp_ink_tool_button_release;
  tool_class->motion         = gimp_ink_tool_motion;
  tool_class->cursor_update  = gimp_ink_tool_cursor_update;
}

static void
gimp_ink_tool_init (GimpInkTool *ink_tool)
{
  GimpTool *tool;

  tool = GIMP_TOOL (ink_tool);

  gimp_tool_control_set_motion_mode (tool->control, GIMP_MOTION_MODE_EXACT);
  gimp_tool_control_set_tool_cursor (tool->control, GIMP_INK_TOOL_CURSOR);
}

static void
gimp_ink_tool_finalize (GObject *object)
{
  GimpInkTool *ink_tool;

  ink_tool = GIMP_INK_TOOL (object);

  if (ink_tool->last_blob)
    {
      g_free (ink_tool->last_blob);
      ink_tool->last_blob = NULL;
    }

  ink_cleanup ();

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_ink_tool_control (GimpTool       *tool,
                       GimpToolAction  action,
                       GimpDisplay    *gdisp)
{
  GimpInkTool *ink_tool;

  ink_tool = GIMP_INK_TOOL (tool);

  switch (action)
    {
    case PAUSE:
      break;

    case RESUME:
      break;

    case HALT:
      ink_cleanup ();
      break;

    default:
      break;
    }

  GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static void
gimp_ink_tool_button_press (GimpTool        *tool,
                            GimpCoords      *coords,
                            guint32          time,
                            GdkModifierType  state,
                            GimpDisplay     *gdisp)
{
  GimpInkTool  *ink_tool;
  InkOptions   *options;
  GimpDrawable *drawable;
  Blob         *b;

  ink_tool = GIMP_INK_TOOL (tool);

  options = (InkOptions *) tool->tool_info->tool_options;

  drawable = gimp_image_active_drawable (gdisp->gimage);

  ink_init (ink_tool, drawable, coords->x, coords->y);

  gimp_tool_control_activate (tool->control);
  tool->gdisp = gdisp; 

  /*  pause the current selection  */
  gimp_image_selection_control (gdisp->gimage, GIMP_SELECTION_PAUSE);

  b = ink_pen_ellipse (options,
                       coords->x,
                       coords->y,
		       coords->pressure,
                       coords->xtilt,
                       coords->ytilt,
		       10.0);

  ink_paste (ink_tool, drawable, b);
  ink_tool->last_blob = b;

  time_smoother_init (ink_tool, time);
  ink_tool->last_time = time;
  dist_smoother_init (ink_tool, 0.0);
  ink_tool->init_velocity = TRUE;
  ink_tool->lastx = coords->x;
  ink_tool->lasty = coords->y;

  gimp_display_flush_now (gdisp);
}

static void
gimp_ink_tool_button_release (GimpTool        *tool,
                              GimpCoords      *coords,
                              guint32          time,
                              GdkModifierType  state,
                              GimpDisplay     *gdisp)
{
  GimpInkTool *ink_tool;
  GimpImage   *gimage;

  ink_tool = GIMP_INK_TOOL (tool);

  gimage = gdisp->gimage;

  /*  resume the current selection  */
  gimp_image_selection_control (gdisp->gimage, GIMP_SELECTION_RESUME);

  /*  Set tool state to inactive -- no longer painting */
  gimp_tool_control_halt (tool->control);

  /*  free the last blob  */
  g_free (ink_tool->last_blob);
  ink_tool->last_blob = NULL;

  ink_finish (ink_tool, gimp_image_active_drawable (gdisp->gimage));
  gimp_image_flush (gdisp->gimage);
}

static void
gimp_ink_tool_motion (GimpTool        *tool,
                      GimpCoords      *coords,
                      guint32          time,
                      GdkModifierType  state,
                      GimpDisplay     *gdisp)
{
  GimpInkTool  *ink_tool;
  InkOptions   *options;
  GimpDrawable *drawable;
  Blob         *b, *blob_union;

  gdouble velocity;
  gdouble dist;
  gdouble lasttime, thistime;

  ink_tool = GIMP_INK_TOOL (tool);

  options = (InkOptions *) tool->tool_info->tool_options;

  drawable = gimp_image_active_drawable (gdisp->gimage);

  lasttime = ink_tool->last_time;

  time_smoother_add (ink_tool, time);
  thistime = ink_tool->last_time = time_smoother_result (ink_tool);

  /* The time resolution on X-based GDK motion events is
     bloody awful, hence the use of the smoothing function.
     Sadly this also means that there is always the chance of
     having an indeterminite velocity since this event and
     the previous several may still appear to issue at the same
     instant. -ADM */

  if (thistime == lasttime)
    thistime = lasttime + 1;

  if (ink_tool->init_velocity)
    {
      dist_smoother_init (ink_tool,
			  dist = sqrt ((ink_tool->lastx - coords->x) *
                                       (ink_tool->lastx - coords->x) +
				       (ink_tool->lasty - coords->y) *
                                       (ink_tool->lasty - coords->y)));
      ink_tool->init_velocity = FALSE;
    }
  else
    {
      dist_smoother_add (ink_tool,
			 sqrt ((ink_tool->lastx - coords->x) *
                               (ink_tool->lastx - coords->x) +
			       (ink_tool->lasty - coords->y) *
                               (ink_tool->lasty - coords->y)));

      dist = dist_smoother_result (ink_tool);
    }

  ink_tool->lastx = coords->x;
  ink_tool->lasty = coords->y;

  velocity = 10.0 * sqrt ((dist) / (gdouble) (thistime - lasttime));

  b = ink_pen_ellipse (options,
                       coords->x,
                       coords->y,
                       coords->pressure,
                       coords->xtilt,
		       coords->ytilt,
                       velocity);

  blob_union = blob_convex_union (ink_tool->last_blob, b);
  g_free (ink_tool->last_blob);
  ink_tool->last_blob = b;

  ink_paste (ink_tool, drawable, blob_union);  
  g_free (blob_union);

  gimp_display_flush_now (gdisp);
}

static void
gimp_ink_tool_cursor_update (GimpTool        *tool,
                             GimpCoords      *coords,
                             GdkModifierType  state,
                             GimpDisplay     *gdisp)
{
  GimpLayer     *layer;
  GdkCursorType  ctype = GDK_TOP_LEFT_ARROW;

  if ((layer = gimp_image_get_active_layer (gdisp->gimage))) 
    {
      gint off_x, off_y;

      gimp_drawable_offsets (GIMP_DRAWABLE (layer), &off_x, &off_y);

      if (coords->x >= off_x &&
          coords->y >= off_y &&
	  coords->x < (off_x + gimp_drawable_width (GIMP_DRAWABLE (layer))) &&
	  coords->y < (off_y + gimp_drawable_height (GIMP_DRAWABLE (layer))))
	{
	  /*  One more test--is there a selected region?
	   *  if so, is cursor inside?
	   */
	  if (gimp_image_mask_is_empty (gdisp->gimage))
	    ctype = GIMP_MOUSE_CURSOR;
	  else if (gimp_image_mask_value (gdisp->gimage, coords->x, coords->y))
	    ctype = GIMP_MOUSE_CURSOR;
	}
    }

  gimp_tool_control_set_cursor (tool->control, ctype);

  GIMP_TOOL_CLASS (parent_class)->cursor_update (tool, coords, state, gdisp);
}


/*  the brush widget functions  */

static void
brush_widget_active_rect (BrushWidget  *brush_widget,
			  GtkWidget    *widget,
			  GdkRectangle *rect)
{
  gint x,y;
  gint r;

  r = MIN (widget->allocation.width, widget->allocation.height) / 2;

  x = widget->allocation.width / 2 + 0.85 * r * brush_widget->ink_options->aspect / 10.0 *
    cos (brush_widget->ink_options->angle);
  y = widget->allocation.height / 2 + 0.85 * r * brush_widget->ink_options->aspect / 10.0 *
    sin (brush_widget->ink_options->angle);

  rect->x = x - 5;
  rect->y = y - 5;
  rect->width = 10;
  rect->height = 10;
}

static void
brush_widget_realize (GtkWidget *widget)
{
  gtk_style_set_background (widget->style, widget->window, GTK_STATE_ACTIVE);
}

static void
brush_widget_draw_brush (BrushWidget *brush_widget,
			 GtkWidget   *widget,
			 gdouble      xc,
			 gdouble      yc,
			 gdouble      radius)
{
  Blob *blob;

  blob = brush_widget->ink_options->function (xc, yc,
                                              radius * cos (brush_widget->ink_options->angle),
                                              radius * sin (brush_widget->ink_options->angle),
                                              (- (radius / brush_widget->ink_options->aspect) *
                                               sin (brush_widget->ink_options->angle)),
                                              ((radius / brush_widget->ink_options->aspect) *
                                               cos (brush_widget->ink_options->angle)));

  paint_blob (widget->window, widget->style->fg_gc[widget->state], blob);
  g_free (blob);
}

static gboolean
brush_widget_expose (GtkWidget      *widget,
		     GdkEventExpose *event,
		     BrushWidget    *brush_widget)
{
  GdkRectangle rect;
  gint         r0;

  r0 = MIN (widget->allocation.width, widget->allocation.height) / 2;

  if (r0 < 2)
    return TRUE;

  brush_widget_draw_brush (brush_widget, widget,
			   widget->allocation.width  / 2,
			   widget->allocation.height / 2,
			   0.9 * r0);

  brush_widget_active_rect (brush_widget, widget, &rect);
  gdk_draw_rectangle (widget->window, widget->style->bg_gc[GTK_STATE_NORMAL],
		      TRUE,	/* filled */
		      rect.x, rect.y, 
		      rect.width, rect.height);
  gtk_paint_shadow (widget->style, widget->window, widget->state,
                    GTK_SHADOW_OUT,
                    NULL, widget, NULL,
                    rect.x, rect.y,
                    rect.width, rect.height);

  return TRUE;
}

static gboolean
brush_widget_button_press (GtkWidget      *widget,
			   GdkEventButton *event,
			   BrushWidget    *brush_widget)
{
  GdkRectangle rect;

  brush_widget_active_rect (brush_widget, widget, &rect);

  if ((event->x >= rect.x) && (event->x-rect.x < rect.width) &&
      (event->y >= rect.y) && (event->y-rect.y < rect.height))
    {
      brush_widget->state = TRUE;

      gtk_grab_add (brush_widget->widget);
    }

  return TRUE;
}

static gboolean
brush_widget_button_release (GtkWidget      *widget,
			     GdkEventButton *event,
			     BrushWidget    *brush_widget)
{
  brush_widget->state = FALSE;

  gtk_grab_remove (brush_widget->widget);

  return TRUE;
}

static gboolean
brush_widget_motion_notify (GtkWidget      *widget,
			    GdkEventMotion *event,
			    BrushWidget    *brush_widget)
{
  int x;
  int y;
  int r0;
  int rsquare;

  if (brush_widget->state)
    {
      x = event->x - widget->allocation.width / 2;
      y = event->y - widget->allocation.height / 2;
      rsquare = x*x + y*y;

      if (rsquare != 0)
	{
	  brush_widget->ink_options->angle = atan2 (y, x);

	  r0 = MIN (widget->allocation.width, widget->allocation.height) / 2;
	  brush_widget->ink_options->aspect =
	    10.0 * sqrt ((gdouble) rsquare / (r0 * r0)) / 0.85;

	  if (brush_widget->ink_options->aspect < 1.0)
	    brush_widget->ink_options->aspect = 1.0;
	  if (brush_widget->ink_options->aspect > 10.0)
	    brush_widget->ink_options->aspect = 10.0;

	  gtk_widget_queue_draw (widget);
	}
    }

  return TRUE;
}

static gboolean
blob_button_expose (GtkWidget      *widget,
                    GdkEventExpose *event,
                    BlobFunc        function)
{
  Blob *blob = (*function) (widget->allocation.width  / 2,
                            widget->allocation.height / 2, 8, 0, 0, 8);
  paint_blob (widget->window, widget->style->fg_gc[widget->state], blob);
  g_free (blob);

  return TRUE;
}

/*
 * Draw a blob onto a drawable with the specified graphics context
 */
static void
paint_blob (GdkDrawable *drawable,
	    GdkGC       *gc,
	    Blob        *blob)
{
  gint i;

  for (i = 0; i < blob->height; i++)
    if (blob->data[i].left <= blob->data[i].right)
      gdk_draw_line (drawable, gc,
                     blob->data[i].left,      i + blob->y,
                     blob->data[i].right + 1, i + blob->y);
}


static Blob *
ink_pen_ellipse (InkOptions *options,
                 gdouble     x_center,
		 gdouble     y_center,
		 gdouble     pressure,
		 gdouble     xtilt,
		 gdouble     ytilt,
		 gdouble     velocity)
{
  gdouble size;
  gdouble tsin, tcos;
  gdouble aspect, radmin;
  gdouble x,y;
  gdouble tscale;
  gdouble tscale_c;
  gdouble tscale_s;

  /* Adjust the size depending on pressure. */

  size = options->size * (1.0 + options->sensitivity *
                          (2.0 * pressure - 1.0) );

  /* Adjust the size further depending on pointer velocity
     and velocity-sensitivity.  These 'magic constants' are
     'feels natural' tigert-approved. --ADM */

  if (velocity < 3.0)
    velocity = 3.0;

#ifdef VERBOSE
  g_print ("%f (%f) -> ", (float)size, (float)velocity);
#endif  

  size = options->vel_sensitivity *
    ((4.5 * size) / (1.0 + options->vel_sensitivity * (2.0*(velocity))))
    + (1.0 - options->vel_sensitivity) * size;

#ifdef VERBOSE
  g_print ("%f\n", (float)size);
#endif

  /* Clamp resulting size to sane limits */

  if (size > options->size * (1.0 + options->sensitivity))
    size = options->size * (1.0 + options->sensitivity);

  if (size*SUBSAMPLE < 1.0) size = 1.0/SUBSAMPLE;

  /* Add brush angle/aspect to tilt vectorially */

  /* I'm not happy with the way the brush widget info is combined with
     tilt info from the brush. My personal feeling is that representing
     both as affine transforms would make the most sense. -RLL */

  tscale = options->tilt_sensitivity * 10.0;
  tscale_c = tscale * cos (gimp_deg_to_rad (options->tilt_angle));
  tscale_s = tscale * sin (gimp_deg_to_rad (options->tilt_angle));

  x = (options->aspect * cos (options->angle) +
       xtilt * tscale_c - ytilt * tscale_s);
  y = (options->aspect * sin (options->angle) +
       ytilt * tscale_c + xtilt * tscale_s);

#ifdef VERBOSE
  g_print ("angle %g aspect %g; %g %g; %g %g\n",
	   options->angle, options->aspect, tscale_c, tscale_s, x, y);
#endif
  aspect = sqrt(x*x+y*y);

  if (aspect != 0)
    {
      tcos = x/aspect;
      tsin = y/aspect;
    }
  else
    {
      tsin = sin (options->angle);
      tcos = cos (options->angle);
    }

  if (aspect < 1.0) 
    aspect = 1.0;
  else if (aspect > 10.0) 
    aspect = 10.0;

  radmin = SUBSAMPLE * size/aspect;
  if (radmin < 1.0) radmin = 1.0;
  
  return options->function (x_center * SUBSAMPLE,
                            y_center * SUBSAMPLE,
                            radmin * aspect * tcos,
                            radmin * aspect * tsin,  
                            -radmin * tsin,
                            radmin * tcos);
}

static void
dist_smoother_init (GimpInkTool *ink_tool,
		    gdouble      initval)
{
  gint i;

  ink_tool->dt_index = 0;

  for (i=0; i<DIST_SMOOTHER_BUFFER; i++)
    {
      ink_tool->dt_buffer[i] = initval;
    }
}

static gdouble
dist_smoother_result (GimpInkTool *ink_tool)
{
  gint    i;
  gdouble result = 0.0;

  for (i = 0; i < DIST_SMOOTHER_BUFFER; i++)
    {
      result += ink_tool->dt_buffer[i];
    }

  return (result / (gdouble) DIST_SMOOTHER_BUFFER);
}

static void
dist_smoother_add (GimpInkTool *ink_tool,
		   gdouble      value)
{
  ink_tool->dt_buffer[ink_tool->dt_index] = value;

  if ((++ink_tool->dt_index) == DIST_SMOOTHER_BUFFER)
    ink_tool->dt_index = 0;
}


static void
time_smoother_init (GimpInkTool *ink_tool,
		    guint32      initval)
{
  gint i;

  ink_tool->ts_index = 0;

  for (i = 0; i < TIME_SMOOTHER_BUFFER; i++)
    {
      ink_tool->ts_buffer[i] = initval;
    }
}

static gdouble
time_smoother_result (GimpInkTool *ink_tool)
{
  gint    i;
  guint64 result = 0;

  for (i = 0; i < TIME_SMOOTHER_BUFFER; i++)
    {
      result += ink_tool->ts_buffer[i];
    }

#ifdef _MSC_VER
  return (gdouble) (gint64) (result / TIME_SMOOTHER_BUFFER);
#else
  return (result / TIME_SMOOTHER_BUFFER);
#endif
}

static void
time_smoother_add (GimpInkTool *ink_tool,
		   guint32      value)
{
  ink_tool->ts_buffer[ink_tool->ts_index] = value;

  if ((++ink_tool->ts_index) == TIME_SMOOTHER_BUFFER)
    ink_tool->ts_index = 0;
}

static void
ink_init (GimpInkTool  *ink_tool,
	  GimpDrawable *drawable, 
	  gdouble       x,
	  gdouble       y)
{
  /*  free the block structures  */
  if (undo_tiles)
    tile_manager_destroy (undo_tiles);
  if (canvas_tiles)
    tile_manager_destroy (canvas_tiles);

  /*  Allocate the undo structure  */
  undo_tiles = tile_manager_new (gimp_drawable_width (drawable),
				 gimp_drawable_height (drawable),
				 gimp_drawable_bytes (drawable));

  /*  Allocate the canvas blocks structure  */
  canvas_tiles = tile_manager_new (gimp_drawable_width (drawable),
				   gimp_drawable_height (drawable), 1);

  /*  Get the initial undo extents  */
  ink_tool->x1 = ink_tool->x2 = x;
  ink_tool->y1 = ink_tool->y2 = y;
}

static void
ink_finish (GimpInkTool  *ink_tool,
	    GimpDrawable *drawable)
{
  gimp_drawable_push_undo (drawable,
                           ink_tool->x1, ink_tool->y1,
                           ink_tool->x2, ink_tool->y2,
                           undo_tiles, TRUE);
  undo_tiles = NULL;

  /*  invalidate the drawable--have to do it here, because
   *  it is not done during the actual painting.
   */
  gimp_viewable_invalidate_preview (GIMP_VIEWABLE (drawable));
}

static void
ink_cleanup (void)
{
  /*  If the undo tiles exist, nuke them  */
  if (undo_tiles)
    {
      tile_manager_destroy (undo_tiles);
      undo_tiles = NULL;
    }

  /*  If the canvas blocks exist, nuke them  */
  if (canvas_tiles)
    {
      tile_manager_destroy (canvas_tiles);
      canvas_tiles = NULL;
    }

  /*  Free the temporary buffer if it exist  */
  if (canvas_buf)
    {
      temp_buf_free (canvas_buf);
      canvas_buf = NULL;
    }
}

/*********************************
 *  Rendering functions          *
 *********************************/

/* Some of this stuff should probably be combined with the 
 * code it was copied from in paint_core.c; but I wanted
 * to learn this stuff, so I've kept it simple.
 *
 * The following only supports CONSTANT mode. Incremental
 * would, I think, interact strangely with the way we
 * do things. But it wouldn't be hard to implement at all.
 */

static void
ink_set_paint_area (GimpInkTool  *ink_tool, 
		    GimpDrawable *drawable, 
		    Blob         *blob)
{
  gint x, y, width, height;
  gint x1, y1, x2, y2;
  gint bytes;

  blob_bounds (blob, &x, &y, &width, &height);

  bytes = gimp_drawable_has_alpha (drawable) ?
    gimp_drawable_bytes (drawable) : gimp_drawable_bytes (drawable) + 1;

  x1 = CLAMP (x/SUBSAMPLE - 1,            0, gimp_drawable_width (drawable));
  y1 = CLAMP (y/SUBSAMPLE - 1,            0, gimp_drawable_height (drawable));
  x2 = CLAMP ((x + width)/SUBSAMPLE + 2,  0, gimp_drawable_width (drawable));
  y2 = CLAMP ((y + height)/SUBSAMPLE + 2, 0, gimp_drawable_height (drawable));

  /*  configure the canvas buffer  */
  if ((x2 - x1) && (y2 - y1))
    canvas_buf = temp_buf_resize (canvas_buf, bytes, x1, y1,
				  (x2 - x1), (y2 - y1));
}

enum { ROW_START, ROW_STOP };

/* The insertion sort here, for SUBSAMPLE = 8, tends to beat out
 * qsort() by 4x with CFLAGS=-O2, 2x with CFLAGS=-g
 */
static void
insert_sort (gint *data,
	     gint  n)
{
  gint i, j, k;
  gint tmp1, tmp2;

  for (i=2; i<2*n; i+=2)
    {
      tmp1 = data[i];
      tmp2 = data[i+1];
      j = 0;
      while (data[j] < tmp1)
	j += 2;

      for (k=i; k>j; k-=2)
	{
	  data[k] = data[k-2];
	  data[k+1] = data[k-1];
	}

      data[j] = tmp1;
      data[j+1] = tmp2;
    }
}

static void
fill_run (guchar *dest,
	  guchar  alpha,
	  gint    w)
{
  if (alpha == 255)
    {
      memset (dest, 255, w);
    }
  else
    {
      while (w--)
	{
	  *dest = MAX(*dest, alpha);
	  dest++;
	}
    }
}

static void
render_blob_line (Blob   *blob,
		  guchar *dest,
		  gint    x,
		  gint    y,
		  gint    width)
{
  gint  buf[4 * SUBSAMPLE];
  gint *data    = buf;
  gint  n       = 0;
  gint  i, j;
  gint  current = 0;  /* number of filled rows at this point
		       * in the scan line
		       */
  gint last_x;

  /* Sort start and ends for all lines */
  
  j = y * SUBSAMPLE - blob->y;
  for (i = 0; i < SUBSAMPLE; i++)
    {
      if (j >= blob->height)
	break;

      if ((j > 0) && (blob->data[j].left <= blob->data[j].right))
	{
	  data[2 * n]                     = blob->data[j].left;
	  data[2 * n + 1]                 = ROW_START;
	  data[2 * SUBSAMPLE + 2 * n]     = blob->data[j].right;
	  data[2 * SUBSAMPLE + 2 * n + 1] = ROW_STOP;
	  n++;
	}
      j++;
    }

  /*   If we have less than SUBSAMPLE rows, compress */
  if (n < SUBSAMPLE)
    {
      for (i = 0; i < 2 * n; i++)
	data[2 * n + i] = data[2 * SUBSAMPLE + i];
    }

  /*   Now count start and end separately */
  n *= 2;

  insert_sort (data, n);

  /* Discard portions outside of tile */

  while ((n > 0) && (data[0] < SUBSAMPLE*x))
    {
      if (data[1] == ROW_START)
	current++;
      else
	current--;
      data += 2;
      n--;
    }

  while ((n > 0) && (data[2*(n-1)] >= SUBSAMPLE*(x+width)))
    n--;
  
  /* Render the row */

  last_x = 0;
  for (i = 0; i < n;)
    {
      gint cur_x = data[2 * i] / SUBSAMPLE - x;
      gint pixel;

      /* Fill in portion leading up to this pixel */
      if (current && cur_x != last_x)
	fill_run (dest + last_x, (255 * current) / SUBSAMPLE, cur_x - last_x);

      /* Compute the value for this pixel */
      pixel = current * SUBSAMPLE; 

      while (i<n)
	{
	  gint tmp_x = data[2 * i] / SUBSAMPLE;

	  if (tmp_x - x != cur_x)
	    break;

	  if (data[2 * i + 1] == ROW_START)
	    {
	      current++;
	      pixel += ((tmp_x + 1) * SUBSAMPLE) - data[2 * i];
	    }
	  else
	    {
	      current--;
	      pixel -= ((tmp_x + 1) * SUBSAMPLE) - data[2 * i];
	    }
	  
	  i++;
	}

      dest[cur_x] = MAX (dest[cur_x], (pixel * 255) / (SUBSAMPLE * SUBSAMPLE));

      last_x = cur_x + 1;
    }

  if (current != 0)
    fill_run (dest + last_x, (255 * current)/ SUBSAMPLE, width - last_x);
}

static void
render_blob (PixelRegion *dest,
	     Blob        *blob)
{
  gint      i;
  gint      h;
  guchar   *s;
  gpointer  pr;

  for (pr = pixel_regions_register (1, dest); 
       pr != NULL; 
       pr = pixel_regions_process (pr))
    {
      h = dest->h;
      s = dest->data;

      for (i=0; i<h; i++)
	{
	  render_blob_line (blob, s,
			    dest->x, dest->y + i, dest->w);
	  s += dest->rowstride;
	}
    }
}

static void
ink_paste (GimpInkTool  *ink_tool, 
	   GimpDrawable *drawable,
	   Blob         *blob)
{
  GimpImage   *gimage;
  GimpContext *context;
  PixelRegion  srcPR;
  gint         offx, offy;
  gchar        col[MAX_CHANNELS];

  if (! (gimage = gimp_item_get_image (GIMP_ITEM (drawable))))
    return;

  context = gimp_get_current_context (gimage->gimp);

  /* Get the the buffer */
  ink_set_paint_area (ink_tool, drawable, blob);
 
  /* check to make sure there is actually a canvas to draw on */
  if (!canvas_buf)
    return;

  gimp_image_get_foreground (gimage, drawable, col);

  /*  set the alpha channel  */
  col[canvas_buf->bytes - 1] = OPAQUE_OPACITY;

  /*  color the pixels  */
  color_pixels (temp_buf_data (canvas_buf), col,
		canvas_buf->width * canvas_buf->height, canvas_buf->bytes);

  /*  set undo blocks  */
  ink_set_undo_tiles (drawable,
		      canvas_buf->x, canvas_buf->y,
		      canvas_buf->width, canvas_buf->height);

  /*  initialize any invalid canvas tiles  */
  ink_set_canvas_tiles (canvas_buf->x, canvas_buf->y,
			canvas_buf->width, canvas_buf->height);

  ink_to_canvas_tiles (ink_tool, blob, col);

  /*  initialize canvas buf source pixel regions  */
  srcPR.bytes = canvas_buf->bytes;
  srcPR.x = 0; srcPR.y = 0;
  srcPR.w = canvas_buf->width;
  srcPR.h = canvas_buf->height;
  srcPR.rowstride = canvas_buf->width * canvas_buf->bytes;
  srcPR.data = temp_buf_data (canvas_buf);

  /*  apply the paint area to the gimage  */
  gimp_image_apply_image (gimage, drawable, &srcPR,
			  FALSE, 
			  gimp_context_get_opacity (context),
			  gimp_context_get_paint_mode (context),
			  undo_tiles,  /*  specify an alternative src1  */
			  canvas_buf->x, canvas_buf->y);

  /*  Update the undo extents  */
  ink_tool->x1 = MIN (ink_tool->x1, canvas_buf->x);
  ink_tool->y1 = MIN (ink_tool->y1, canvas_buf->y);
  ink_tool->x2 = MAX (ink_tool->x2, (canvas_buf->x + canvas_buf->width));
  ink_tool->y2 = MAX (ink_tool->y2, (canvas_buf->y + canvas_buf->height));

  /*  Update the gimage--it is important to call gimp_image_update
   *  instead of drawable_update because we don't want the drawable
   *  preview to be constantly invalidated
   */
  gimp_drawable_offsets (drawable, &offx, &offy);
  gimp_image_update (gimage,
                     canvas_buf->x + offx,
                     canvas_buf->y + offy,
                     canvas_buf->width,
                     canvas_buf->height);
}

/* This routine a) updates the representation of the stroke
 * in the canvas tiles, then renders the dirty bit of it
 * into canvas_buf.
 */
static void
ink_to_canvas_tiles (GimpInkTool *ink_tool,
		     Blob        *blob,
		     guchar      *color)
{
  PixelRegion srcPR, maskPR;

  /*  draw the blob on the canvas tiles  */
  pixel_region_init (&srcPR, canvas_tiles,
		     canvas_buf->x, canvas_buf->y,
		     canvas_buf->width, canvas_buf->height, TRUE);

  render_blob (&srcPR, blob);

  /*  combine the canvas tiles and the canvas buf  */
  srcPR.bytes = canvas_buf->bytes;
  srcPR.x = 0; srcPR.y = 0;
  srcPR.w = canvas_buf->width;
  srcPR.h = canvas_buf->height;
  srcPR.rowstride = canvas_buf->width * canvas_buf->bytes;
  srcPR.data = temp_buf_data (canvas_buf);

  pixel_region_init (&maskPR, canvas_tiles,
		     canvas_buf->x, canvas_buf->y,
		     canvas_buf->width, canvas_buf->height, FALSE);

  /*  apply the canvas tiles to the canvas buf  */
  apply_mask_to_region (&srcPR, &maskPR, OPAQUE_OPACITY);
}

static void
ink_set_undo_tiles (GimpDrawable *drawable,
		    gint          x,
		    gint          y,
		    gint          w,
		    gint          h)
{
  gint  i, j;
  Tile *src_tile;
  Tile *dest_tile;

  for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))
    {
      for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH)))
	{
	  dest_tile = tile_manager_get_tile (undo_tiles, j, i, FALSE, FALSE);
	  if (tile_is_valid (dest_tile) == FALSE)
	    {
	      src_tile = tile_manager_get_tile (gimp_drawable_data (drawable),
						j, i, TRUE, FALSE);
	      tile_manager_map_tile (undo_tiles, j, i, src_tile);
	      tile_release (src_tile, FALSE);
	    }
	}
    }
}


static void
ink_set_canvas_tiles (gint x,
		      gint y,
		      gint w,
		      gint h)
{
  gint  i, j;
  Tile *tile;

  for (i = y; i < (y + h); i += (TILE_HEIGHT - (i % TILE_HEIGHT)))
    {
      for (j = x; j < (x + w); j += (TILE_WIDTH - (j % TILE_WIDTH)))
	{
	  tile = tile_manager_get_tile (canvas_tiles, j, i, FALSE, FALSE);
	  if (tile_is_valid (tile) == FALSE)
	    {
	      tile = tile_manager_get_tile (canvas_tiles, j, i, TRUE, TRUE);
	      memset (tile_data_pointer (tile, 0, 0), 
		      0, 
		      tile_size (tile));
	      tile_release (tile, TRUE);
	    }
	}
    }
}


/*  tool options stuff  */

static GimpToolOptions *
ink_options_new (GimpToolInfo *tool_info)
{
  InkOptions *options;
  GtkWidget  *table;
  GtkWidget  *vbox;
  GtkWidget  *hbox;
  GtkWidget  *hbox2;
  GtkWidget  *radio_button;
  GtkWidget  *blob;
  GtkWidget  *frame;
  GtkWidget  *darea;

  options = g_new0 (InkOptions, 1);

  gimp_paint_options_init ((GimpPaintOptions *) options, tool_info->context);

  paint_options_init ((GimpPaintOptions *) options, tool_info);

  ((GimpToolOptions *) options)->reset_func = ink_options_reset;

  options->size             = options->size_d             = 4.4;
  options->sensitivity      = options->sensitivity_d      = 1.0;
  options->vel_sensitivity  = options->vel_sensitivity_d  = 0.8;
  options->tilt_sensitivity = options->tilt_sensitivity_d = 0.4;
  options->tilt_angle       = options->tilt_angle_d       = 0.0;
  options->function         = options->function_d         = blob_ellipse;
  options->aspect           = options->aspect_d           = 1.0;
  options->angle            = options->angle_d            = 0.0;

  /*  the main vbox  */
  vbox = gtk_vbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (((GimpToolOptions *) options)->main_vbox), vbox,
		      TRUE, TRUE, 0);
  gtk_widget_show (vbox);

  /* adjust sliders */
  frame = gtk_frame_new (_("Adjustment"));
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, TRUE, 0);
  gtk_widget_show (frame);

  table = gtk_table_new (2, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 2);
  gtk_container_set_border_width (GTK_CONTAINER (table), 2);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  /*  size slider  */
  options->size_w = gimp_scale_entry_new (GTK_TABLE (table), 0, 0,
					  _("Size:"), -1, 5,
					  options->size,
					  0.0, 20.0, 1.0, 2.0, 1,
					  TRUE, 0.0, 0.0,
					  NULL, NULL);

  g_signal_connect (G_OBJECT (options->size_w), "value_changed",
		    G_CALLBACK (gimp_double_adjustment_update),
		    &options->size);

  /* angle adjust slider */
  options->tilt_angle_w = gimp_scale_entry_new (GTK_TABLE (table), 0, 1,
						_("Angle:"), -1, 5,
						options->tilt_angle,
						-90.0, 90.0, 1, 10.0, 1,
						TRUE, 0.0, 0.0,
						NULL, NULL);

  g_signal_connect (G_OBJECT (options->tilt_angle_w), "value_changed",
		    G_CALLBACK (gimp_double_adjustment_update),
		    &options->tilt_angle);

  /* sens sliders */
  frame = gtk_frame_new (_("Sensitivity"));
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, TRUE, 0);
  gtk_widget_show (frame);

  table = gtk_table_new (3, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 2);
  gtk_container_set_border_width (GTK_CONTAINER (table), 2);
  gtk_container_add (GTK_CONTAINER (frame), table);
  gtk_widget_show (table);

  /* size sens slider */
  options->sensitivity_w = gimp_scale_entry_new (GTK_TABLE (table), 0, 0,
						 _("Size:"), -1, -1,
						 options->sensitivity,
						 0.0, 1.0, 0.01, 0.1, 1,
						 TRUE, 0.0, 0.0,
						 NULL, NULL);

  g_signal_connect (G_OBJECT (options->sensitivity_w), "value_changed",
		    G_CALLBACK (gimp_double_adjustment_update),
		    &options->sensitivity);
  
  /* tilt sens slider */
  options->tilt_sensitivity_w = gimp_scale_entry_new (GTK_TABLE (table), 0, 1,
						      _("Tilt:"), -1, -1,
						      options->tilt_sensitivity,
						      0.0, 1.0, 0.01, 0.1, 1,
						      TRUE, 0.0, 0.0,
						      NULL, NULL);

  g_signal_connect (G_OBJECT (options->tilt_sensitivity_w), "value_changed",
		    G_CALLBACK (gimp_double_adjustment_update),
		    &options->tilt_sensitivity);

  /* velocity sens slider */
  options->vel_sensitivity_w = gimp_scale_entry_new (GTK_TABLE (table), 0, 2,
						     _("Speed:"), -1, -1,
						     options->vel_sensitivity,
						     0.0, 1.0, 0.01, 0.1, 1,
						     TRUE, 0.0, 0.0,
						     NULL, NULL);

  g_signal_connect (G_OBJECT (options->vel_sensitivity_w), "value_changed",
		    G_CALLBACK (gimp_double_adjustment_update),
		    &options->vel_sensitivity);

  /*  bottom hbox */
  hbox = gtk_hbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  /* Brush type radiobuttons */
  frame = gtk_frame_new (_("Type"));
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (frame), hbox2);
  
  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 2);
  gtk_box_pack_start (GTK_BOX (hbox2), vbox, FALSE, FALSE, 0);
  gtk_widget_show (vbox);

  blob = gtk_drawing_area_new ();
  gtk_widget_set_size_request (blob, 21, 21);
  g_signal_connect (G_OBJECT (blob), "expose_event",
                    G_CALLBACK (blob_button_expose),
                    blob_ellipse);

  radio_button = gtk_radio_button_new (NULL);
  gtk_container_add (GTK_CONTAINER (radio_button), blob);
  gtk_box_pack_start (GTK_BOX (vbox), radio_button, FALSE, FALSE, 0);

  g_object_set_data (G_OBJECT (radio_button), "gimp-item-data", blob_ellipse);

  g_signal_connect (G_OBJECT (radio_button), "toggled",
		    G_CALLBACK (ink_type_update),
		    options);

  options->function_w[0] = radio_button;

  blob = gtk_drawing_area_new ();
  gtk_widget_set_size_request (blob, 21, 21);
  g_signal_connect (G_OBJECT (blob), "expose_event",
                    G_CALLBACK (blob_button_expose),
                    blob_square);
  radio_button =
    gtk_radio_button_new_from_widget (GTK_RADIO_BUTTON (radio_button));
  gtk_container_add (GTK_CONTAINER (radio_button), blob);
  gtk_box_pack_start (GTK_BOX (vbox), radio_button, FALSE, FALSE, 0);

  g_object_set_data (G_OBJECT (radio_button), "gimp-item-data", blob_square);

  g_signal_connect (G_OBJECT (radio_button), "toggled",
		    G_CALLBACK (ink_type_update), 
		    options);
  

  options->function_w[1] = radio_button;

  blob = gtk_drawing_area_new ();
  gtk_widget_set_size_request (blob, 21, 21);
  g_signal_connect (G_OBJECT (blob), "expose_event",
                    G_CALLBACK (blob_button_expose),
                    blob_diamond);
  radio_button =
    gtk_radio_button_new_from_widget (GTK_RADIO_BUTTON (radio_button));
  gtk_container_add (GTK_CONTAINER (radio_button), blob);
  gtk_box_pack_start (GTK_BOX (vbox), radio_button, FALSE, FALSE, 0);

  g_object_set_data (G_OBJECT (radio_button), "gimp-item-data", blob_diamond);

  g_signal_connect (G_OBJECT (radio_button), "toggled",
		    G_CALLBACK (ink_type_update), 
		    options);

  options->function_w[2] = radio_button;

  /* Brush shape widget */
  frame = gtk_frame_new (_("Shape"));
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 2);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  frame = gtk_aspect_frame_new (NULL, 0.0, 0.5, 1.0, FALSE);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);

  options->brush_w = g_new (BrushWidget, 1);
  options->brush_w->state       = FALSE;
  options->brush_w->ink_options = options;

  darea = gtk_drawing_area_new ();
  options->brush_w->widget = darea;

  gtk_widget_set_size_request (darea, 60, 60);
  gtk_container_add (GTK_CONTAINER (frame), darea);

  gtk_widget_set_events (darea, 
			 GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
			 | GDK_POINTER_MOTION_MASK | GDK_EXPOSURE_MASK);

  g_signal_connect (G_OBJECT (darea), "button_press_event",
		    G_CALLBACK (brush_widget_button_press),
		    options->brush_w);
  g_signal_connect (G_OBJECT (darea), "button_release_event",
		    G_CALLBACK (brush_widget_button_release),
		    options->brush_w);
  g_signal_connect (G_OBJECT (darea), "motion_notify_event",
		    G_CALLBACK (brush_widget_motion_notify),
		    options->brush_w);
  g_signal_connect (G_OBJECT (darea), "expose_event",
		    G_CALLBACK (brush_widget_expose), 
		    options->brush_w);
  g_signal_connect (G_OBJECT (darea), "realize",
		    G_CALLBACK (brush_widget_realize),
		    options->brush_w);

  gtk_widget_show_all (hbox);

  ink_options_reset ((GimpToolOptions *) options);

  return (GimpToolOptions *) options;
}

static void
ink_options_reset (GimpToolOptions *tool_options)
{
  InkOptions *options;

  options = (InkOptions *) tool_options;

  paint_options_reset (tool_options);

  gtk_adjustment_set_value (GTK_ADJUSTMENT (options->size_w),
			    options->size_d);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (options->sensitivity_w),
			    options->sensitivity_d);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (options->tilt_sensitivity_w),
			    options->tilt_sensitivity_d);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (options->vel_sensitivity_w),
			    options->vel_sensitivity_d);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (options->tilt_angle_w),
			    options->tilt_angle_d);
  gtk_toggle_button_set_active (((options->function_d == blob_ellipse) ?
				 GTK_TOGGLE_BUTTON (options->function_w[0]) :
				 ((options->function_d == blob_square) ?
				  GTK_TOGGLE_BUTTON (options->function_w[1]) :
				  GTK_TOGGLE_BUTTON (options->function_w[2]))),
				TRUE);
  options->aspect = options->aspect_d;
  options->angle  = options->angle_d;
  gtk_widget_queue_draw (options->brush_w->widget);
}

static void
ink_type_update (GtkWidget  *radio_button,
		 InkOptions *options)
{
  BlobFunc function;

  function = g_object_get_data (G_OBJECT (radio_button), "gimp-item-data");

  if (GTK_TOGGLE_BUTTON (radio_button)->active)
    options->function = function;

  gtk_widget_queue_draw (options->brush_w->widget);
}
