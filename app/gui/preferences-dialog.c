/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1997 Spencer Kimball and Peter Mattis
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
#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "gui-types.h"

#include "config/gimpconfig.h"
#include "config/gimpconfig-params.h"
#include "config/gimpconfig-utils.h"
#include "config/gimprc.h"

#include "core/gimp.h"

#include "widgets/gimpcolorpanel.h"
#include "widgets/gimpdeviceinfo.h"
#include "widgets/gimpdevices.h"
#include "widgets/gimpdialogfactory.h"
#include "widgets/gimppropwidgets.h"

#include "gui.h"
#include "resolution-calibrate-dialog.h"
#include "session.h"

#include "gimp-intl.h"


#define MAX_COMMENT_LENGTH 512  /* arbitrary */


/*  preferences local functions  */

static GtkWidget * prefs_dialog_new               (Gimp       *gimp,
                                                   GObject    *config);
static void        prefs_config_notify            (GObject    *config,
                                                   GParamSpec *param_spec,
                                                   GObject    *config_copy);
static void        prefs_config_copy_notify       (GObject    *config_copy,
                                                   GParamSpec *param_spec,
                                                   GObject    *config);
static void        prefs_cancel_callback          (GtkWidget  *widget,
                                                   GtkWidget  *dialog);
static void        prefs_ok_callback              (GtkWidget  *widget,
                                                   GtkWidget  *dialog);

static void   prefs_default_resolution_callback   (GtkWidget  *widget,
                                                   GtkWidget  *size_sizeentry);
static void   prefs_res_source_callback           (GtkWidget  *widget,
                                                   GObject    *config);
static void   prefs_resolution_calibrate_callback (GtkWidget  *widget,
                                                   GtkWidget  *sizeentry);
static void   prefs_input_dialog_able_callback    (GtkWidget  *widget,
                                                   GdkDevice  *device,
                                                   gpointer    data);


/*  private variables  */

static GtkWidget *prefs_dialog = NULL;


/*  public function  */

GtkWidget *
preferences_dialog_create (Gimp *gimp)
{
  GObject *config;
  GObject *config_copy;
  GObject *config_orig;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  if (prefs_dialog)
    return prefs_dialog;

  /*  turn of autosaving while the prefs dialog is open  */
  gimp_rc_set_autosave (GIMP_RC (gimp->edit_config), FALSE);

  config       = G_OBJECT (gimp->edit_config);
  config_copy  = gimp_config_duplicate (config);
  config_orig  = gimp_config_duplicate (config);

  g_signal_connect_object (config, "notify",
                           G_CALLBACK (prefs_config_notify),
                           config_copy, 0);
  g_signal_connect_object (config_copy, "notify",
                           G_CALLBACK (prefs_config_copy_notify),
                           config, 0);

  prefs_dialog = prefs_dialog_new (gimp, config_copy);

  g_object_add_weak_pointer (G_OBJECT (prefs_dialog),
                             (gpointer *) &prefs_dialog);

  g_object_set_data (G_OBJECT (prefs_dialog), "gimp", gimp);

  g_object_set_data_full (G_OBJECT (prefs_dialog), "config-copy", config_copy,
                          (GDestroyNotify) g_object_unref);
  g_object_set_data_full (G_OBJECT (prefs_dialog), "config-orig", config_orig,
                          (GDestroyNotify) g_object_unref);

  return prefs_dialog;
}


/*  private functions  */

static void
prefs_config_notify (GObject    *config,
                     GParamSpec *param_spec,
                     GObject    *config_copy)
{
  GValue global_value = { 0, };
  GValue copy_value   = { 0, };

  g_value_init (&global_value, param_spec->value_type);
  g_value_init (&copy_value,   param_spec->value_type);

  g_object_get_property (config,      param_spec->name, &global_value);
  g_object_get_property (config_copy, param_spec->name, &copy_value);

  if (g_param_values_cmp (param_spec, &global_value, &copy_value))
    {
      g_signal_handlers_block_by_func (config_copy,
                                       prefs_config_copy_notify,
                                       config);

      g_object_set_property (config_copy, param_spec->name, &global_value);

      g_signal_handlers_unblock_by_func (config_copy,
                                         prefs_config_copy_notify,
                                         config);
    }

  g_value_unset (&global_value);
  g_value_unset (&copy_value);
}

static void
prefs_config_copy_notify (GObject    *config_copy,
                          GParamSpec *param_spec,
                          GObject    *config)
{
  GValue copy_value   = { 0, };
  GValue global_value = { 0, };

  g_value_init (&copy_value,   param_spec->value_type);
  g_value_init (&global_value, param_spec->value_type);

  g_object_get_property (config_copy, param_spec->name, &copy_value);
  g_object_get_property (config,      param_spec->name, &global_value);

  if (g_param_values_cmp (param_spec, &copy_value, &global_value))
    {
      if (param_spec->flags & GIMP_PARAM_CONFIRM)
        {
#ifdef GIMP_CONFIG_DEBUG
          g_print ("NOT Applying prefs change of '%s' to edit_config "
                   "because it needs confirmation\n",
                   param_spec->name);
#endif
        }
      else
        {
#ifdef GIMP_CONFIG_DEBUG
          g_print ("Applying prefs change of '%s' to edit_config\n",
                   param_spec->name);
#endif
          g_signal_handlers_block_by_func (config,
                                           prefs_config_notify,
                                           config_copy);

          g_object_set_property (config, param_spec->name, &copy_value);

          g_signal_handlers_unblock_by_func (config,
                                             prefs_config_notify,
                                             config_copy);
        }
    }

  g_value_unset (&copy_value);
  g_value_unset (&global_value);
}

static void
prefs_cancel_callback (GtkWidget *widget,
		       GtkWidget *dialog)
{
  Gimp    *gimp;
  GObject *config_orig;
  GList   *diff;
  GList   *list;

  gimp        = g_object_get_data (G_OBJECT (dialog), "gimp");
  config_orig = g_object_get_data (G_OBJECT (dialog), "config-orig");

  g_object_ref (config_orig);

  gtk_widget_destroy (dialog);  /*  destroys config_copy  */

  diff = gimp_config_diff (G_OBJECT (gimp->edit_config), config_orig,
                           GIMP_PARAM_SERIALIZE);

  g_object_freeze_notify (G_OBJECT (gimp->edit_config));

  for (list = diff; list; list = g_list_next (list))
    {
      GParamSpec *param_spec;
      GValue      value = { 0, };

      param_spec = (GParamSpec *) list->data;

      g_value_init (&value, param_spec->value_type);

      g_object_get_property (config_orig,
                             param_spec->name,
                             &value);
      g_object_set_property (G_OBJECT (gimp->edit_config),
                             param_spec->name,
                             &value);

      g_value_unset (&value);
    }

  g_object_thaw_notify (G_OBJECT (gimp->edit_config));

  g_list_free (diff);
  g_object_unref (config_orig);

  /*  enable autosaving again  */
  gimp_rc_set_autosave (GIMP_RC (gimp->edit_config), TRUE);
}

