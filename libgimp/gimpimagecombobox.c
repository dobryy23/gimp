/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpimagecombobox.c
 * Copyright (C) 2004 Sven Neumann <sven@gimp.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "gimp.h"

#include "gimpimagecombobox.h"
#include "gimppixbuf.h"

#include "libgimp-intl.h"


#define MENU_THUMBNAIL_SIZE  24


static void  gimp_image_combo_box_model_add      (GtkListStore            *store,
                                                  gint                     num_images,
                                                  gint32                  *images,
                                                  GimpImageConstraintFunc  constraint,
                                                  gpointer                 data);
static void  gimp_image_combo_box_model_add_none (GtkListStore            *store);


/**
 * gimp_image_combo_box_new:
 * @constraint: a #GimpImageConstraintFunc or %NULL
 * @data:       a pointer that is passed to @constraint
 *
 * Creates a new #GimpIntComboBox filled with all currently opened
 * images. If a @constraint function is specified, it is called for
 * each image and only if the function returns %TRUE, the image is
 * added to the combobox.
 *
 * Use gimp_int_combo_box_set_active() to get the active image ID and
 * gimp_int_combo_box_get_active() to retrieve the ID of the selected
 * image.
 *
 * Return value: a new #GimpIntComboBox.
 *
 * Since: GIMP 2.2
 **/
GtkWidget *
gimp_image_combo_box_new (GimpImageConstraintFunc constraint,
                          gpointer                data)
{
  GtkWidget    *combo_box;
  GtkTreeModel *model;
  GtkTreeIter   iter;
  gint32       *images;
  gint          num_images;

  combo_box = g_object_new (GIMP_TYPE_INT_COMBO_BOX, NULL);

  model = gtk_combo_box_get_model (GTK_COMBO_BOX (combo_box));

  images = gimp_image_list (&num_images);

  if (num_images > 0)
    gimp_image_combo_box_model_add (GTK_LIST_STORE (model),
                                    num_images, images,
                                    constraint, data);
  else
    gimp_image_combo_box_model_add_none (GTK_LIST_STORE (model));

  g_free (images);

  if (gtk_tree_model_get_iter_first (model, &iter))
    gtk_combo_box_set_active_iter (GTK_COMBO_BOX (combo_box), &iter);

  return combo_box;
}

static void
gimp_image_combo_box_model_add (GtkListStore            *store,
                                gint                     num_images,
                                gint32                  *images,
                                GimpImageConstraintFunc  constraint,
                                gpointer                 data)
{
  GtkTreeIter  iter;
  gint         i;

  for (i = 0; i < num_images; i++)
    {
      if (! constraint || (* constraint) (images[i], data))
        {
          gchar     *image_name = gimp_image_get_name (images[i]);
          gchar     *label;
          GdkPixbuf *thumb;

          label = g_strdup_printf ("%s-%d", image_name, images[i]);

          g_free (image_name);

          thumb = gimp_image_get_thumbnail (images[i],
                                            MENU_THUMBNAIL_SIZE,
                                            MENU_THUMBNAIL_SIZE,
                                            GIMP_PIXBUF_SMALL_CHECKS);

          gtk_list_store_append (store, &iter);
          gtk_list_store_set (store, &iter,
                              GIMP_INT_STORE_VALUE,  images[i],
                              GIMP_INT_STORE_LABEL,  label,
                              GIMP_INT_STORE_PIXBUF, thumb,
                              -1);

          if (thumb)
            g_object_unref (thumb);

          g_free (label);
        }
    }
}

static void
gimp_image_combo_box_model_add_none (GtkListStore *store)
{
  GtkTreeIter  iter;

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter,
                      GIMP_INT_STORE_VALUE, -1,
                      GIMP_INT_STORE_LABEL, _("(None)"),
                      -1);
}
