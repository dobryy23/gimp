/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpitemtreeview.c
 * Copyright (C) 2001-2003 Michael Natterer <mitch@gimp.org>
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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "widgets-types.h"

#include "core/gimpchannel.h"
#include "core/gimpcontainer.h"
#include "core/gimpimage.h"
#include "core/gimpimage-undo-push.h"
#include "core/gimplayer.h"
#include "core/gimpmarshal.h"

#include "vectors/gimpvectors.h"

#include "gimpchanneltreeview.h"
#include "gimpdnd.h"
#include "gimpitemtreeview.h"
#include "gimpitemfactory.h"
#include "gimplayertreeview.h"
#include "gimpmenufactory.h"
#include "gimppreviewrenderer.h"
#include "gimpvectorstreeview.h"
#include "gimpwidgets-utils.h"

#include "libgimp/gimpintl.h"


enum
{
  SET_IMAGE,
  LAST_SIGNAL
};


static void   gimp_item_tree_view_class_init (GimpItemTreeViewClass *klass);
static void   gimp_item_tree_view_init       (GimpItemTreeView      *view,
                                              GimpItemTreeViewClass *view_class);

static GObject * gimp_item_tree_view_constructor    (GType              type,
                                                     guint              n_params,
                                                     GObjectConstructParam *params);
static void   gimp_item_tree_view_destroy           (GtkObject         *object);

static void   gimp_item_tree_view_real_set_image    (GimpItemTreeView  *view,
                                                     GimpImage         *gimage);

static void   gimp_item_tree_view_select_item       (GimpContainerView *view,
                                                     GimpViewable      *item,
                                                     gpointer           insert_data);
static void   gimp_item_tree_view_activate_item     (GimpContainerView *view,
                                                     GimpViewable      *item,
                                                     gpointer           insert_data);
static void   gimp_item_tree_view_context_item      (GimpContainerView *view,
                                                     GimpViewable      *item,
                                                     gpointer           insert_data);

static void   gimp_item_tree_view_real_rename_item  (GimpItemTreeView  *view,
                                                     GimpItem          *item,
                                                     const gchar       *new_name);