static void
prefs_ok_callback (GtkWidget *widget,
		   GtkWidget *dialog)
{
  Gimp    *gimp;
  GObject *config_copy;
  GList   *restart_diff;
  GList   *confirm_diff;
  GList   *list;

  gimp        = g_object_get_data (G_OBJECT (dialog), "gimp");
  config_copy = g_object_get_data (G_OBJECT (dialog), "config-copy");

  g_object_ref (config_copy);

  gtk_widget_destroy (dialog);  /*  destroys config_orig  */

  confirm_diff = gimp_config_diff (G_OBJECT (gimp->edit_config), config_copy,
                                   GIMP_PARAM_CONFIRM);

  g_object_freeze_notify (G_OBJECT (gimp->edit_config));

  for (list = confirm_diff; list; list = g_list_next (list))
    {
      GParamSpec *param_spec;
      GValue      value = { 0, };

      param_spec = (GParamSpec *) list->data;

      g_value_init (&value, param_spec->value_type);

      g_object_get_property (config_copy,
                             param_spec->name,
                             &value);
      g_object_set_property (G_OBJECT (gimp->edit_config),
                             param_spec->name,
                             &value);

      g_value_unset (&value);
    }

  g_object_thaw_notify (G_OBJECT (gimp->edit_config));

  g_list_free (confirm_diff);
  g_object_unref (config_copy);

  gimp_rc_save (GIMP_RC (gimp->edit_config));

  /*  enable autosaving again  */
  gimp_rc_set_autosave (GIMP_RC (gimp->edit_config), TRUE);

  /*  spit out a solely informational warning about changed values
   *  which need restart
   */
  restart_diff = gimp_config_diff (G_OBJECT (gimp->edit_config),
                                   G_OBJECT (gimp->config),
                                   GIMP_PARAM_RESTART);

  if (restart_diff)
    {
      GString *string;

      string = g_string_new (_("You will have to restart GIMP for\n"
                               "the following changes to take effect:"));
      g_string_append (string, "\n\n");

      for (list = restart_diff; list; list = g_list_next (list))
        {
          GParamSpec *param_spec;

          param_spec = (GParamSpec *) list->data;

          g_string_append_printf (string, "%s\n", param_spec->name);
        }

      g_message (string->str);

      g_string_free (string, TRUE);
    }

  g_list_free (restart_diff);
}

static void
prefs_default_resolution_callback (GtkWidget *widget,
				   GtkWidget *size_sizeentry)
{
  gdouble xres;
  gdouble yres;

  xres = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (widget), 0);
  yres = gimp_size_entry_get_refval (GIMP_SIZE_ENTRY (widget), 1);

  gimp_size_entry_set_resolution (GIMP_SIZE_ENTRY (size_sizeentry), 0,
				  xres, FALSE);
  gimp_size_entry_set_resolution (GIMP_SIZE_ENTRY (size_sizeentry), 1,
				  yres, FALSE);
}

static void
prefs_res_source_callback (GtkWidget *widget,
			   GObject   *config)
{
  gdouble  xres;
  gdouble  yres;
  gboolean from_gdk;

  gimp_toggle_button_sensitive_update (GTK_TOGGLE_BUTTON (widget));

  from_gdk = GTK_TOGGLE_BUTTON (widget)->active;

  if (from_gdk)
    {
      gui_get_screen_resolution (&xres, &yres);
    }
  else
    {
      GimpSizeEntry *sizeentry;

      sizeentry = g_object_get_data (G_OBJECT (widget),
                                     "monitor_resolution_sizeentry");

      if (sizeentry)
	{
	  xres = gimp_size_entry_get_refval (sizeentry, 0);
	  yres = gimp_size_entry_get_refval (sizeentry, 1);
	}
    }

  g_object_set (config,
                "monitor-xresolution",                      xres,
                "monitor-yresolution",                      yres,
                "monitor-resolution-from-windowing-system", from_gdk,
                NULL);
}

static void
prefs_resolution_calibrate_callback (GtkWidget *widget,
				     GtkWidget *sizeentry)
{
  GtkWidget *dialog;
  GtkWidget *notebook;
  GtkWidget *image;

  dialog = gtk_widget_get_toplevel (sizeentry);

  notebook = g_object_get_data (G_OBJECT (dialog),   "notebook");
  image    = g_object_get_data (G_OBJECT (notebook), "image");

  resolution_calibrate_dialog (sizeentry,
                               gtk_image_get_pixbuf (GTK_IMAGE (image)),
                               NULL, NULL, NULL);
}

static void
prefs_input_dialog_able_callback (GtkWidget *widget,
                                  GdkDevice *device,
                                  gpointer   data)
{
  gimp_device_info_changed_by_device (device);
}

static GtkWidget *
prefs_notebook_append_page (Gimp          *gimp,
                            GtkNotebook   *notebook,
			    const gchar   *notebook_label,
                            const gchar   *notebook_icon,
			    GtkTreeStore  *tree,
			    const gchar   *tree_label,
			    const gchar   *help_data,
			    GtkTreeIter   *parent,
			    GtkTreeIter   *iter,
			    gint           page_index)
{
  GtkWidget   *event_box;
  GtkWidget   *vbox;
  GdkPixbuf   *pixbuf       = NULL;
  GdkPixbuf   *small_pixbuf = NULL;

  event_box = gtk_event_box_new ();
  gtk_notebook_append_page (notebook, event_box, NULL);
  gtk_widget_show (event_box);

  gimp_help_set_help_data (event_box, NULL, help_data);

  vbox = gtk_vbox_new (FALSE, 4);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);
  gtk_container_add (GTK_CONTAINER (event_box), vbox);
  gtk_widget_show (vbox);

  if (notebook_icon)
    {
      gchar *filename;

      filename = g_build_filename (gui_themes_get_theme_dir (gimp),
                                   "images",
                                   "preferences",
                                   notebook_icon,
                                   NULL);

      if (g_file_test (filename, G_FILE_TEST_IS_REGULAR))
        pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      else
        pixbuf = NULL;

      g_free (filename);

      if (pixbuf)
        {
          small_pixbuf = gdk_pixbuf_scale_simple (pixbuf,
                                                  18, 18,
                                                  GDK_INTERP_BILINEAR);
        }
    }

  gtk_tree_store_append (tree, iter, parent);
  gtk_tree_store_set (tree, iter,
                      0, small_pixbuf,
                      1, tree_label,
                      2, page_index,
                      3, notebook_label,
                      4, pixbuf,
                      -1);

  if (pixbuf)
    g_object_unref (pixbuf);

  if (small_pixbuf)
    g_object_unref (small_pixbuf);

  return vbox;
}

