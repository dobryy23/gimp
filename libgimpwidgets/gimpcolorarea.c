/* LIBGIMP - The GIMP Library 
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball                
 *
 * gimpcolorarea.c
 * Copyright (C) 2001 Sven Neumann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "libgimpcolor/gimpcolor.h"
#include "libgimpbase/gimpbase.h"

#include "gimpwidgetstypes.h"

#include "gimpcolorarea.h"


#define DRAG_PREVIEW_SIZE   32
#define DRAG_ICON_OFFSET    -8


enum
{
  COLOR_CHANGED,
  LAST_SIGNAL
};


static void  gimp_color_area_class_init    (GimpColorAreaClass *klass);
static void  gimp_color_area_init          (GimpColorArea      *gca);

static void  gimp_color_area_destroy       (GtkObject          *object);

static void  gimp_color_area_size_allocate (GtkWidget          *widget,
                                            GtkAllocation      *allocation);
static gboolean   gimp_color_area_expose   (GtkWidget          *widget,
                                            GdkEventExpose     *event);

static void       gimp_color_area_update        (GimpColorArea    *gca);
static gboolean   gimp_color_area_idle_update   (gpointer          data);

static void  gimp_color_area_drag_begin         (GtkWidget        *widget,
						 GdkDragContext   *context);
static void  gimp_color_area_drag_end           (GtkWidget        *widget,
						 GdkDragContext   *context);
static void  gimp_color_area_drag_data_received (GtkWidget        *widget, 
						 GdkDragContext   *context,
						 gint              x,
						 gint              y,
						 GtkSelectionData *selection_data,
						 guint             info,
						 guint             time);
static void  gimp_color_area_drag_data_get      (GtkWidget        *widget, 
						 GdkDragContext   *context,
						 GtkSelectionData *selection_data,
						 guint             info,
						 guint             time);


static const GtkTargetEntry targets[] = { { "application/x-color", 0 } };

static guint   gimp_color_area_signals[LAST_SIGNAL] = { 0 };

static GtkDrawingAreaClass * parent_class = NULL;


GType
gimp_color_area_get_type (void)
{
  static GType gca_type = 0;

  if (! gca_type)
    {
      static const GTypeInfo gca_info =
      {
        sizeof (GimpColorAreaClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gimp_color_area_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data     */
        sizeof (GimpColorArea),
        0,              /* n_preallocs    */
        (GInstanceInitFunc) gimp_color_area_init,
      };

      gca_type = g_type_register_static (GTK_TYPE_DRAWING_AREA,
                                         "GimpColorArea", 
                                         &gca_info, 0);
    }

  return gca_type;
}