static void   gimp_item_tree_view_new_clicked       (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_new_dropped       (GtkWidget         *widget,
                                                     GimpViewable      *viewable,
                                                     gpointer           data);

static void   gimp_item_tree_view_raise_clicked     (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_raise_extended_clicked
                                                    (GtkWidget         *widget,
                                                     guint              state,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_lower_clicked     (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_lower_extended_clicked
                                                    (GtkWidget         *widget,
                                                     guint              state,
                                                     GimpItemTreeView  *view);

static void   gimp_item_tree_view_duplicate_clicked (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_edit_clicked      (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_delete_clicked    (GtkWidget         *widget,
                                                     GimpItemTreeView  *view);

static void   gimp_item_tree_view_item_changed      (GimpImage         *gimage,
                                                     GimpItemTreeView  *view);
static void   gimp_item_tree_view_size_changed      (GimpImage         *gimage,
                                                     GimpItemTreeView  *view);

static void   gimp_item_tree_view_name_edited       (GtkCellRendererText *cell,
                                                     const gchar       *path,
                                                     const gchar       *name,
                                                     GimpItemTreeView  *view);


static guint  view_signals[LAST_SIGNAL] = { 0 };

static GimpContainerTreeViewClass *parent_class = NULL;


GType
gimp_item_tree_view_get_type (void)
{
  static GType view_type = 0;

  if (! view_type)
    {
      static const GTypeInfo view_info =
      {
        sizeof (GimpItemTreeViewClass),
        NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gimp_item_tree_view_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (GimpItemTreeView),
        0,              /* n_preallocs */
        (GInstanceInitFunc) gimp_item_tree_view_init,
      };

      view_type = g_type_register_static (GIMP_TYPE_CONTAINER_TREE_VIEW,
                                          "GimpItemTreeView",
                                          &view_info, 0);
    }

  return view_type;
}

static void
gimp_item_tree_view_class_init (GimpItemTreeViewClass *klass)
{
  GObjectClass           *object_class;
  GtkObjectClass         *gtk_object_class;
  GimpContainerViewClass *container_view_class;

  object_class         = G_OBJECT_CLASS (klass);
  gtk_object_class     = GTK_OBJECT_CLASS (klass);
  container_view_class = GIMP_CONTAINER_VIEW_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  view_signals[SET_IMAGE] =
    g_signal_new ("set_image",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GimpItemTreeViewClass, set_image),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_OBJECT);

  object_class->constructor           = gimp_item_tree_view_constructor;

  gtk_object_class->destroy           = gimp_item_tree_view_destroy;

  container_view_class->select_item   = gimp_item_tree_view_select_item;
  container_view_class->activate_item = gimp_item_tree_view_activate_item;
  container_view_class->context_item  = gimp_item_tree_view_context_item;

  klass->set_image                    = gimp_item_tree_view_real_set_image;

  klass->get_container                = NULL;
  klass->get_active_item              = NULL;
  klass->set_active_item              = NULL;
  klass->reorder_item                 = NULL;
  klass->add_item                     = NULL;
  klass->remove_item                  = NULL;
  klass->convert_item                 = NULL;
  klass->rename_item                  = gimp_item_tree_view_real_rename_item;

  klass->new_desc                     = NULL;
  klass->duplicate_desc               = NULL;
  klass->edit_desc                    = NULL;
  klass->delete_desc                  = NULL;
  klass->raise_desc                   = NULL;
  klass->raise_to_top_desc            = NULL;
  klass->lower_desc                   = NULL;
  klass->lower_to_bottom_desc         = NULL;
}

static void
gimp_item_tree_view_init (GimpItemTreeView      *view,
                          GimpItemTreeViewClass *view_class)
{
  GimpEditor *editor;
  gchar      *str;

  editor = GIMP_EDITOR (view);

  view->gimage      = NULL;
  view->item_type   = G_TYPE_NONE;
  view->signal_name = NULL;

  view->new_button =
    gimp_editor_add_button (editor,
                            GTK_STOCK_NEW, view_class->new_desc, NULL,
                            G_CALLBACK (gimp_item_tree_view_new_clicked),
                            NULL,
                            view);

  str = g_strdup_printf (_("%s\n"
                           "%s  To Top"),
                         view_class->raise_desc,
                         gimp_get_mod_name_shift ());

  view->raise_button =
    gimp_editor_add_button (editor,
                            GTK_STOCK_GO_UP, str, NULL,
                            G_CALLBACK (gimp_item_tree_view_raise_clicked),
                            G_CALLBACK (gimp_item_tree_view_raise_extended_clicked),
                            view);

  g_free (str);

  str = g_strdup_printf (_("%s\n"
                           "%s  To Bottom"),
                         view_class->lower_desc,
                         gimp_get_mod_name_shift ());

  view->lower_button =
    gimp_editor_add_button (editor,
                            GTK_STOCK_GO_DOWN, str, NULL,
                            G_CALLBACK (gimp_item_tree_view_lower_clicked),
                            G_CALLBACK (gimp_item_tree_view_lower_extended_clicked),
                            view);

  g_free (str);

  view->duplicate_button =
    gimp_editor_add_button (editor,
                            GIMP_STOCK_DUPLICATE, view_class->duplicate_desc,
                            NULL,
                            G_CALLBACK (gimp_item_tree_view_duplicate_clicked),
                            NULL,
                            view);

  view->edit_button =
    gimp_editor_add_button (editor,
                            GIMP_STOCK_EDIT, view_class->edit_desc, NULL,
                            G_CALLBACK (gimp_item_tree_view_edit_clicked),
                            NULL,
                            view);

  view->delete_button =
    gimp_editor_add_button (editor,
                            GTK_STOCK_DELETE, view_class->delete_desc, NULL,
                            G_CALLBACK (gimp_item_tree_view_delete_clicked),
                            NULL,
                            view);

  gtk_widget_set_sensitive (view->new_button,       FALSE);
  gtk_widget_set_sensitive (view->raise_button,     FALSE);
  gtk_widget_set_sensitive (view->lower_button,     FALSE);
  gtk_widget_set_sensitive (view->duplicate_button, FALSE);
  gtk_widget_set_sensitive (view->edit_button,      FALSE);
  gtk_widget_set_sensitive (view->delete_button,    FALSE);
}

static GObject *
gimp_item_tree_view_constructor (GType                  type,
                                 guint                  n_params,
                                 GObjectConstructParam *params)
{
  GimpContainerTreeView *tree_view;
  GimpItemTreeView      *item_view;
  GObject               *object;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  tree_view = GIMP_CONTAINER_TREE_VIEW (object);
  item_view = GIMP_ITEM_TREE_VIEW (object);

  tree_view->name_cell->mode = GTK_CELL_RENDERER_MODE_EDITABLE;
  GTK_CELL_RENDERER_TEXT (tree_view->name_cell)->editable = TRUE;

  g_signal_connect (tree_view->name_cell, "edited",
                    G_CALLBACK (gimp_item_tree_view_name_edited),
                    item_view);

  return object;
}

static void
gimp_item_tree_view_destroy (GtkObject *object)
{
  GimpItemTreeView *view;

  view = GIMP_ITEM_TREE_VIEW (object);

  if (view->gimage)
    gimp_item_tree_view_set_image (view, NULL);

  if (view->signal_name)
    {
      g_free (view->signal_name);
      view->signal_name = NULL;
    }

  if (view->item_factory)
    {
      g_object_unref (view->item_factory);
      view->item_factory = NULL;
    }

  GTK_OBJECT_CLASS (parent_class)->destroy (object);
}

GtkWidget *
gimp_item_tree_view_new (gint                  preview_size,
                         GimpImage            *gimage,
                         GType                 item_type,
                         const gchar          *signal_name,
                         GimpNewItemFunc       new_item_func,
                         GimpEditItemFunc      edit_item_func,
                         GimpActivateItemFunc  activate_item_func,
                         GimpMenuFactory      *menu_factory,
                         const gchar          *menu_identifier)
{
  GimpItemTreeView  *tree_view;
  GimpContainerView *view;

  g_return_val_if_fail (preview_size > 0 &&
			preview_size <= GIMP_PREVIEW_MAX_SIZE, NULL);
  g_return_val_if_fail (! gimage || GIMP_IS_IMAGE (gimage), NULL);
  g_return_val_if_fail (signal_name != NULL, NULL);
  g_return_val_if_fail (new_item_func != NULL, NULL);
  g_return_val_if_fail (edit_item_func != NULL, NULL);
  g_return_val_if_fail (activate_item_func != NULL, NULL);
  g_return_val_if_fail (GIMP_IS_MENU_FACTORY (menu_factory), NULL);
  g_return_val_if_fail (menu_identifier != NULL, NULL);

  if (item_type == GIMP_TYPE_LAYER)
    {
      tree_view = g_object_new (GIMP_TYPE_LAYER_TREE_VIEW, NULL);
    }
  else if (item_type == GIMP_TYPE_CHANNEL)
    {
      tree_view = g_object_new (GIMP_TYPE_CHANNEL_TREE_VIEW, NULL);
    }
  else if (item_type == GIMP_TYPE_VECTORS)
    {
      tree_view = g_object_new (GIMP_TYPE_VECTORS_TREE_VIEW, NULL);
    }
  else
    {
      g_warning ("gimp_item_tree_view_new: unsupported item type '%s'\n",
                 g_type_name (item_type));
      return NULL;
    }

  view = GIMP_CONTAINER_VIEW (tree_view);

  view->preview_size = preview_size;
  view->reorderable  = TRUE;

  tree_view->item_type          = item_type;
  tree_view->signal_name        = g_strdup (signal_name);
  tree_view->new_item_func      = new_item_func;
  tree_view->edit_item_func     = edit_item_func;
  tree_view->activate_item_func = activate_item_func;

  tree_view->menu_factory = menu_factory;
  tree_view->item_factory = gimp_menu_factory_menu_new (menu_factory,
                                                        menu_identifier,
                                                        GTK_TYPE_MENU,
                                                        tree_view,
                                                        FALSE);

  /*  connect "drop to new" manually as it makes a difference whether
   *  it was clicked or dropped
   */
  gimp_dnd_viewable_dest_add (tree_view->new_button,
			      item_type,
			      gimp_item_tree_view_new_dropped,
			      view);

  gimp_container_view_enable_dnd (view,
				  GTK_BUTTON (tree_view->duplicate_button),
				  item_type);
  gimp_container_view_enable_dnd (view,
				  GTK_BUTTON (tree_view->edit_button),
				  item_type);
  gimp_container_view_enable_dnd (view,
				  GTK_BUTTON (tree_view->delete_button),
				  item_type);

  gimp_item_tree_view_set_image (tree_view, gimage);

  return GTK_WIDGET (tree_view);
}

void
gimp_item_tree_view_set_image (GimpItemTreeView *view,
                               GimpImage        *gimage)
{
  g_return_if_fail (GIMP_IS_ITEM_TREE_VIEW (view));
  g_return_if_fail (! gimage || GIMP_IS_IMAGE (gimage));

  g_signal_emit (view, view_signals[SET_IMAGE], 0, gimage);
}

static void
gimp_item_tree_view_real_set_image (GimpItemTreeView *view,
                                    GimpImage        *gimage)
{
  g_return_if_fail (GIMP_IS_ITEM_TREE_VIEW (view));
  g_return_if_fail (! gimage || GIMP_IS_IMAGE (gimage));

  if (view->gimage == gimage)
    return;

  if (view->gimage)
    {
      g_signal_handlers_disconnect_by_func (view->gimage,
					    gimp_item_tree_view_item_changed,
					    view);
      g_signal_handlers_disconnect_by_func (view->gimage,
					    gimp_item_tree_view_size_changed,
					    view);

      gimp_container_view_set_container (GIMP_CONTAINER_VIEW (view), NULL);
    }

  view->gimage = gimage;

  if (view->gimage)
    {
      GimpContainer *container;

      container =
        GIMP_ITEM_TREE_VIEW_GET_CLASS (view)->get_container (view->gimage);

      gimp_container_view_set_container (GIMP_CONTAINER_VIEW (view), container);

      g_signal_connect (view->gimage, view->signal_name,
			G_CALLBACK (gimp_item_tree_view_item_changed),
			view);
      g_signal_connect (view->gimage, "size_changed",
			G_CALLBACK (gimp_item_tree_view_size_changed),
			view);

      gimp_item_tree_view_item_changed (view->gimage, view);
    }

  gtk_widget_set_sensitive (view->new_button, (view->gimage != NULL));
}


/*  GimpContainerView methods  */

static void
gimp_item_tree_view_select_item (GimpContainerView *view,
                                 GimpViewable      *item,
                                 gpointer           insert_data)
{
  GimpItemTreeView *tree_view;
  gboolean          raise_sensitive     = FALSE;
  gboolean          lower_sensitive     = FALSE;
  gboolean          duplicate_sensitive = FALSE;
  gboolean          edit_sensitive      = FALSE;
  gboolean          delete_sensitive    = FALSE;

  tree_view = GIMP_ITEM_TREE_VIEW (view);

  GIMP_CONTAINER_VIEW_CLASS (parent_class)->select_item (view, item,
                                                         insert_data);

  if (item)
    {
      GimpItemTreeViewClass *item_view_class;
      GimpItem              *active_item;
      gint                   index;

      item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (tree_view);

      active_item = item_view_class->get_active_item (tree_view->gimage);

      if (active_item != (GimpItem *) item)
	{
	  item_view_class->set_active_item (tree_view->gimage,
                                            GIMP_ITEM (item));

	  gimp_image_flush (tree_view->gimage);
	}

      index = gimp_container_get_child_index (view->container,
					      GIMP_OBJECT (item));

      if (view->container->num_children > 1)
	{
	  if (index > 0)
	    raise_sensitive = TRUE;

	  if (index < (view->container->num_children - 1))
	    lower_sensitive = TRUE;
	}

      duplicate_sensitive = TRUE;
      edit_sensitive      = TRUE;
      delete_sensitive    = TRUE;
    }

  gtk_widget_set_sensitive (tree_view->raise_button,     raise_sensitive);
  gtk_widget_set_sensitive (tree_view->lower_button,     lower_sensitive);
  gtk_widget_set_sensitive (tree_view->duplicate_button, duplicate_sensitive);
  gtk_widget_set_sensitive (tree_view->edit_button,      edit_sensitive);
  gtk_widget_set_sensitive (tree_view->delete_button,    delete_sensitive);
}

static void
gimp_item_tree_view_activate_item (GimpContainerView *view,
                                   GimpViewable      *item,
                                   gpointer           insert_data)
{
  GimpItemTreeView *item_view;

  item_view = GIMP_ITEM_TREE_VIEW (view);

  if (GIMP_CONTAINER_VIEW_CLASS (parent_class)->activate_item)
    GIMP_CONTAINER_VIEW_CLASS (parent_class)->activate_item (view,
							     item,
							     insert_data);

  item_view->activate_item_func (GIMP_ITEM (item));
}

static void
gimp_item_tree_view_context_item (GimpContainerView *view,
                                  GimpViewable      *item,
                                  gpointer           insert_data)
{
  GimpItemTreeView *item_view;

  if (GIMP_CONTAINER_VIEW_CLASS (parent_class)->context_item)
    GIMP_CONTAINER_VIEW_CLASS (parent_class)->context_item (view,
							    item,
							    insert_data);

  item_view = GIMP_ITEM_TREE_VIEW (view);

  if (item_view->item_factory)
    gimp_item_factory_popup_with_data (item_view->item_factory,
                                       item_view,
                                       NULL);
}


/*  GimpItemTreeView methods  */

static void
gimp_item_tree_view_real_rename_item (GimpItemTreeView *view,
                                      GimpItem         *item,
                                      const gchar      *new_name)
{
  if (strcmp (new_name, gimp_object_get_name (GIMP_OBJECT (item))))
    {
      GimpItemTreeViewClass *item_view_class;

      item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

      gimp_image_undo_push_item_rename (view->gimage,
                                        item_view_class->rename_desc,
                                        item);

      gimp_object_set_name (GIMP_OBJECT (item), new_name);

      gimp_image_flush (view->gimage);
    }
}


/*  "New" functions  */

static void
gimp_item_tree_view_new_clicked (GtkWidget        *widget,
                                 GimpItemTreeView *view)
{
  view->new_item_func (view->gimage, NULL, TRUE);
}

static void
gimp_item_tree_view_new_dropped (GtkWidget    *widget,
                                 GimpViewable *viewable,
                                 gpointer      data)
{
  GimpItemTreeView *view;

  view = GIMP_ITEM_TREE_VIEW (data);

  if (viewable && gimp_container_have (GIMP_CONTAINER_VIEW (view)->container,
				       GIMP_OBJECT (viewable)))
    {
      view->new_item_func (view->gimage, GIMP_ITEM (viewable), FALSE);

      gimp_image_flush (view->gimage);
    }
}


/*  "Duplicate" functions  */

static void
gimp_item_tree_view_duplicate_clicked (GtkWidget        *widget,
                                       GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      GimpItem *new_item;

      new_item = gimp_item_duplicate (item,
                                      G_TYPE_FROM_INSTANCE (item),
                                      TRUE);

      if (new_item)
        {
          item_view_class->add_item (view->gimage, new_item, -1);

          gimp_image_flush (view->gimage);
        }
    }
}


/*  "Raise/Lower" functions  */

static void
gimp_item_tree_view_raise_clicked (GtkWidget        *widget,
                                   GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      GimpContainer *container;
      gint           index;

      container = GIMP_CONTAINER_VIEW (view)->container;

      index = gimp_container_get_child_index (container, GIMP_OBJECT (item));

      if (index > 0)
        {
          item_view_class->reorder_item (view->gimage, item, index - 1, TRUE,
                                         item_view_class->raise_desc);

          gimp_image_flush (view->gimage);
        }
    }
}

static void
gimp_item_tree_view_raise_extended_clicked (GtkWidget        *widget,
                                            guint             state,
                                            GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      GimpContainer *container;
      gint           index;

      container = GIMP_CONTAINER_VIEW (view)->container;

      index = gimp_container_get_child_index (container, GIMP_OBJECT (item));

      if ((state & GDK_SHIFT_MASK) && (index > 0))
        {
          item_view_class->reorder_item (view->gimage, item, 0, TRUE,
                                         item_view_class->raise_to_top_desc);

          gimp_image_flush (view->gimage);
        }
    }
}

static void
gimp_item_tree_view_lower_clicked (GtkWidget        *widget,
                                   GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      GimpContainer *container;
      gint           index;

      container = GIMP_CONTAINER_VIEW (view)->container;

      index = gimp_container_get_child_index (container, GIMP_OBJECT (item));

      if (index < container->num_children - 1)
        {
          item_view_class->reorder_item (view->gimage, item, index + 1, TRUE,
                                         item_view_class->lower_desc);

          gimp_image_flush (view->gimage);
        }
    }
}

static void
gimp_item_tree_view_lower_extended_clicked (GtkWidget        *widget,
                                            guint             state,
                                            GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      GimpContainer *container;
      gint           index;

      container = GIMP_CONTAINER_VIEW (view)->container;

      index = gimp_container_get_child_index (container, GIMP_OBJECT (item));

      if ((state & GDK_SHIFT_MASK) && (index < container->num_children - 1))
        {
          item_view_class->reorder_item (view->gimage, item,
                                         container->num_children - 1, TRUE,
                                         item_view_class->lower_to_bottom_desc);

          gimp_image_flush (view->gimage);
        }
    }
}