static void
prefs_tree_select_callback (GtkTreeSelection *sel,
			    GtkNotebook      *notebook)
{
  GtkWidget    *label;
  GtkWidget    *image;
  GtkTreeModel *model;
  GtkTreeIter   iter;
  GValue        val = { 0, };

  if (! gtk_tree_selection_get_selected (sel, &model, &iter))
    return;

  label = g_object_get_data (G_OBJECT (notebook), "label");
  image = g_object_get_data (G_OBJECT (notebook), "image");

  gtk_tree_model_get_value (model, &iter, 3, &val);

  gtk_label_set_text (GTK_LABEL (label),
                      g_value_get_string (&val));

  g_value_unset (&val);

  gtk_tree_model_get_value (model, &iter, 4, &val);

  gtk_image_set_from_pixbuf (GTK_IMAGE (image),
                             g_value_get_object (&val));

  g_value_unset (&val);

  gtk_tree_model_get_value (model, &iter, 2, &val);

  gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook),
				 g_value_get_int (&val));

  g_value_unset (&val);
}

static void
prefs_format_string_select_callback (GtkTreeSelection *sel,
                                     GtkEntry         *entry)
{
  GtkTreeModel *model;
  GtkTreeIter   iter;
  GValue        val = { 0, };

  if (! gtk_tree_selection_get_selected (sel, &model, &iter))
    return;

  gtk_tree_model_get_value (model, &iter, 1, &val);

  gtk_entry_set_text (entry, g_value_get_string (&val));

  g_value_unset (&val);
}

static GtkWidget *
prefs_frame_new (gchar        *label,
		 GtkContainer *parent,
                 gboolean      expand)
{
  GtkWidget *frame;
  GtkWidget *vbox;

  frame = gtk_frame_new (label);

  if (FALSE)
    {
      PangoAttrList  *attrs;
      PangoAttribute *attr;

      gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);

      attrs = pango_attr_list_new ();

      attr = pango_attr_scale_new (PANGO_SCALE_LARGE);
      attr->start_index = 0;
      attr->end_index   = -1;
      pango_attr_list_insert (attrs, attr);

      attr = pango_attr_weight_new (PANGO_WEIGHT_BOLD);
      attr->start_index = 0;
      attr->end_index   = -1;
      pango_attr_list_insert (attrs, attr);

      gtk_label_set_attributes (GTK_LABEL (gtk_frame_get_label_widget (GTK_FRAME (frame))), attrs);
      pango_attr_list_unref (attrs);
    }

  if (GTK_IS_BOX (parent))
    gtk_box_pack_start (GTK_BOX (parent), frame, expand, expand, 0);
  else
    gtk_container_add (parent, frame);

  gtk_widget_show (frame);

  vbox = gtk_vbox_new (FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 2);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  return vbox;
}

static GtkWidget *
prefs_table_new (gint          rows,
                 GtkContainer *parent,
                 gboolean      left_align)
{
  GtkWidget *table;

  if (left_align)
    {
      GtkWidget *hbox;

      hbox = gtk_hbox_new (FALSE, 0);

      if (GTK_IS_BOX (parent))
        gtk_box_pack_start (GTK_BOX (parent), hbox, FALSE, FALSE, 0);
      else
        gtk_container_add (parent, hbox);

      gtk_widget_show (hbox);

      parent = GTK_CONTAINER (hbox);
    }

  table = gtk_table_new (rows, 2, FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (table), 2);

  if (rows > 1)
    gtk_table_set_row_spacings (GTK_TABLE (table), 2);

  gtk_table_set_col_spacings (GTK_TABLE (table), 4);

  if (GTK_IS_BOX (parent))
    gtk_box_pack_start (GTK_BOX (parent), table, FALSE, FALSE, 0);
  else
    gtk_container_add (parent, table);

  gtk_widget_show (table);

  return table;
}

static GtkWidget *
prefs_check_button_add (GObject     *config,
                        const gchar *property_name,
                        const gchar *label,
                        GtkBox      *vbox)
{
  GtkWidget *button;

  button = gimp_prop_check_button_new (config, property_name, label);

  if (button)
    {
      gtk_box_pack_start (vbox, button, FALSE, FALSE, 0);
      gtk_widget_show (button);
    }

  return button;
}

static GtkWidget *
prefs_color_button_add (GObject     *config,
                        const gchar *property_name,
                        const gchar *label,
                        const gchar *title,
                        GtkTable    *table,
                        gint         table_row)
{
  GtkWidget *button;

  button = gimp_prop_color_button_new (config, property_name, title,
                                       20, 20, GIMP_COLOR_AREA_FLAT);

  if (button)
    gimp_table_attach_aligned (table, 0, table_row,
                               label, 1.0, 0.5,
                               button, 1, TRUE);

  return button;
}

static GtkWidget *
prefs_enum_option_menu_add (GObject     *config,
                            const gchar *property_name,
                            gint         minimum,
                            gint         maximum,
                            const gchar *label,
                            GtkTable    *table,
                            gint         table_row)
{
  GtkWidget *menu;

  menu = gimp_prop_enum_option_menu_new (config, property_name,
                                         minimum, maximum);

  if (menu)
    gimp_table_attach_aligned (table, 0, table_row,
                               label, 1.0, 0.5,
                               menu, 1, TRUE);

  return menu;
}

static GtkWidget *
prefs_boolean_option_menu_add (GObject     *config,
                               const gchar *property_name,
                               const gchar *true_text,
                               const gchar *false_text,
                               const gchar *label,
                               GtkTable    *table,
                               gint         table_row)
{
  GtkWidget *menu;

  menu = gimp_prop_boolean_option_menu_new (config, property_name,
                                            true_text, false_text);

  if (menu)
    gimp_table_attach_aligned (table, 0, table_row,
                               label, 1.0, 0.5,
                               menu, 1, TRUE);

  return menu;
}

static GtkWidget *
prefs_spin_button_add (GObject     *config,
                       const gchar *property_name,
                       gdouble      step_increment,
                       gdouble      page_increment,
                       gint         digits,
                       const gchar *label,
                       GtkTable    *table,
                       gint         table_row)
{
  GtkWidget  *spinbutton;

  spinbutton = gimp_prop_spin_button_new (config, property_name,
                                          step_increment, page_increment,
                                          digits);

  if (spinbutton)
    gimp_table_attach_aligned (table, 0, table_row,
                               label, 1.0, 0.5,
                               spinbutton, 1, TRUE);

  return spinbutton;
}

static GtkWidget *
prefs_memsize_entry_add (GObject     *config,
                         const gchar *property_name,
                         const gchar *label,
                         GtkTable    *table,
                         gint         table_row)
{
  GtkWidget *entry;

  entry = gimp_prop_memsize_entry_new (config, property_name);

  if (entry)
    gimp_table_attach_aligned (table, 0, table_row,
                               label, 1.0, 0.5,
                               entry, 1, TRUE);

  return entry;
}

