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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "apptypes.h"

#include "gimpcontainer.h"
#include "gimpcontainerview.h"
#include "gimpdatacontainerview.h"
#include "gimpcontainergridview.h"
#include "gimpcontainerlistview.h"
#include "gimpcontext.h"
#include "gimpmarshal.h"
#include "gimpviewable.h"

#include "libgimp/gimpintl.h"

#include "pixmaps/delete.xpm"
#include "pixmaps/duplicate.xpm"
#include "pixmaps/new.xpm"
#include "pixmaps/pennorm.xpm"
#include "pixmaps/refresh.xpm"


static void   gimp_data_container_view_class_init (GimpDataContainerViewClass *klass);
static void   gimp_data_container_view_init       (GimpDataContainerView      *view);
static void   gimp_data_container_view_destroy    (GtkObject                  *object);

static void   gimp_data_container_view_new_clicked       (GtkWidget             *widget,
							  GimpDataContainerView *view);
static void   gimp_data_container_view_duplicate_clicked (GtkWidget             *widget,
							  GimpDataContainerView *view);
static void   gimp_data_container_view_edit_clicked      (GtkWidget             *widget,
							  GimpDataContainerView *view);
static void   gimp_data_container_view_delete_clicked    (GtkWidget             *widget,
							  GimpDataContainerView *view);
static void   gimp_data_container_view_refresh_clicked   (GtkWidget             *widget,
							  GimpDataContainerView *view);


static GtkVBoxClass *parent_class = NULL;


GtkType
gimp_data_container_view_get_type (void)
{
  static guint view_type = 0;

  if (! view_type)
    {
      GtkTypeInfo view_info =
      {
	"GimpDataContainerView",
	sizeof (GimpDataContainerView),
	sizeof (GimpDataContainerViewClass),
	(GtkClassInitFunc) gimp_data_container_view_class_init,
	(GtkObjectInitFunc) gimp_data_container_view_init,
	/* reserved_1 */ NULL,
	/* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL
      };

      view_type = gtk_type_unique (GTK_TYPE_VBOX, &view_info);
    }

  return view_type;
}

static void
gimp_data_container_view_class_init (GimpDataContainerViewClass *klass)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass *) klass;

  parent_class = gtk_type_class (GTK_TYPE_VBOX);

  object_class->destroy = gimp_data_container_view_destroy;
}

static void
gimp_data_container_view_init (GimpDataContainerView *view)
{
  GtkWidget *pixmap;

  view->view = NULL;

  gtk_box_set_spacing (GTK_BOX (view), 2);

  view->button_box = gtk_hbox_new (TRUE, 2);
  gtk_box_pack_end (GTK_BOX (view), view->button_box, FALSE, FALSE, 0);
  gtk_widget_show (view->button_box);

  /* new */

  view->new_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (view->button_box), view->new_button,
		      TRUE, TRUE, 0);
  gtk_widget_show (view->new_button);

  gimp_help_set_help_data (view->new_button, _("New"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->new_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_data_container_view_new_clicked),
		      view);

  pixmap = gimp_pixmap_new (new_xpm);
  gtk_container_add (GTK_CONTAINER (view->new_button), pixmap);
  gtk_widget_show (pixmap);

  /* duplicate */

  view->duplicate_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (view->button_box), view->duplicate_button,
		      TRUE, TRUE, 0);
  gtk_widget_show (view->duplicate_button);

  gimp_help_set_help_data (view->duplicate_button, _("Duplicate"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->duplicate_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_data_container_view_duplicate_clicked),
		      view);  

  pixmap = gimp_pixmap_new (duplicate_xpm);
  gtk_container_add (GTK_CONTAINER (view->duplicate_button), pixmap);
  gtk_widget_show (pixmap);

  /* edit */

  view->edit_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (view->button_box), view->edit_button,
		      TRUE, TRUE, 0);
  gtk_widget_show (view->edit_button);

  gimp_help_set_help_data (view->edit_button, _("Edit"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->edit_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_data_container_view_edit_clicked),
		      view);  

  pixmap = gimp_pixmap_new (pennorm_xpm);
  gtk_container_add (GTK_CONTAINER (view->edit_button), pixmap);
  gtk_widget_show (pixmap);

  /* delete */

  view->delete_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (view->button_box), view->delete_button,
		      TRUE, TRUE, 0);
  gtk_widget_show (view->delete_button);

  gimp_help_set_help_data (view->delete_button, _("Delete"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->delete_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_data_container_view_delete_clicked),
		      view);  

  pixmap = gimp_pixmap_new (delete_xpm);
  gtk_container_add (GTK_CONTAINER (view->delete_button), pixmap);
  gtk_widget_show (pixmap);

  /* refresh */

  view->refresh_button = gtk_button_new ();
  gtk_box_pack_start (GTK_BOX (view->button_box), view->refresh_button,
		      TRUE, TRUE, 0);
  gtk_widget_show (view->refresh_button);

  gimp_help_set_help_data (view->refresh_button, _("Refresh"), NULL);

  gtk_signal_connect (GTK_OBJECT (view->refresh_button), "clicked",
		      GTK_SIGNAL_FUNC (gimp_data_container_view_refresh_clicked),
		      view);  

  pixmap = gimp_pixmap_new (refresh_xpm);
  gtk_container_add (GTK_CONTAINER (view->refresh_button), pixmap);
  gtk_widget_show (pixmap);
}