/*  "Edit" functions  */

static void
gimp_item_tree_view_edit_clicked (GtkWidget        *widget,
                                  GimpItemTreeView *view)
{
  GimpItem *item;

  item = GIMP_ITEM_TREE_VIEW_GET_CLASS (view)->get_active_item (view->gimage);

  if (item)
    view->edit_item_func (item);
}


/*  "Delete" functions  */

static void
gimp_item_tree_view_delete_clicked (GtkWidget        *widget,
                                    GimpItemTreeView *view)
{
  GimpItemTreeViewClass *item_view_class;
  GimpItem              *item;

  item_view_class = GIMP_ITEM_TREE_VIEW_GET_CLASS (view);

  item = item_view_class->get_active_item (view->gimage);

  if (item)
    {
      item_view_class->remove_item (view->gimage, item);

      gimp_image_flush (view->gimage);
    }
}


/*  GimpImage callbacks  */

static void
gimp_item_tree_view_item_changed (GimpImage        *gimage,
                                  GimpItemTreeView *view)
{
  GimpItem *item;

  item = GIMP_ITEM_TREE_VIEW_GET_CLASS (view)->get_active_item (view->gimage);

  gimp_container_view_select_item (GIMP_CONTAINER_VIEW (view),
                                   (GimpViewable *) item);
}