static void
prefs_help_func (const gchar *help_data)
{
  if (prefs_dialog)
    {
      GtkWidget *notebook;
      GtkWidget *event_box;
      gint       page_num;

      notebook  = g_object_get_data (G_OBJECT (prefs_dialog), "notebook");
      page_num  = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook));
      event_box = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), page_num);

      help_data = g_object_get_data (G_OBJECT (event_box), "gimp_help_data");
      gimp_standard_help_func (help_data);
    }
}

static GtkWidget *
prefs_dialog_new (Gimp    *gimp,
                  GObject *config)
{
  GtkWidget         *dialog;
  GtkWidget         *tv;
  GtkTreeStore      *tree;
  GtkTreeViewColumn *column;
  GtkCellRenderer   *cell;
  GtkTreeSelection  *sel;
  GtkTreeIter        top_iter;
  GtkTreeIter        child_iter;
  GtkTreeIter        grandchild_iter;
  gint               page_index;

  GtkWidget         *ebox;
  GtkWidget         *frame;
  GtkWidget         *notebook;
  GtkWidget         *vbox;
  GtkWidget         *vbox2;
  GtkWidget         *hbox;
  GtkWidget         *abox;
  GtkWidget         *button;
  GtkWidget         *fileselection;
  GtkWidget         *patheditor;
  GtkWidget         *table;
  GtkWidget         *label;
  GtkWidget         *image;
  GtkWidget         *sizeentry;
  GtkWidget         *sizeentry2;
  GtkWidget         *separator;
  GtkWidget         *calibrate_button;
  GtkWidget         *scrolled_window;
  GtkWidget         *text_view;
  GtkTextBuffer     *text_buffer;
  PangoAttrList     *attrs;
  PangoAttribute    *attr;
  GSList            *group;

  gint               i;
  gchar             *pixels_per_unit;

  GimpCoreConfig    *core_config;
  GimpDisplayConfig *display_config;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (G_IS_OBJECT (config), NULL);

  core_config    = GIMP_CORE_CONFIG (config);
  display_config = GIMP_DISPLAY_CONFIG (config);

  dialog = gimp_dialog_new (_("Preferences"), "preferences",
                            prefs_help_func,
                            "dialogs/preferences/preferences.html",
                            GTK_WIN_POS_NONE,
                            FALSE, TRUE, FALSE,

                            GTK_STOCK_CANCEL, prefs_cancel_callback,
                            NULL, NULL, NULL, FALSE, TRUE,

                            GTK_STOCK_OK, prefs_ok_callback,
                            NULL, NULL, NULL, TRUE, FALSE,

                            NULL);

  /* The main hbox */
  hbox = gtk_hbox_new (FALSE, 6);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 6);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), hbox);
  gtk_widget_show (hbox);

  /* The categories tree */
  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, FALSE, 0);
  gtk_widget_show (frame);

  tree = gtk_tree_store_new (5,
                             GDK_TYPE_PIXBUF, G_TYPE_STRING,
                             G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF);
  tv = gtk_tree_view_new_with_model (GTK_TREE_MODEL (tree));
  g_object_unref (tree);

  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tv), FALSE);

  column = gtk_tree_view_column_new ();

  cell = gtk_cell_renderer_pixbuf_new ();
  gtk_tree_view_column_pack_start (column, cell, FALSE);
  gtk_tree_view_column_set_attributes (column, cell, "pixbuf", 0, NULL);

  cell = gtk_cell_renderer_text_new ();
  gtk_tree_view_column_pack_start (column, cell, TRUE);
  gtk_tree_view_column_set_attributes (column, cell, "text", 1, NULL);

  gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column);

  gtk_container_add (GTK_CONTAINER (frame), tv);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  vbox = gtk_vbox_new (FALSE, 4);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  gtk_widget_show (vbox);

  ebox = gtk_event_box_new ();
  gtk_widget_set_state (ebox, GTK_STATE_PRELIGHT);
  gtk_box_pack_start (GTK_BOX (vbox), ebox, FALSE, TRUE, 0);
  gtk_widget_show (ebox);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
  gtk_container_add (GTK_CONTAINER (ebox), frame);
  gtk_widget_show (frame);

  hbox = gtk_hbox_new (FALSE, 4);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 4);
  gtk_container_add (GTK_CONTAINER (frame), hbox);
  gtk_widget_show (hbox);

  label = gtk_label_new (NULL);
  gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  attrs = pango_attr_list_new ();

  attr = pango_attr_scale_new (PANGO_SCALE_X_LARGE);
  attr->start_index = 0;
  attr->end_index   = -1;
  pango_attr_list_insert (attrs, attr);

  attr = pango_attr_weight_new (PANGO_WEIGHT_BOLD);
  attr->start_index = 0;
  attr->end_index   = -1;
  pango_attr_list_insert (attrs, attr);

  gtk_label_set_attributes (GTK_LABEL (label), attrs);
  pango_attr_list_unref (attrs);

  image = gtk_image_new ();
  gtk_box_pack_end (GTK_BOX (hbox), image, FALSE, FALSE, 0);
  gtk_widget_show (image);

  /* The main preferences notebook */
  notebook = gtk_notebook_new ();
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook), FALSE);
  gtk_box_pack_start (GTK_BOX (vbox), notebook, TRUE, TRUE, 0);

  g_object_set_data (G_OBJECT (dialog), "notebook", notebook);

  g_object_set_data (G_OBJECT (notebook), "label", label);
  g_object_set_data (G_OBJECT (notebook), "image", image);

  sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (tv));
  g_signal_connect (sel, "changed",
		    G_CALLBACK (prefs_tree_select_callback),
		    notebook);

  page_index = 0;


  /***************/
  /*  New Image  */
  /***************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("New Image"),
                                     "new-image.png",
				     GTK_TREE_STORE (tree),
				     _("New Image"),
				     "dialogs/preferences/new_file.html",
				     NULL,
				     &top_iter,
				     page_index++);

  /* select this page in the tree */
  gtk_tree_selection_select_iter (sel, &top_iter);

  /*  Default Image Size and Unit  */
  vbox2 = prefs_frame_new (_("Default Image Size and Unit"),
                           GTK_CONTAINER (vbox), FALSE);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (vbox2), hbox);
  gtk_widget_show (hbox);

  sizeentry = gimp_prop_coordinates_new (config,
                                         "default-image-width",
                                         "default-image-height",
                                         "default-unit",
                                         "%p", GIMP_SIZE_ENTRY_UPDATE_SIZE,
                                         core_config->default_xresolution,
                                         core_config->default_yresolution,
                                         FALSE);

  gtk_table_set_col_spacings (GTK_TABLE (sizeentry), 2);
  gtk_table_set_row_spacings (GTK_TABLE (sizeentry), 2);

  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("Width"), 0, 1, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("Height"), 0, 2, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("Pixels"), 1, 4, 0.0);

  gtk_box_pack_start (GTK_BOX (hbox), sizeentry, FALSE, FALSE, 0);
  gtk_widget_show (sizeentry);

  /*  Default Image Resolution and Resolution Unit  */
  vbox2 = prefs_frame_new (_("Default Image Resolution and Resolution Unit"),
                           GTK_CONTAINER (vbox), FALSE);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (vbox2), hbox);
  gtk_widget_show (hbox);

  pixels_per_unit = g_strconcat (_("Pixels"), "/%s", NULL);

  sizeentry2 = gimp_prop_coordinates_new (config,
                                          "default-xresolution",
                                          "default-yresolution",
                                          "default-resolution_unit",
                                          pixels_per_unit,
                                          GIMP_SIZE_ENTRY_UPDATE_RESOLUTION,
                                          0.0, 0.0,
                                          TRUE);

  gtk_table_set_col_spacings (GTK_TABLE (sizeentry2), 2);
  gtk_table_set_row_spacings (GTK_TABLE (sizeentry2), 2);

  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry2),
				_("Horizontal"), 0, 1, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry2),
				_("Vertical"), 0, 2, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry2),
				_("dpi"), 1, 4, 0.0);

  g_signal_connect (sizeentry2, "value_changed",
                    G_CALLBACK (prefs_default_resolution_callback),
                    sizeentry);
  g_signal_connect (sizeentry2, "refval_changed",
                    G_CALLBACK (prefs_default_resolution_callback),
                    sizeentry);

  gtk_box_pack_start (GTK_BOX (hbox), sizeentry2, FALSE, FALSE, 0);
  gtk_widget_show (sizeentry2);

  table = prefs_table_new (2, GTK_CONTAINER (vbox), TRUE);

  prefs_enum_option_menu_add (config, "default-image-type",
                              GIMP_RGB, GIMP_GRAY,
                              _("Default Image _Type:"),
                              GTK_TABLE (table), 0);
  prefs_memsize_entry_add (config, "max-new-image-size",
                           _("Maximum Image Size:"),
                           GTK_TABLE (table), 1);


  /*********************************/
  /*  New Image / Default Comment  */
  /*********************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Default Comment"),
                                     "default-comment.png",
				     GTK_TREE_STORE (tree),
				     _("Default Comment"),
				     "dialogs/preferences/new_file.html#default_comment",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  Comment  */
  vbox2 = prefs_frame_new (_("Comment Used for New Images"),
                           GTK_CONTAINER (vbox), TRUE);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 4);
  gtk_container_add (GTK_CONTAINER (vbox2), scrolled_window);
  gtk_widget_show (scrolled_window);

  text_buffer = gimp_prop_text_buffer_new (config, "default-comment",
                                           MAX_COMMENT_LENGTH);

  text_view = gtk_text_view_new_with_buffer (text_buffer);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD);
  gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
  gtk_widget_show (text_view);

  g_object_unref (text_buffer);


  /***************/
  /*  Interface  */
  /***************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Interface"),
                                     "interface.png",
				     GTK_TREE_STORE (tree),
				     _("Interface"),
				     "dialogs/preferences/interface.html",
				     NULL,
				     &top_iter,
				     page_index++);

  /*  General  */
  vbox2 = prefs_frame_new (_("General"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "layer-previews",
                          _("_Enable Layer & Channel Previews"),
                          GTK_BOX (vbox2));

  table = prefs_table_new (4, GTK_CONTAINER (vbox2), FALSE);

  prefs_enum_option_menu_add (config, "layer-preview-size", 0, 0,
                              _("_Layer & Channel Preview Size:"),
                              GTK_TABLE (table), 0);
  prefs_enum_option_menu_add (config, "navigation-preview-size", 0, 0,
                              _("_Navigation Preview Size:"),
                              GTK_TABLE (table), 1);
  prefs_spin_button_add (config, "last-opened-size", 1.0, 5.0, 0,
                         _("_Recent Documents List Size:"),
                         GTK_TABLE (table), 3);

  /* Dialog Bahavior */
  vbox2 = prefs_frame_new (_("Dialog Behavior"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "info-window-per-display",
                          _("_Info Window Per Display"),
                          GTK_BOX (vbox2));

  /* Menus */
  vbox2 = prefs_frame_new (_("Menus"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "tearoff-menus",
                          _("Enable _Tearoff Menus"),
                          GTK_BOX (vbox2));

  prefs_check_button_add (config, "can-change-accels",
                          _("Dynamic _Keyboard Shortcuts (does not work in GNOME2 yet)"),
                          GTK_BOX (vbox2));

  /* Window Positions */
  vbox2 = prefs_frame_new (_("Window Positions"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "save-session-info",
                          _("_Save Window Positions on Exit"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "restore-session",
                          _("R_estore Saved Window Positions on Start-up"),
                          GTK_BOX (vbox2));

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 4);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
      
  button = gtk_button_new_with_label (_("Clear Saved Window Positions Now"));
  gtk_misc_set_padding (GTK_MISC (GTK_BIN (button)->child), 2, 0);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_signal_connect (button, "clicked",
                    G_CALLBACK (gimp_dialog_factories_session_clear),
                    NULL);


  /*****************************/
  /*  Interface / Help System  */
  /*****************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Help System"),
                                     "help-system.png",
				     GTK_TREE_STORE (tree),
				     _("Help System"),
				     "dialogs/preferences/interface.html#help_system",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  General  */
  vbox2 = prefs_frame_new (_("General"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "show-tool-tips",
                          _("Show Tool _Tips"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "use-help",
                          _("Context Sensitive _Help with \"F1\""),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "show-tips",
                          _("Show Tips on _Startup"),
                          GTK_BOX (vbox2));

  /*  Help Browser  */
  vbox2 = prefs_frame_new (_("Help Browser"), GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (1, GTK_CONTAINER (vbox2), FALSE);

  prefs_enum_option_menu_add (config, "help-browser", 0, 0,
                              _("Help _Browser to Use:"),
                              GTK_TABLE (table), 0);


  /******************************/
  /*  Interface / Tool Options  */
  /******************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Tool Options"),
                                     "tool-options.png",
				     GTK_TREE_STORE (tree),
				     _("Tool Options"),
				     "dialogs/preferences/interface.html#tool_options",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  Contiguous Regions  */
  vbox2 = prefs_frame_new (_("Finding Contiguous Regions"),
                           GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (1, GTK_CONTAINER (vbox2), FALSE);

  prefs_spin_button_add (config, "default-threshold", 1.0, 5.0, 0,
                         _("Default _Threshold:"),
                         GTK_TABLE (table), 0);

  /*  Scaling  */
  vbox2 = prefs_frame_new (_("Scaling"), GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (1, GTK_CONTAINER (vbox2), TRUE);

  prefs_enum_option_menu_add (config, "interpolation-type", 0, 0,
                              _("Default _Interpolation:"),
                              GTK_TABLE (table), 0);


  /*******************************/
  /*  Interface / Input Devices  */
  /*******************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Input Devices"),
                                     "input-devices.png",
				     GTK_TREE_STORE (tree),
				     _("Input Devices"),
				     "dialogs/preferences/input-devices.html",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  Input Device Settings  */
  vbox2 = prefs_frame_new (_("Input Device Settings"),
                           GTK_CONTAINER (vbox), FALSE);

  {
    GtkWidget *input_dialog;
    GtkWidget *input_vbox;
    GList     *input_children;

    input_dialog = gtk_input_dialog_new ();

    input_children = gtk_container_get_children (GTK_CONTAINER (GTK_DIALOG (input_dialog)->vbox));

    input_vbox = GTK_WIDGET (input_children->data);

    g_list_free (input_children);

    g_object_ref (input_vbox);

    gtk_container_remove (GTK_CONTAINER (GTK_DIALOG (input_dialog)->vbox),
                          input_vbox);
    gtk_box_pack_start (GTK_BOX (vbox2), input_vbox, TRUE, TRUE, 0);

    g_object_unref (input_vbox);

    g_object_weak_ref (G_OBJECT (input_vbox),
                       (GWeakNotify) gtk_widget_destroy,
                       input_dialog);

    g_signal_connect (input_dialog, "enable_device",
                      G_CALLBACK (prefs_input_dialog_able_callback),
                      NULL);
    g_signal_connect (input_dialog, "disable_device",
                      G_CALLBACK (prefs_input_dialog_able_callback),
                      NULL);
  }

  prefs_check_button_add (config, "save-device-status",
                          _("Save Input Device Settings on Exit"),
                          GTK_BOX (vbox));

  hbox = gtk_hbox_new (FALSE, 2);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 4);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);
      
  button = gtk_button_new_with_label (_("Save Input Device Settings Now"));
  gtk_misc_set_padding (GTK_MISC (GTK_BIN (button)->child), 2, 0);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_signal_connect_swapped (button, "clicked",
                            G_CALLBACK (gimp_devices_save),
                            gimp);


  /*******************************/
  /*  Interface / Image Windows  */
  /*******************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Image Windows"),
                                     "image-windows.png",
				     GTK_TREE_STORE (tree),
				     _("Image Windows"),
				     "dialogs/preferences/interface.html#image_windows",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  Appearance  */
  vbox2 = prefs_frame_new (_("Appearance"), GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "default-dot-for-dot",
                          _("Use \"_Dot for Dot\" by default"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "show-menubar",
                          _("Show Menubar"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "show-rulers",
                          _("Show _Rulers"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "show-scrollbars",
                          _("Show Scrollbars"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "show-statusbar",
                          _("Show S_tatusbar"),
                          GTK_BOX (vbox2));

  table = prefs_table_new (2, GTK_CONTAINER (vbox2), FALSE);

  prefs_boolean_option_menu_add (config, "initial-zoom-to-fit",
                                 _("Fit to Window"),
                                 "1:1",
                                 _("Inital Zoom Ratio:"),
                                 GTK_TABLE (table), 0);
  prefs_spin_button_add (config, "marching-ants-speed", 10.0, 100.0, 0,
                         _("Marching _Ants Speed:"),
                         GTK_TABLE (table), 1);

  /*  Canvas Padding Color  */
  vbox2 = prefs_frame_new (_("Canvas Padding Color"),
                           GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (2, GTK_CONTAINER (vbox2), FALSE);

  prefs_enum_option_menu_add (config, "canvas-padding-mode", 0, 0,
                              _("Padding Mode:"),
                              GTK_TABLE (table), 0);
  button = prefs_color_button_add (config, "canvas-padding-color",
                                   _("Custom Color:"),
                                   _("Select Custom Canvas Padding Color"),
                                   GTK_TABLE (table), 1);
  gimp_color_panel_set_context (GIMP_COLOR_PANEL (button),
                                gimp_get_user_context (gimp));

  /*  Zoom & Resize Behavior  */
  vbox2 = prefs_frame_new (_("Zoom & Resize Behavior"),
                           GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "resize-windows-on-zoom",
                          _("Resize Window on _Zoom"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "resize-windows-on-resize",
                          _("Resize Window on Image _Size Change"),
                          GTK_BOX (vbox2));

  /*  Pointer Movement Feedback  */
  vbox2 = prefs_frame_new (_("Pointer Movement Feedback"),
                           GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "perfect-mouse",
                          _("Perfect-but-Slow _Pointer Tracking"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "cursor-updating",
                          _("Enable Cursor _Updating"),
                          GTK_BOX (vbox2));

  table = prefs_table_new (1, GTK_CONTAINER (vbox2), FALSE);

  prefs_enum_option_menu_add (config, "cursor-mode", 0, 0,
                              _("Cursor M_ode:"),
                              GTK_TABLE (table), 0);


  /****************************************************************/
  /*  Interface / Image Windows / Image Title & Statusbar Format  */
  /****************************************************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Image Title & Statusbar Format"),
                                     "image-title.png",
				     GTK_TREE_STORE (tree),
				     _("Title & Status"),
				     "dialogs/preferences/image_windows.html",
				     &child_iter,
				     &grandchild_iter,
				     page_index++);

  {
    const gchar *format_strings[] =
    {
      NULL,
      "%f-%p.%i (%t)",
      "%f-%p.%i (%t) %z%%",
      "%f-%p.%i (%t) %d:%s",
      "%f-%p.%i (%t) %s:%d",
      "%f-%p.%i (%t) %m"
    };

    const gchar *format_names[] =
    {
      N_("Custom"),
      N_("Standard"),
      N_("Show zoom percentage"),
      N_("Show zoom ratio"),
      N_("Show reversed zoom ratio"),
      N_("Show memory usage")
    };

    struct
    {
      gchar       *current_setting;
      const gchar *title;
      const gchar *property_name;
    }
    formats[] =
    {
      {
        display_config->image_title_format,
        N_("Image Title Format"),
        "image-title-format"
      },
      {
        display_config->image_status_format,
        N_("Image Statusbar Format"),
        "image-status-format"
      }
    };

    gint format;

    g_assert (G_N_ELEMENTS (format_strings) == G_N_ELEMENTS (format_names));

    for (format = 0; format < G_N_ELEMENTS (formats); format++)
      {
        GtkListStore      *list_store;
        GtkWidget         *view;
        GtkWidget         *scrolled_win;
        GtkWidget         *entry;
        GtkCellRenderer   *rend;
        GtkTreeViewColumn *col;
        GtkTreeIter        iter;
        GtkTreeSelection  *sel;
        gint               i;

        format_strings[0] = formats[format].current_setting;

        vbox2 = prefs_frame_new (gettext (formats[format].title),
                                 GTK_CONTAINER (vbox), TRUE);

        entry = gimp_prop_entry_new (config, formats[format].property_name, 0);
        gtk_box_pack_start (GTK_BOX (vbox2), entry, FALSE, FALSE, 0);
        gtk_widget_show (entry);

        scrolled_win = gtk_scrolled_window_new (NULL, NULL);
        gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
                                             GTK_SHADOW_IN);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                        GTK_POLICY_NEVER,
                                        GTK_POLICY_AUTOMATIC);
        gtk_box_pack_start (GTK_BOX (vbox2), scrolled_win, TRUE, TRUE, 0);
        gtk_widget_show (scrolled_win);

        list_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

        view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
        gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (view), FALSE);
        gtk_container_add (GTK_CONTAINER (scrolled_win), view);
        gtk_widget_show (view);

        g_object_unref (list_store);

        rend = gtk_cell_renderer_text_new ();
        col = gtk_tree_view_column_new_with_attributes (NULL, rend,
                                                        "text", 0,
                                                        NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW (view), col);

        rend = gtk_cell_renderer_text_new ();
        col = gtk_tree_view_column_new_with_attributes (NULL, rend,
                                                        "text", 1,
                                                        NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW (view), col);

        sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (view));
        g_signal_connect (sel, "changed",
                          G_CALLBACK (prefs_format_string_select_callback),
                          entry);

        for (i = 0; i < G_N_ELEMENTS (format_strings); i++)
          {
            gtk_list_store_append (list_store, &iter);
            gtk_list_store_set (list_store, &iter,
                                0, format_names[i],
                                1, format_strings[i],
                                -1);

            if (i == 0)
              gtk_tree_selection_select_iter (sel, &iter);
          }
      }
  }


  /*************************/
  /*  Interface / Display  */
  /*************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Display"),
                                     "display.png",
				     GTK_TREE_STORE (tree),
				     _("Display"),
				     "dialogs/preferences/display.html",
				     &top_iter,
				     &child_iter,
				     page_index++);

  /*  Transparency  */
  vbox2 = prefs_frame_new (_("Transparency"), GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (2, GTK_CONTAINER (vbox2), TRUE);

  prefs_enum_option_menu_add (config, "transparency-type", 0, 0,
                              _("Transparency _Type:"),
                              GTK_TABLE (table), 0);
  prefs_enum_option_menu_add (config, "transparency-size", 0, 0,
                              _("Check _Size:"),
                              GTK_TABLE (table), 1);

  /*  8-Bit Displays  */
  vbox2 = prefs_frame_new (_("8-Bit Displays"), GTK_CONTAINER (vbox), FALSE);

  if (gdk_rgb_get_visual ()->depth != 8)
    gtk_widget_set_sensitive (GTK_WIDGET (vbox2->parent), FALSE);

  table = prefs_table_new (1, GTK_CONTAINER (vbox2), FALSE);

  prefs_spin_button_add (config, "min-colors", 1.0, 8.0, 0,
                         _("Minimum Number of Colors:"),
                         GTK_TABLE (table), 0);
  prefs_check_button_add (config, "install-colormap",
                          _("Install Colormap"),
                          GTK_BOX (vbox2));
  prefs_check_button_add (config, "colormap-cycling",
                          _("Colormap Cycling"),
                          GTK_BOX (vbox2));


  /*************************/
  /*  Interface / Monitor  */
  /*************************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Monitor"),
                                     "monitor.png",
				     GTK_TREE_STORE (tree),
				     _("Monitor"),
				     "dialogs/preferences/monitor.html",
				     &top_iter,
				     &child_iter,
				     page_index++);

  vbox2 = prefs_frame_new (_("Get Monitor Resolution"),
                           GTK_CONTAINER (vbox), FALSE);

  {
    gdouble  xres, yres;
    gchar   *str;

    gui_get_screen_resolution (&xres, &yres);

    str = g_strdup_printf (_("(Currently %d x %d dpi)"),
			   ROUND (xres), ROUND (yres));
    label = gtk_label_new (str);
    g_free (str);
  }

  abox = gtk_alignment_new (0.5, 0.5, 0.0, 0.0);

  sizeentry = gimp_prop_coordinates_new (config,
                                         "monitor-xresolution",
                                         "monitor-yresolution",
                                         NULL,
                                         pixels_per_unit,
                                         GIMP_SIZE_ENTRY_UPDATE_RESOLUTION,
                                         0.0, 0.0,
                                         TRUE);

  g_free (pixels_per_unit);
  pixels_per_unit = NULL;

  gtk_table_set_col_spacings (GTK_TABLE (sizeentry), 2);
  gtk_table_set_row_spacings (GTK_TABLE (sizeentry), 2);

  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("Horizontal"), 0, 1, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("Vertical"), 0, 2, 0.0);
  gimp_size_entry_attach_label (GIMP_SIZE_ENTRY (sizeentry),
				_("dpi"), 1, 4, 0.0);

  gtk_container_add (GTK_CONTAINER (abox), sizeentry);
  gtk_widget_show (sizeentry);
  gtk_widget_set_sensitive (sizeentry, ! display_config->monitor_res_from_gdk);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 4);
  
  calibrate_button = gtk_button_new_with_mnemonic (_("C_alibrate"));
  gtk_misc_set_padding (GTK_MISC (GTK_BIN (calibrate_button)->child), 4, 0);
  gtk_box_pack_start (GTK_BOX (hbox), calibrate_button, FALSE, FALSE, 0);
  gtk_widget_show (calibrate_button);
  gtk_widget_set_sensitive (calibrate_button,
                            ! display_config->monitor_res_from_gdk);

  g_signal_connect (calibrate_button, "clicked",
		    G_CALLBACK (prefs_resolution_calibrate_callback),
		    sizeentry);

  group = NULL;

  button = gtk_radio_button_new_with_mnemonic (group,
                                               _("From _Windowing System"));
  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
  gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_object_set_data (G_OBJECT (button), "monitor_resolution_sizeentry",
                     sizeentry);
  g_object_set_data (G_OBJECT (button), "set_sensitive",
                     label);
  g_object_set_data (G_OBJECT (button), "inverse_sensitive",
                     sizeentry);
  g_object_set_data (G_OBJECT (sizeentry), "inverse_sensitive",
                     calibrate_button);

  g_signal_connect (button, "toggled",
		    G_CALLBACK (prefs_res_source_callback),
		    config);

  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox2), separator, FALSE, FALSE, 0);
  gtk_widget_show (separator);

  button = gtk_radio_button_new_with_mnemonic (group, _("_Manually"));
  group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
  gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  gtk_box_pack_start (GTK_BOX (vbox2), abox, FALSE, FALSE, 0);
  gtk_widget_show (abox);

  gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  if (! display_config->monitor_res_from_gdk)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);


  /*****************/
  /*  Environment  */
  /*****************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Environment"),
                                     "environment.png",
				     GTK_TREE_STORE (tree),
				     _("Environment"),
				     "dialogs/preferences/environment.html",
				     NULL,
				     &top_iter,
				     page_index++);

  /*  Resource Consumption  */
  vbox2 = prefs_frame_new (_("Resource Consumption"),
                           GTK_CONTAINER (vbox), FALSE);

  prefs_check_button_add (config, "stingy-memory-use",
                          _("Conservative Memory Usage"),
                          GTK_BOX (vbox2));

#ifdef ENABLE_MP
  table = prefs_table_new (4, GTK_CONTAINER (vbox2), FALSE);
#else
  table = prefs_table_new (3, GTK_CONTAINER (vbox2), FALSE);
#endif /* ENABLE_MP */

  prefs_spin_button_add (config, "undo-levels", 1.0, 5.0, 0,
                         _("Minimal Number of Undo Levels:"),
                         GTK_TABLE (table), 0);
  prefs_memsize_entry_add (config, "undo-size",
                           _("Maximum Undo Memory:"),
                           GTK_TABLE (table), 1);
  prefs_memsize_entry_add (config, "tile-cache-size",
                           _("Tile Cache Size:"),
                           GTK_TABLE (table), 2);

#ifdef ENABLE_MP
  prefs_spin_button_add (config, "num-processors", 1.0, 4.0, 0,
                         _("Number of Processors to Use:"),
                         GTK_TABLE (table), 3);
#endif /* ENABLE_MP */

  /*  File Saving  */
  vbox2 = prefs_frame_new (_("File Saving"), GTK_CONTAINER (vbox), FALSE);
  table = prefs_table_new (2, GTK_CONTAINER (vbox2), TRUE);

  prefs_boolean_option_menu_add (config, "trust-dirty-flag",
                                 _("Only when Modified"),
                                 _("Always"),
                                 _("\"File -> Save\" Saves the Image:"),
                                 GTK_TABLE (table), 0);
  prefs_enum_option_menu_add (config, "thumbnail-size", 0, 0,
                              _("Size of Thumbnail Files:"),
                              GTK_TABLE (table), 1);


  /*************/
  /*  Folders  */
  /*************/
  vbox = prefs_notebook_append_page (gimp,
                                     GTK_NOTEBOOK (notebook),
				     _("Folders"),
                                     "folders.png",
				     GTK_TREE_STORE (tree),
				     _("Folders"),
				     "dialogs/preferences/folders.html",
				     NULL,
				     &top_iter,
				     page_index++);

  {
    static const struct
    {
      const gchar *label;
      const gchar *fs_label;
      const gchar *property_name;
    }
    dirs[] =
    {
      { N_("Temp Dir:"), N_("Select Temp Dir"), "temp-path" },
      { N_("Swap Dir:"), N_("Select Swap Dir"), "swap-path" },
    };

    table = prefs_table_new (G_N_ELEMENTS (dirs) + 1,
                             GTK_CONTAINER (vbox), FALSE);

    for (i = 0; i < G_N_ELEMENTS (dirs); i++)
      {
	fileselection = gimp_prop_file_entry_new (config, dirs[i].property_name,
                                                  gettext (dirs[i].fs_label),
                                                  TRUE, TRUE);
	gimp_table_attach_aligned (GTK_TABLE (table), 0, i,
				   gettext (dirs[i].label), 1.0, 0.5,
				   fileselection, 1, FALSE);
      }
  }


  /*********************/
  /* Folders / <paths> */
  /*********************/
  {
    static const struct
    {
      const gchar *tree_label;
      const gchar *label;
      const gchar *icon;
      const gchar *help_data;
      const gchar *fs_label;
      const gchar *property_name;
    }
    paths[] =
    {
      { N_("Brushes"), N_("Brush Folders"), "folders-brushes.png",
	"dialogs/preferences/folders.html#brushes",
	N_("Select Brush Folders"),
	"brush-path" },
      { N_("Patterns"), N_("Pattern Folders"), "folders-patterns.png",
	"dialogs/preferences/folders.html#patterns",
	N_("Select Pattern Folders"),
	"pattern-path" },
      { N_("Palettes"), N_("Palette Folders"), "folders-palettes.png",
	"dialogs/preferences/folders.html#palettes",
	N_("Select Palette Folders"),
        "palette-path" },
      { N_("Gradients"), N_("Gradient Folders"), "folders-gradients.png",
	"dialogs/preferences/folders.html#gradients",
	N_("Select Gradient Folders"),
        "gradient-path" },
      { N_("Plug-Ins"), N_("Plug-In Folders"), "folders-plug-ins.png",
	"dialogs/preferences/folders.html#plug_ins",
	N_("Select Plug-In Folders"),
        "plug-in-path" },
      { N_("Tool Plug-Ins"), N_("Tool Plug-In Folders"), "folders-tool-plug-ins.png",
	"dialogs/preferences/folders.html#tool_plug_ins",
	N_("Select Tool Plug-In Folders"),
        "tool-plug-in-path" },
      { N_("Modules"), N_("Module Folders"), "folders-modules.png",
	"dialogs/preferences/folders.html#modules",
	N_("Select Module Folders"),
        "module-path" },
      { N_("Environment"), N_("Environment Folders"), "folders-environ.png",
	"dialogs/preferences/folders.html#environ",
	N_("Select Environment Folders"),
        "environ-path" },
      { N_("Themes"), N_("Theme Folders"), "folders-themes.png",
	"dialogs/preferences/folders.html#themes",
	N_("Select Theme Folders"),
        "theme-path" }
    };

    for (i = 0; i < G_N_ELEMENTS (paths); i++)
      {
	vbox = prefs_notebook_append_page (gimp,
                                           GTK_NOTEBOOK (notebook),
					   gettext (paths[i].label),
                                           paths[i].icon,
				           GTK_TREE_STORE (tree),
					   gettext (paths[i].tree_label),
					   paths[i].help_data,
					   &top_iter,
					   &child_iter,
					   page_index++);

	patheditor = gimp_prop_path_editor_new (config, paths[i].property_name,
                                                gettext (paths[i].fs_label));
	gtk_container_add (GTK_CONTAINER (vbox), patheditor);
	gtk_widget_show (patheditor);
      }
  }

  gtk_widget_show (tv);
  gtk_widget_show (notebook);

  gtk_tree_view_expand_all (GTK_TREE_VIEW (tv));

  return dialog;
}
