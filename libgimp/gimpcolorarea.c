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

#include "gimp.h"

#include "gimpuitypes.h"

#include "gimpcolorarea.h"
#include "gimplimits.h"


#define DRAG_PREVIEW_SIZE   32
#define DRAG_ICON_OFFSET    -8


static const GtkTargetEntry targets[] = { { "application/x-color", 0 } };


struct _GimpColorArea
{
  GtkPreview  preview;

  gboolean    alpha;
  GimpRGB     color;
};

enum
{
  COLOR_CHANGED,
  LAST_SIGNAL
};

static guint           gimp_color_area_signals[LAST_SIGNAL] = { 0 };
static GtkWidgetClass *parent_class = NULL;


static void  gimp_color_area_class_init (GimpColorAreaClass *class);
static void  gimp_color_area_init       (GimpColorArea      *gca);
static void  gimp_color_area_destroy    (GtkObject          *object);
static void  gimp_color_area_paint      (GimpColorArea      *gca);

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

GtkType
gimp_color_area_get_type (void)
{
  static guint gca_type = 0;

  if (!gca_type)
    {
      GtkTypeInfo gca_info =
      {
	"GimpColorArea",
	sizeof (GimpColorArea),
	sizeof (GimpColorAreaClass),
	(GtkClassInitFunc) gimp_color_area_class_init,
	(GtkObjectInitFunc) gimp_color_area_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL
      };

      gca_type = gtk_type_unique (gtk_preview_get_type (), &gca_info);
    }
  
  return gca_type;
}

static void
gimp_color_area_class_init (GimpColorAreaClass *class)
{
  GtkObjectClass  *object_class;
  GtkWidgetClass  *widget_class;

  object_class  = (GtkObjectClass*) class;
  widget_class  = (GtkWidgetClass*) class;

  parent_class = gtk_type_class (gtk_preview_get_type ());

  gimp_color_area_signals[COLOR_CHANGED] = 
    gtk_signal_new ("color_changed",
		    GTK_RUN_FIRST,
		    object_class->type,
		    GTK_SIGNAL_OFFSET (GimpColorAreaClass,
				       color_changed),
		    gtk_marshal_NONE__POINTER , 
		    GTK_TYPE_NONE, 
		    1, GTK_TYPE_POINTER);

  gtk_object_class_add_signals (object_class, gimp_color_area_signals, 
				LAST_SIGNAL);

  class->color_changed = NULL;

  object_class->destroy       = gimp_color_area_destroy;

  widget_class->drag_begin         = gimp_color_area_drag_begin;
  widget_class->drag_end           = gimp_color_area_drag_end;
  widget_class->drag_data_received = gimp_color_area_drag_data_received;
  widget_class->drag_data_get      = gimp_color_area_drag_data_get;
}

static void
gimp_color_area_init (GimpColorArea *gca)
{
  gca->alpha = FALSE;
}

static void
gimp_color_area_destroy (GtkObject *object)
{
  GimpColorArea *gca;
   
  g_return_if_fail (object != NULL);
  g_return_if_fail (GIMP_IS_COLOR_AREA (object));
  
  gca = GIMP_COLOR_AREA (object);

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

/**
 * gimp_color_area_new:
 * @color: An array of guchar holding the color (RGB or RGBA)
 * @bpp: May be 3 for RGB or 4 for RGBA.
 * 
 * Creates a new #GimpColorArea widget.
 *
 * This returns a preview area showing the color. It handles color
 * DND. If the color changes, the "color_changed" signal is emitted.
 * 
 * Returns: Pointer to the new #GimpColorArea widget.
 **/
GtkWidget *
gimp_color_area_new (GimpRGB  *color,
		     gboolean  alpha)
{
  GimpColorArea *gca;

  g_return_val_if_fail (color != NULL, NULL); 
 
  gca = gtk_type_new (gimp_color_area_get_type ());

  gca->color = *color;
 
  GTK_PREVIEW (gca)->type   = GTK_PREVIEW_COLOR;
  GTK_PREVIEW (gca)->bpp    = 3;
  GTK_PREVIEW (gca)->dither = GDK_RGB_DITHER_NORMAL;
  GTK_PREVIEW (gca)->expand = TRUE;

  gtk_signal_connect_after (GTK_OBJECT (gca), "size_allocate", 
			    GTK_SIGNAL_FUNC (gimp_color_area_paint),
			    NULL);

  gtk_drag_dest_set (GTK_WIDGET (gca),
		     GTK_DEST_DEFAULT_HIGHLIGHT |
		     GTK_DEST_DEFAULT_MOTION |
		     GTK_DEST_DEFAULT_DROP,
		     targets, 1,
		     GDK_ACTION_COPY);
  gtk_drag_source_set (GTK_WIDGET (gca),
		       GDK_BUTTON2_MASK,
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
			   GimpRGB       *color)
{
  g_return_if_fail (gca != NULL);
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));

  g_return_if_fail (color != NULL);
 
  if (gimp_rgba_distance (&gca->color, color) > 0.000001)
    {
      gca->color = *color;
      
      gimp_color_area_paint (gca);
      
      gtk_signal_emit (GTK_OBJECT (gca),
		       gimp_color_area_signals[COLOR_CHANGED],
		       &gca->color);
    }
}

