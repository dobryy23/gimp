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

#include <stdio.h>

#include <gtk/gtk.h>

#include "libgimpcolor/gimpcolor.h"

#include "widgets-types.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"

#include "gui/gui-types.h"
#include "gui/color-notebook.h"

#include "gimpcolorpanel.h"
#include "gimpitemfactory.h"

#include "app_procs.h"


struct _GimpColorPanel
{
  GimpColorButton  parent;

  ColorNotebook   *color_notebook;
  gboolean         color_notebook_active;
};


/*  local function prototypes  */

static void       gimp_color_panel_class_init      (GimpColorPanelClass *klass);
static void       gimp_color_panel_init            (GimpColorPanel      *panel);

static void       gimp_color_panel_destroy         (GtkObject           *object);
static gboolean   gimp_color_panel_button_press    (GtkWidget           *widget,
                                                    GdkEventButton      *bevent);
static void       gimp_color_panel_color_changed   (GimpColorButton     *button);
static void       gimp_color_panel_clicked         (GtkButton           *button);

static void       gimp_color_panel_select_callback (ColorNotebook       *notebook,
                                                    const GimpRGB       *color,
                                                    ColorNotebookState   state,
                                                    gpointer             data);


static GimpColorButtonClass *parent_class = NULL;


GType
gimp_color_panel_get_type (void)
{
  static GType panel_type = 0;

  if (! panel_type)
    {
      static const GTypeInfo panel_info =
      {
        sizeof (GimpColorPanelClass),
        NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gimp_color_panel_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GimpColorPanel),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gimp_color_panel_init,
      };

      panel_type = g_type_register_static (GIMP_TYPE_COLOR_BUTTON,
                                           "GimpColorPanel",
                                           &panel_info, 0);
    }
  
  return panel_type;
}

static void
gimp_color_panel_class_init (GimpColorPanelClass *klass)
{
  GtkObjectClass       *object_class;
  GtkWidgetClass       *widget_class;
  GtkButtonClass       *button_class;
  GimpColorButtonClass *color_button_class;

  object_class       = GTK_OBJECT_CLASS (klass);
  widget_class       = GTK_WIDGET_CLASS (klass);
  button_class       = GTK_BUTTON_CLASS (klass);
  color_button_class = GIMP_COLOR_BUTTON_CLASS (klass);
  
  parent_class = g_type_class_peek_parent (klass);

  object_class->destroy             = gimp_color_panel_destroy;
  widget_class->button_press_event  = gimp_color_panel_button_press;
  button_class->clicked             = gimp_color_panel_clicked;
  color_button_class->color_changed = gimp_color_panel_color_changed;
}

static void
gimp_color_panel_init (GimpColorPanel *panel)
{
  panel->color_notebook        = NULL;
  panel->color_notebook_active = FALSE;
}

static void
gimp_color_panel_destroy (GtkObject *object)
{
  GimpColorPanel *panel;

  g_return_if_fail (GIMP_IS_COLOR_PANEL (object));
  
  panel = GIMP_COLOR_PANEL (object);

  if (panel->color_notebook)
    {
      color_notebook_hide (panel->color_notebook);
      color_notebook_free (panel->color_notebook);
      panel->color_notebook = NULL;
    }

  GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

static gboolean
gimp_color_panel_button_press (GtkWidget      *widget,
                               GdkEventButton *bevent)
{
  if (bevent->button == 3)
    {
      GimpColorButton *color_button;
      GimpRGB          fg, bg, black, white;

      color_button = GIMP_COLOR_BUTTON (widget);

      gimp_context_get_foreground (gimp_get_user_context (the_gimp), &fg);
      gimp_context_get_background (gimp_get_user_context (the_gimp), &bg);
      gimp_rgba_set (&black, 0.0, 0.0, 0.0, 1.0);
      gimp_rgba_set (&white, 1.0, 1.0, 1.0, 1.0);

      gimp_item_factory_set_color (color_button->item_factory,
                                   "/Foreground Color", &fg, FALSE);
      gimp_item_factory_set_color (color_button->item_factory,
                                   "/Background Color", &bg, FALSE);
      gimp_item_factory_set_color (color_button->item_factory,
                                   "/Black", &black, FALSE);
      gimp_item_factory_set_color (color_button->item_factory,
                                   "/White", &white, FALSE);
    }

  if (GTK_WIDGET_CLASS (parent_class)->button_press_event)
    return GTK_WIDGET_CLASS (parent_class)->button_press_event (widget, bevent);

  return FALSE;
}

GtkWidget *
gimp_color_panel_new (const gchar       *title,
		      const GimpRGB     *color,
		      GimpColorAreaType  type,
		      gint               width,
		      gint               height)
{
  GimpColorPanel *panel;

  g_return_val_if_fail (color != NULL, NULL);

  panel = g_object_new (GIMP_TYPE_COLOR_PANEL, NULL);

  GIMP_COLOR_BUTTON (panel)->title = g_strdup (title);

  gimp_color_button_set_type (GIMP_COLOR_BUTTON (panel), type);
  gimp_color_button_set_color (GIMP_COLOR_BUTTON (panel), color);
  gtk_widget_set_size_request (GTK_WIDGET (panel), width, height);

  return GTK_WIDGET (panel);
}

static void
gimp_color_panel_color_changed (GimpColorButton *button)
{
  GimpColorPanel *panel;
  GimpRGB         color;

  panel = GIMP_COLOR_PANEL (button);

  if (panel->color_notebook_active)
    {
      gimp_color_button_get_color (GIMP_COLOR_BUTTON (button), &color);
      color_notebook_set_color (panel->color_notebook, &color);
    }
}

static void
gimp_color_panel_clicked (GtkButton *button)
{
  GimpColorPanel *panel;
  GimpRGB         color;

  panel = GIMP_COLOR_PANEL (button);

  gimp_color_button_get_color (GIMP_COLOR_BUTTON (button), &color);

  if (! panel->color_notebook)
    {
      panel->color_notebook =
	color_notebook_new (GIMP_COLOR_BUTTON (button)->title,
			    (const GimpRGB *) &color,
			    gimp_color_panel_select_callback,
			    panel,
			    FALSE,
			    gimp_color_button_has_alpha (GIMP_COLOR_BUTTON (button)));
      panel->color_notebook_active = TRUE;
    }
  else
    {
      if (! panel->color_notebook_active)
	{
	  color_notebook_show (panel->color_notebook);
	  panel->color_notebook_active = TRUE;
	}
      color_notebook_set_color (panel->color_notebook, &color);
    }
}

static void
gimp_color_panel_select_callback (ColorNotebook      *notebook,
				  const GimpRGB      *color,
				  ColorNotebookState  state,
				  gpointer            data)
{
  GimpColorPanel *panel;

  panel = GIMP_COLOR_PANEL (data);

  if (panel->color_notebook)
    {
      switch (state)
	{
	case COLOR_NOTEBOOK_UPDATE:
	  break;
	case COLOR_NOTEBOOK_OK:
	  gimp_color_button_set_color (GIMP_COLOR_BUTTON (panel), color);
	  /* Fallthrough */
	case COLOR_NOTEBOOK_CANCEL:
	  color_notebook_hide (panel->color_notebook);
	  panel->color_notebook_active = FALSE;
	}
    }
}