static void
gimp_color_area_class_init (GimpColorAreaClass *klass)
{
  GtkObjectClass  *object_class;
  GtkWidgetClass  *widget_class;

  object_class  = GTK_OBJECT_CLASS (klass);
  widget_class  = GTK_WIDGET_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  gimp_color_area_signals[COLOR_CHANGED] = 
    g_signal_new ("color_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpColorAreaClass, color_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  object_class->destroy            = gimp_color_area_destroy;

  widget_class->size_allocate      = gimp_color_area_size_allocate;
  widget_class->expose_event       = gimp_color_area_expose;

  widget_class->drag_begin         = gimp_color_area_drag_begin;
  widget_class->drag_end           = gimp_color_area_drag_end;
  widget_class->drag_data_received = gimp_color_area_drag_data_received;
  widget_class->drag_data_get      = gimp_color_area_drag_data_get;

  klass->color_changed             = NULL;
}

static void
gimp_color_area_init (GimpColorArea *gca)
{
  gimp_rgba_set (&gca->color, 0.0, 0.0, 0.0, 1.0);

  gca->buf       = NULL;
  gca->width     = 0;
  gca->height    = 0;
  gca->rowstride = 0;

  gca->type      = GIMP_COLOR_AREA_FLAT;
  gca->idle_id   = 0;
}

static void
gimp_color_area_destroy (GtkObject *object)
{
  GimpColorArea *gca;

  g_return_if_fail (GIMP_IS_COLOR_AREA (object));
  
  gca = GIMP_COLOR_AREA (object);

  if (gca->buf)
    {
      g_free (gca->buf);
      gca->buf       = NULL;
      gca->width     = 0;
      gca->height    = 0;
      gca->rowstride = 0;
    }

  if (gca->idle_id)
    {
      g_source_remove (gca->idle_id);
      gca->idle_id = 0;
    }

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static void
gimp_color_area_size_allocate (GtkWidget     *widget,
			       GtkAllocation *allocation)
{
  GimpColorArea *gca;

  gca = GIMP_COLOR_AREA (widget);

  if (GTK_WIDGET_CLASS (parent_class)->size_allocate)
    GTK_WIDGET_CLASS (parent_class)->size_allocate (widget, allocation);

  if (widget->allocation.width  != gca->width ||
      widget->allocation.height != gca->height)
    {
      gca->width  = widget->allocation.width;
      gca->height = widget->allocation.height;

      gca->rowstride = (gca->width * 3 + 3) & ~0x3;

      g_free (gca->buf);
      gca->buf = g_new (guchar, gca->rowstride * gca->height);

      gimp_color_area_update (gca);
    } 
}

static gboolean
gimp_color_area_expose (GtkWidget      *widget,
                        GdkEventExpose *event)
{
  GimpColorArea *gca;
  guchar        *buf;

  gca = GIMP_COLOR_AREA (widget);
  
  if (gca->idle_id || ! gca->buf || ! GTK_WIDGET_DRAWABLE (widget))
    return FALSE;

  buf = gca->buf + event->area.y * gca->rowstride + event->area.x * 3;

  gdk_draw_rgb_image_dithalign (widget->window,
                                widget->style->black_gc,
                                event->area.x,
                                event->area.y,
                                event->area.width,
                                event->area.height,
                                GDK_RGB_DITHER_MAX,
                                buf,
                                gca->rowstride,
                                event->area.x,
                                event->area.y);

  return FALSE;
}

/**
 * gimp_color_area_new:
 * @color:     A pointer to a #GimpRGB struct.
 * @type:      The type of color area to create.
 * @drag_mask: The event_mask that should trigger drags.
 * 
 * Creates a new #GimpColorArea widget.
 *
 * This returns a preview area showing the color. It handles color
 * DND. If the color changes, the "color_changed" signal is emitted.
 * 
 * Returns: Pointer to the new #GimpColorArea widget.
 **/
GtkWidget *
gimp_color_area_new (const GimpRGB     *color,
		     GimpColorAreaType  type,
		     GdkModifierType    drag_mask)
{
  GimpColorArea *gca;

  g_return_val_if_fail (color != NULL, NULL); 
 
  gca = g_object_new (GIMP_TYPE_COLOR_AREA, NULL);

  gca->color = *color;
  gca->type  = type;
 
  gtk_drag_dest_set (GTK_WIDGET (gca),
		     GTK_DEST_DEFAULT_HIGHLIGHT |
		     GTK_DEST_DEFAULT_MOTION |
		     GTK_DEST_DEFAULT_DROP,
		     targets, 1,
		     GDK_ACTION_COPY);

  /*  do we need this ??  */
  drag_mask &= (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK);

  if (drag_mask)
    gtk_drag_source_set (GTK_WIDGET (gca),
			 drag_mask,
			 targets, 1,
			 GDK_ACTION_COPY | GDK_ACTION_MOVE);

  return GTK_WIDGET (gca);
}

/**
 * gimp_color_area_set_color:
 * @gca: Pointer to a #GimpColorArea.
 * @color:
 * 
 **/
void       
gimp_color_area_set_color (GimpColorArea *gca,
			   const GimpRGB *color)
{
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));
  g_return_if_fail (color != NULL);

  if (gimp_rgba_distance (&gca->color, color) > 0.000001)
    {
      gca->color = *color;

      gimp_color_area_update (gca);

      g_signal_emit (G_OBJECT (gca),
                     gimp_color_area_signals[COLOR_CHANGED], 0);
    }
}