static void
gimp_data_container_view_destroy (GtkObject *object)
{
  GimpDataContainerView *view;

  view = GIMP_DATA_CONTAINER_VIEW (object);

  if (GTK_OBJECT_CLASS (parent_class)->destroy)
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

GtkWidget *
gimp_data_container_view_new (GimpViewType   view_type,
			      GimpContainer *container,
                              GimpContext   *context,
                              gint           preview_size,
                              gint           min_items_x,
                              gint           min_items_y)
{
  GimpDataContainerView *data_view;

  g_return_val_if_fail (container != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_CONTAINER (container), NULL);
  g_return_val_if_fail (! context || GIMP_IS_CONTEXT (context), NULL);
  g_return_val_if_fail (preview_size > 0 && preview_size <= 64, NULL);
  g_return_val_if_fail (min_items_x > 0 && min_items_x <= 64, NULL);
  g_return_val_if_fail (min_items_y > 0 && min_items_y <= 64, NULL);

  data_view = gtk_type_new (GIMP_TYPE_DATA_CONTAINER_VIEW);

  switch (view_type)
    {
    case GIMP_VIEW_TYPE_GRID:
      data_view->view =
	GIMP_CONTAINER_VIEW (gimp_container_grid_view_new (container,
							   context,
							   preview_size,
							   min_items_x,
							   min_items_y));
      break;

    case GIMP_VIEW_TYPE_LIST:
      data_view->view =
	GIMP_CONTAINER_VIEW (gimp_container_list_view_new (container,
							   context,
							   preview_size,
							   min_items_x,
							   min_items_y));
      break;

    default:
      g_warning ("%s(): unknown GimpViewType passed", G_GNUC_FUNCTION);
      gtk_object_unref (GTK_OBJECT (data_view));
      return NULL;
    }

  gtk_container_add (GTK_CONTAINER (data_view), GTK_WIDGET (data_view->view));
  gtk_widget_show (GTK_WIDGET (data_view->view));

  return GTK_WIDGET (data_view);
}

static void
gimp_data_container_view_new_clicked (GtkWidget             *widget,
				      GimpDataContainerView *view)
{
  g_print ("new %s\n", gimp_object_get_name (gimp_context_get_by_type (view->view->context, view->view->container->children_type)));
}

static void
gimp_data_container_view_duplicate_clicked (GtkWidget             *widget,
					    GimpDataContainerView *view)
{
  g_print ("duplicate %s\n", gimp_object_get_name (gimp_context_get_by_type (view->view->context, view->view->container->children_type)));
}

static void
gimp_data_container_view_edit_clicked (GtkWidget             *widget,
				       GimpDataContainerView *view)
{
  g_print ("edit %s\n", gimp_object_get_name (gimp_context_get_by_type (view->view->context, view->view->container->children_type)));
}

static void
gimp_data_container_view_delete_clicked (GtkWidget             *widget,
					 GimpDataContainerView *view)
{
  g_print ("delete %s\n", gimp_object_get_name (gimp_context_get_by_type (view->view->context, view->view->container->children_type)));
}

static void
gimp_data_container_view_refresh_clicked (GtkWidget             *widget,
					  GimpDataContainerView *view)
{
  g_print ("refresh\n");
}