static void  
gimp_color_area_paint (GimpColorArea *gca)
{
  gint     x, y;
  gdouble  c0, c1;
  guint    width, height;
  guchar  *p0, *p1;
  guchar  *even, *odd;

  g_return_if_fail (gca != NULL);
  g_return_if_fail (GIMP_IS_COLOR_AREA (gca));

  if (! GTK_WIDGET_DRAWABLE (GTK_WIDGET (gca)))
    return;
      
  gdk_window_get_size (GTK_WIDGET (gca)->window, &width, &height);

  if (!width || !height)
    return;

  p0 = even = g_new (guchar, width * 3);
  p1 = odd  = g_new (guchar, width * 3);

  if (gca->alpha)
    {
      for (x = 0; x < width; x++)
	{
	  if ((x / GIMP_CHECK_SIZE_SM) & 1) 
	    {
	      c0 = GIMP_CHECK_LIGHT;
	      c1 = GIMP_CHECK_DARK;
	    } 
	  else 
	    {
	      c0 = GIMP_CHECK_DARK;
	      c1 = GIMP_CHECK_LIGHT;
	    }

	  *p0++ = (c0 + (gca->color.r - c0) * gca->color.a) * 255.999;
	  *p1++ = (c1 + (gca->color.r - c1) * gca->color.a) * 255.999;
	  *p0++ = (c0 + (gca->color.g - c0) * gca->color.a) * 255.999;
	  *p1++ = (c1 + (gca->color.g - c1) * gca->color.a) * 255.999;
	  *p0++ = (c0 + (gca->color.b - c0) * gca->color.a) * 255.999;
	  *p1++ = (c1 + (gca->color.b - c1) * gca->color.a) * 255.999;
	}

      for (y = 0; y < height; y++)
	{
	  if ((y / GIMP_CHECK_SIZE_SM) & 1)
	    gtk_preview_draw_row (GTK_PREVIEW (gca), odd, 0, y, width);
	  else
	    gtk_preview_draw_row (GTK_PREVIEW (gca), even, 0, y, width);
	} 
    }
  else
    {
      for (x = 0; x < width; x++)
	{
	  *p0++ = gca->color.r;
	  *p0++ = gca->color.g;
	  *p0++ = gca->color.b;
	}

      for (y = 0; y < height; y++)
	gtk_preview_draw_row (GTK_PREVIEW (gca), even, 0, y, width);
    }

  g_free (even);
  g_free (odd);

  gtk_widget_queue_draw (GTK_WIDGET (gca));
}

static void
gimp_color_area_drag_begin (GtkWidget      *widget,
			    GdkDragContext *context)
{
  GimpColorArea *gca;
  GtkWidget     *window;
  GdkColor       bg;

  gca = GIMP_COLOR_AREA (widget);

  window = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_widget_set_app_paintable (GTK_WIDGET (window), TRUE);
  gtk_widget_set_usize (window, DRAG_PREVIEW_SIZE, DRAG_PREVIEW_SIZE);
  gtk_widget_realize (window);
  gtk_object_set_data_full (GTK_OBJECT (widget),
			    "gimp-color-button-drag-window",
			    window,
			    (GtkDestroyNotify) gtk_widget_destroy);

  bg.red   = gca->color.r * 0xffff;
  bg.green = gca->color.g * 0xffff;
  bg.blue  = gca->color.b * 0xffff;

  gdk_color_alloc (gtk_widget_get_colormap (window), &bg);
  gdk_window_set_background (window->window, &bg);

  gtk_drag_set_icon_widget (context, window, DRAG_ICON_OFFSET, DRAG_ICON_OFFSET);
}

static void
gimp_color_area_drag_end (GtkWidget      *widget,
			  GdkDragContext *context)
{
  gtk_object_set_data (GTK_OBJECT (widget),
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
      g_warning ("Received invalid color data\n");
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
  vals[3] = gca->color.a * 0xffff;

  gtk_selection_data_set (selection_data,
			  gdk_atom_intern ("application/x-color", FALSE),
			  16, (guchar *)vals, 8);
}