void
gimp_color_area_get_color (GimpColorArea *gca,
			   GimpRGB       *color)
{
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));
  g_return_if_fail (color != NULL);

  *color = gca->color;
}

gboolean    
gimp_color_area_has_alpha (GimpColorArea *gca)
{
  g_return_val_if_fail (GIMP_IS_COLOR_AREA (gca), FALSE);

  return gca->type != GIMP_COLOR_AREA_FLAT;
}

void
gimp_color_area_set_type (GimpColorArea     *gca,
			  GimpColorAreaType  type)
{
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));

  gca->type = type;
  gimp_color_area_update (gca);
}

static void
gimp_color_area_update (GimpColorArea *gca)
{
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));

  if (gca->idle_id)
    {
      g_source_remove (gca->idle_id);
    }

  gca->idle_id = g_idle_add_full (G_PRIORITY_LOW,
                                  (GSourceFunc) gimp_color_area_idle_update, 
                                  gca,
                                  NULL);
}

static gboolean
gimp_color_area_idle_update (gpointer data)
{
  GimpColorArea *gca;
  GtkWidget     *widget;
  guint          width, height;
  guint          x, y;
  guint          check_size = 0;
  guchar         light[3];
  guchar         dark[3];
  guchar         opaque[3];
  guchar        *p;
  gdouble        frac;

  gca    = GIMP_COLOR_AREA (data);
  widget = GTK_WIDGET (data);

  gca->idle_id = 0;

  if (! GTK_WIDGET_REALIZED (widget))
    return FALSE;

  if (! gca->buf)
    return FALSE;

  width  = gca->width;
  height = gca->height;

  switch (gca->type)
    {
    case GIMP_COLOR_AREA_FLAT:
      check_size = 0;
      break;

    case GIMP_COLOR_AREA_SMALL_CHECKS:
      check_size = GIMP_CHECK_SIZE_SM;
      break;

    case GIMP_COLOR_AREA_LARGE_CHECKS:
      check_size = GIMP_CHECK_SIZE;
      break;
    }
      
  opaque[0] = gca->color.r * 255.999;
  opaque[1] = gca->color.g * 255.999;
  opaque[2] = gca->color.b * 255.999;

  if (check_size && gca->color.a < 1.0)
    {
      light[0] = (GIMP_CHECK_LIGHT + 
		  (gca->color.r - GIMP_CHECK_LIGHT) * gca->color.a) * 255.999;
      dark[0]  = (GIMP_CHECK_DARK + 
		  (gca->color.r - GIMP_CHECK_DARK)  * gca->color.a) * 255.999;
      light[1] = (GIMP_CHECK_LIGHT + 
		  (gca->color.g - GIMP_CHECK_LIGHT) * gca->color.a) * 255.999;
      dark[1]  = (GIMP_CHECK_DARK + 
		  (gca->color.g - GIMP_CHECK_DARK)  * gca->color.a) * 255.999;
      light[2] = (GIMP_CHECK_LIGHT + 
		  (gca->color.b - GIMP_CHECK_LIGHT) * gca->color.a) * 255.999;
      dark[2]  = (GIMP_CHECK_DARK + 
		  (gca->color.b - GIMP_CHECK_DARK)  * gca->color.a) * 255.999;

      for (y = 0; y < height; y++)
	{
	  p = gca->buf + y * gca->rowstride;

	  for (x = 0; x < width; x++)
	    {
	      if ((width - x) * height > y * width)
		{
		  *p++ = opaque[0];
		  *p++ = opaque[1];
		  *p++ = opaque[2];

		  continue;
		}

	      frac = y - (gdouble) ((width - x) * height) / (gdouble) width;

	      if (((x / check_size) ^ (y / check_size)) & 1) 
		{
		  if ((gint) frac)
		    {
		      *p++ = light[0];
		      *p++ = light[1];
		      *p++ = light[2];
		    }
		  else
		    {
		      *p++ = (gdouble) light[0]  * frac + 
			     (gdouble) opaque[0] * (1.0 - frac);
		      *p++ = (gdouble) light[1]  * frac + 
			     (gdouble) opaque[1] * (1.0 - frac);
		      *p++ = (gdouble) light[2]  * frac + 
			     (gdouble) opaque[2] * (1.0 - frac);
		    }
		}
	      else
		{
		  if ((gint) frac)
		    {
		      *p++ = dark[0];
		      *p++ = dark[1];
		      *p++ = dark[2];
		    }
		  else
		    {
		      *p++ = (gdouble) dark[0] * frac + 
			     (gdouble) opaque[0] * (1.0 - frac);
		      *p++ = (gdouble) dark[1] * frac + 
			     (gdouble) opaque[1] * (1.0 - frac);
		      *p++ = (gdouble) dark[2] * frac + 
			     (gdouble) opaque[2] * (1.0 - frac);
		    }
		}
	    }
	} 
    }
  else
    {
      for (y = 0; y < height; y++)
	{
	  p = gca->buf + y * gca->rowstride;

          for (x = 0; x < width; x++)
            {
              *p++ = opaque[0];
              *p++ = opaque[1];
              *p++ = opaque[2];
            }
        }
    }

  gtk_widget_queue_draw (GTK_WIDGET (gca));

  return FALSE;
}

