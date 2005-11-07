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

#include <errno.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <glib/gstdio.h>
#include <gtk/gtk.h>

#include "libgimpbase/gimpbase.h"

#ifdef G_OS_WIN32
#include "libgimpbase/gimpwin32-io.h"
#endif

#include "menus-types.h"

#include "config/gimpguiconfig.h"

#include "core/gimp.h"

#include "widgets/gimpactionfactory.h"
#include "widgets/gimpmenufactory.h"

#include "dockable-menu.h"
#include "image-menu.h"
#include "menus.h"
#include "plug-in-menus.h"
#include "tool-options-menu.h"
#include "toolbox-menu.h"

#include "gimp-intl.h"


/*  local function prototypes  */

static void   menu_can_change_accels (GimpGuiConfig *config);


/*  global variables  */

GimpMenuFactory * global_menu_factory = NULL;


/*  private variables  */

static gboolean   menurc_deleted      = FALSE;


/*  public functions  */

void
menus_init (Gimp              *gimp,
            GimpActionFactory *action_factory)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (GIMP_IS_ACTION_FACTORY (action_factory));
  g_return_if_fail (global_menu_factory == NULL);

  /* We need to make sure the property is installed before using it */
  g_type_class_ref (GTK_TYPE_MENU);

  menu_can_change_accels (GIMP_GUI_CONFIG (gimp->config));

  g_signal_connect (gimp->config, "notify::can-change-accels",
                    G_CALLBACK (menu_can_change_accels), NULL);

  global_menu_factory = gimp_menu_factory_new (gimp, action_factory);

  gimp_menu_factory_manager_register (global_menu_factory, "<Image>",
                                      "file",
                                      "context",
                                      "edit",
                                      "select",
                                      "view",
                                      "image",
                                      "drawable",
                                      "layers",
                                      "channels",
                                      "vectors",
                                      "tools",
                                      "dialogs",
                                      "plug-in",
                                      "quick-mask",
                                      NULL,
                                      "/image-menubar",
                                      "image-menu.xml", image_menu_setup,
                                      "/dummy-menubar",
                                      "image-menu.xml", image_menu_setup,
                                      "/quick-mask-popup",
                                      "quick-mask-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Toolbox>",
                                      "file",
                                      "context",
                                      "debug",
                                      "help",
                                      "edit",
                                      "select",
                                      "image",
                                      "drawable",
                                      "layers",
                                      "channels",
                                      "vectors",
                                      "tools",
                                      "dialogs",
                                      "plug-in",
                                      "quick-mask",
                                      NULL,
                                      "/toolbox-menubar",
                                      "toolbox-menu.xml", toolbox_menu_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Dock>",
                                      "file",
                                      "context",
                                      "edit",
                                      "select",
                                      "image",
                                      "drawable",
                                      "layers",
                                      "channels",
                                      "vectors",
                                      "tools",
                                      "dialogs",
                                      "plug-in",
                                      "quick-mask",
                                      "dock",
                                      NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Layers>",
                                      "layers",
                                      NULL,
                                      "/layers-popup",
                                      "layers-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Channels>",
                                      "channels",
                                      NULL,
                                      "/channels-popup",
                                      "channels-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Vectors>",
                                      "vectors",
                                      NULL,
                                      "/vectors-popup",
                                      "vectors-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Dockable>",
                                      "dockable",
                                      "dock",
                                      NULL,
                                      "/dockable-popup",
                                      "dockable-menu.xml", dockable_menu_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Brushes>",
                                      "brushes",
                                      "plug-in",
                                      NULL,
                                      "/brushes-popup",
                                      "brushes-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Patterns>",
                                      "patterns",
                                      "plug-in",
                                      NULL,
                                      "/patterns-popup",
                                      "patterns-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Gradients>",
                                      "gradients",
                                      "plug-in",
                                      NULL,
                                      "/gradients-popup",
                                      "gradients-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Palettes>",
                                      "palettes",
                                      "plug-in",
                                      NULL,
                                      "/palettes-popup",
                                      "palettes-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Fonts>",
                                      "fonts",
                                      "plug-in",
                                      NULL,
                                      "/fonts-popup",
                                      "fonts-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Buffers>",
                                      "buffers",
                                      "plug-in",
                                      NULL,
                                      "/buffers-popup",
                                      "buffers-menu.xml", plug_in_menus_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Documents>",
                                      "documents",
                                      NULL,
                                      "/documents-popup",
                                      "documents-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Templates>",
                                      "templates",
                                      NULL,
                                      "/templates-popup",
                                      "templates-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Images>",
                                      "images",
                                      NULL,
                                      "/images-popup",
                                      "images-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<Tools>",
                                      "tools",
                                      NULL,
                                      "/tools-popup",
                                      "tools-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<BrushEditor>",
                                      "brush-editor",
                                      NULL,
                                      "/brush-editor-popup",
                                      "brush-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<GradientEditor>",
                                      "gradient-editor",
                                      NULL,
                                      "/gradient-editor-popup",
                                      "gradient-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<PaletteEditor>",
                                      "palette-editor",
                                      NULL,
                                      "/palette-editor-popup",
                                      "palette-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<ColormapEditor>",
                                      "colormap-editor",
                                      NULL,
                                      "/colormap-editor-popup",
                                      "colormap-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<SelectionEditor>",
                                      "select",
                                      "vectors",
                                      NULL,
                                      "/selection-editor-popup",
                                      "selection-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<NavigationEditor>",
                                      "view",
                                      NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<UndoEditor>",
                                      "edit",
                                      NULL,
                                      "/undo-editor-popup",
                                      "undo-editor-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<ErrorConsole>",
                                      "error-console",
                                      NULL,
                                      "/error-console-popup",
                                      "error-console-menu.xml", NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<ToolOptions>",
                                      "tool-options",
                                      NULL,
                                      "/tool-options-popup",
                                      "tool-options-menu.xml",
                                      tool_options_menu_setup,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<TextEditor>",
                                      "text-editor",
                                      NULL,
                                      "/text-editor-toolbar",
                                      "text-editor-toolbar.xml",
                                      NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<CursorInfo>",
                                      "cursor-info",
                                      NULL,
                                      "/cursor-info-popup",
                                      "cursor-info-menu.xml",
                                      NULL,
                                      NULL);

  gimp_menu_factory_manager_register (global_menu_factory, "<SamplePointEditor>",
                                      "sample-point-editor",
                                      NULL,
                                      "/sample-point-editor-popup",
                                      "sample-point-editor-menu.xml",
                                      NULL,
                                      NULL);
}