static void
gimp_item_tree_view_size_changed (GimpImage        *gimage,
                                  GimpItemTreeView *view)
{
  gint preview_size;

  preview_size = GIMP_CONTAINER_VIEW (view)->preview_size;

  gimp_container_view_set_preview_size (GIMP_CONTAINER_VIEW (view),
					preview_size);
}

static void
gimp_item_tree_view_name_edited (GtkCellRendererText *cell,
                                 const gchar         *path_str,
                                 const gchar         *new_text,
                                 GimpItemTreeView    *view)
{
  GimpContainerTreeView *tree_view;
  GtkTreePath           *path;
  GtkTreeIter            iter;

  tree_view = GIMP_CONTAINER_TREE_VIEW (view);

  path = gtk_tree_path_new_from_string (path_str);

  if (gtk_tree_model_get_iter (tree_view->model, &iter, path))
    {
      GimpPreviewRenderer *renderer;
      GimpItem            *item;

      gtk_tree_model_get (tree_view->model, &iter,
                          tree_view->model_column_renderer, &renderer,
                          -1);

      item = GIMP_ITEM (renderer->viewable);

      GIMP_ITEM_TREE_VIEW_GET_CLASS (view)->rename_item (view, item,
                                                         new_text);

      g_object_unref (renderer);
    }

  gtk_tree_path_free (path);
}