static void
gimp_color_area_drag_begin (GtkWidget      *widget,
			    GdkDragContext *context)
{
  GimpRGB    color;
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *color_area;

  window = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_widget_realize (window);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER (window), frame);

  gimp_color_area_get_color (GIMP_COLOR_AREA (widget), &color);

  color_area = gimp_color_area_new (&color,
                                    GIMP_COLOR_AREA (widget)->type,
                                    0);

  gtk_widget_set_size_request (color_area,
                               DRAG_PREVIEW_SIZE, DRAG_PREVIEW_SIZE);
  gtk_container_add (GTK_CONTAINER (frame), color_area);
  gtk_widget_show (color_area);
  gtk_widget_show (frame);

  g_object_set_data_full (G_OBJECT (widget),
                          "gimp-color-area-drag-window",
                          window,
                          (GDestroyNotify) gtk_widget_destroy);

  gtk_drag_set_icon_widget (context, window, 
                            DRAG_ICON_OFFSET, DRAG_ICON_OFFSET);
}

static void
gimp_color_area_drag_end (GtkWidget      *widget,
			  GdkDragContext *context)
{
  g_object_set_data (G_OBJECT (widget),
                     "gimp-color-area-drag-window", NULL);
}

static void
gimp_color_area_drag_data_received (GtkWidget        *widget, 
				    GdkDragContext   *context,
				    gint              x,
				    gint              y,
				    GtkSelectionData *selection_data,
				    guint             info,
				    guint             time)
{
  GimpColorArea *gca;
  GimpRGB        color;
  guint16       *vals;

  gca = GIMP_COLOR_AREA (widget);

  if (selection_data->length < 0)
    return;

  if ((selection_data->format != 16) || 
      (selection_data->length != 8))
    {
      g_warning ("Received invalid color data");
      return;
    }

  vals = (guint16 *)selection_data->data;

  gimp_rgba_set (&color, 
		 (gdouble) vals[0] / 0xffff,
		 (gdouble) vals[1] / 0xffff,
		 (gdouble) vals[2] / 0xffff,
		 (gdouble) vals[3] / 0xffff);

  gimp_color_area_set_color (gca, &color);
}

static void
gimp_color_area_drag_data_get (GtkWidget        *widget, 
			       GdkDragContext   *context,
			       GtkSelectionData *selection_data,
			       guint             info,
			       guint             time)
{
  GimpColorArea *gca;
  guint16        vals[4];

  gca = GIMP_COLOR_AREA (widget);

  vals[0] = gca->color.r * 0xffff;
  vals[1] = gca->color.g * 0xffff;
  vals[2] = gca->color.b * 0xffff;

  if (gca->type == GIMP_COLOR_AREA_FLAT)
    vals[3] = 0xffff;
  else
    vals[3] = gca->color.a * 0xffff;

  gtk_selection_data_set (selection_data,
			  gdk_atom_intern ("application/x-color", FALSE),
			  16, (guchar *)vals, 8);
}