void
menus_exit (Gimp *gimp)
{
  g_return_if_fail (GIMP_IS_GIMP (gimp));
  g_return_if_fail (global_menu_factory != NULL);
  g_return_if_fail (global_menu_factory->gimp == gimp);

  g_object_unref (global_menu_factory);
  global_menu_factory = NULL;

  g_signal_handlers_disconnect_by_func (gimp->config,
                                        menu_can_change_accels,
                                        NULL);
}

void
menus_restore (Gimp *gimp)
{
  gchar *filename;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  filename = gimp_personal_rc_file ("menurc");

  if (gimp->be_verbose)
    g_print ("Parsing '%s'\n", gimp_filename_to_utf8 (filename));

  gtk_accel_map_load (filename);
  g_free (filename);
}

void
menus_save (Gimp     *gimp,
            gboolean  always_save)
{
  gchar *filename;

  g_return_if_fail (GIMP_IS_GIMP (gimp));

  if (menurc_deleted && ! always_save)
    return;

  filename = gimp_personal_rc_file ("menurc");

  if (gimp->be_verbose)
    g_print ("Writing '%s'\n", gimp_filename_to_utf8 (filename));

  gtk_accel_map_save (filename);
  g_free (filename);

  menurc_deleted = FALSE;
}

gboolean
menus_clear (Gimp    *gimp,
             GError **error)
{
  gchar    *filename;
  gboolean  success = TRUE;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  filename = gimp_personal_rc_file ("menurc");

  if (g_unlink (filename) != 0 && errno != ENOENT)
    {
      g_set_error (error, 0, 0, _("Deleting \"%s\" failed: %s"),
                   gimp_filename_to_utf8 (filename), g_strerror (errno));
      success = FALSE;
    }
  else
    {
      menurc_deleted = TRUE;
    }

  g_free (filename);

  return success;
}


/*  private functions  */

static void
menu_can_change_accels (GimpGuiConfig *config)
{
  g_object_set (gtk_settings_get_for_screen (gdk_screen_get_default ()),
                "gtk-can-change-accels", config->can_change_accels,
                NULL);
}
