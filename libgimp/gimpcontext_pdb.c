/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpcontext_pdb.c
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

/* NOTE: This file is autogenerated by pdbgen.pl */

#include "config.h"

#include "gimp.h"

/**
 * gimp_context_push:
 *
 * Pushes a context to the top of the plug-in's context stack.
 *
 * This procedure creates a new context by copying the current context.
 * This copy becomes the new current context for the calling plug-in
 * until it is popped again using gimp-context-pop.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_push (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-push",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_pop:
 *
 * Pops the topmost context from the plug-in's context stack.
 *
 * This procedure removes the topmost context from the plug-in's
 * context stack. The context that was active before the corresponding
 * call to gimp-context-push becomes the new current context of the
 * plug-in.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_pop (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-pop",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_paint_method:
 *
 * Retrieve the currently active paint method.
 *
 * This procedure returns the name of the currently active paint
 * method.
 *
 * Returns: The name of the active paint method.
 *
 * Since: GIMP 2.4
 */
gchar *
gimp_context_get_paint_method (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-paint-method",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_paint_method:
 * @name: The name of the paint method.
 *
 * Set the specified paint method as the active paint method.
 *
 * This procedure allows the active paint method to be set by
 * specifying its name. The name is simply a string which corresponds
 * to one of the names of the available paint methods. If there is no
 * matching method found, this procedure will return an error.
 * Otherwise, the specified method becomes active and will be used in
 * all subsequent paint operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.4
 */
gboolean
gimp_context_set_paint_method (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-paint-method",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_foreground:
 * @foreground: The foreground color.
 *
 * Get the current GIMP foreground color.
 *
 * This procedure returns the current GIMP foreground color. The
 * foreground color is used in a variety of tools such as paint tools,
 * blending, and bucket fill.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_get_foreground (GimpRGB *foreground)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-get-foreground",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *foreground = return_vals[1].data.d_color;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_set_foreground:
 * @foreground: The foreground color.
 *
 * Set the current GIMP foreground color.
 *
 * This procedure sets the current GIMP foreground color. After this is
 * set, operations which use foreground such as paint tools, blending,
 * and bucket fill will use the new value.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_foreground (const GimpRGB *foreground)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-foreground",
				    &nreturn_vals,
				    GIMP_PDB_COLOR, foreground,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_background:
 * @background: The background color.
 *
 * Get the current GIMP background color.
 *
 * This procedure returns the current GIMP background color. The
 * background color is used in a variety of tools such as blending,
 * erasing (with non-alpha images), and image filling.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_get_background (GimpRGB *background)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-get-background",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  if (success)
    *background = return_vals[1].data.d_color;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_set_background:
 * @background: The background color.
 *
 * Set the current GIMP background color.
 *
 * This procedure sets the current GIMP background color. After this is
 * set, operations which use background such as blending, filling
 * images, clearing, and erasing (in non-alpha images) will use the new
 * value.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_background (const GimpRGB *background)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-background",
				    &nreturn_vals,
				    GIMP_PDB_COLOR, background,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_set_default_colors:
 *
 * Set the current GIMP foreground and background colors to black and
 * white.
 *
 * This procedure sets the current GIMP foreground and background
 * colors to their initial default values, black and white.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_default_colors (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-default-colors",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_swap_colors:
 *
 * Swap the current GIMP foreground and background colors.
 *
 * This procedure swaps the current GIMP foreground and background
 * colors, so that the new foreground color becomes the old background
 * color and vice versa.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_swap_colors (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-swap-colors",
				    &nreturn_vals,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_opacity:
 *
 * Get the opacity.
 *
 * This procedure returns the opacity setting. The return value is a
 * floating point number between 0 and 100.
 *
 * Returns: The opacity.
 *
 * Since: GIMP 2.2
 */
gdouble
gimp_context_get_opacity (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gdouble opacity = 0;

  return_vals = gimp_run_procedure ("gimp-context-get-opacity",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    opacity = return_vals[1].data.d_float;

  gimp_destroy_params (return_vals, nreturn_vals);

  return opacity;
}

/**
 * gimp_context_set_opacity:
 * @opacity: The opacity.
 *
 * Set the opacity.
 *
 * This procedure modifies the opacity setting. The value should be a
 * floating point number between 0 and 100.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_opacity (gdouble opacity)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-opacity",
				    &nreturn_vals,
				    GIMP_PDB_FLOAT, opacity,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_paint_mode:
 *
 * Get the paint mode.
 *
 * This procedure returns the paint-mode setting. The return value is
 * an integer which corresponds to the values listed in the argument
 * description.
 *
 * Returns: The paint mode.
 *
 * Since: GIMP 2.2
 */
GimpLayerModeEffects
gimp_context_get_paint_mode (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  GimpLayerModeEffects paint_mode = 0;

  return_vals = gimp_run_procedure ("gimp-context-get-paint-mode",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    paint_mode = return_vals[1].data.d_int32;

  gimp_destroy_params (return_vals, nreturn_vals);

  return paint_mode;
}

/**
 * gimp_context_set_paint_mode:
 * @paint_mode: The paint mode.
 *
 * Set the paint mode.
 *
 * This procedure modifies the paint_mode setting.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_paint_mode (GimpLayerModeEffects paint_mode)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-paint-mode",
				    &nreturn_vals,
				    GIMP_PDB_INT32, paint_mode,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_brush:
 *
 * Retrieve the currently active brush.
 *
 * This procedure returns the name of the currently active brush. All
 * paint operations and stroke operations use this brush to control the
 * application of paint to the image.
 *
 * Returns: The name of the active brush.
 *
 * Since: GIMP 2.2
 */
gchar *
gimp_context_get_brush (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-brush",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_brush:
 * @name: The name of the brush.
 *
 * Set the specified brush as the active brush.
 *
 * This procedure allows the active brush to be set by specifying its
 * name. The name is simply a string which corresponds to one of the
 * names of the installed brushes. If there is no matching brush found,
 * this procedure will return an error. Otherwise, the specified brush
 * becomes active and will be used in all subsequent paint operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_brush (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-brush",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_pattern:
 *
 * Retrieve the currently active pattern.
 *
 * This procedure returns name of the the currently active pattern. All
 * clone and bucket-fill operations with patterns will use this pattern
 * to control the application of paint to the image.
 *
 * Returns: The name of the active pattern.
 *
 * Since: GIMP 2.2
 */
gchar *
gimp_context_get_pattern (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-pattern",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_pattern:
 * @name: The name of the pattern.
 *
 * Set the specified pattern as the active pattern.
 *
 * This procedure allows the active pattern to be set by specifying its
 * name. The name is simply a string which corresponds to one of the
 * names of the installed patterns. If there is no matching pattern
 * found, this procedure will return an error. Otherwise, the specified
 * pattern becomes active and will be used in all subsequent paint
 * operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_pattern (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-pattern",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_gradient:
 *
 * Retrieve the currently active gradient.
 *
 * This procedure returns the name of the currently active gradient.
 *
 * Returns: The name of the active gradient.
 *
 * Since: GIMP 2.2
 */
gchar *
gimp_context_get_gradient (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-gradient",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_gradient:
 * @name: The name of the gradient.
 *
 * Sets the specified gradient as the active gradient.
 *
 * This procedure lets you set the specified gradient as the active or
 * \"current\" one. The name is simply a string which corresponds to
 * one of the loaded gradients. If no matching gradient is found, this
 * procedure will return an error. Otherwise, the specified gradient
 * will become active and will be used for subsequent custom gradient
 * operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_gradient (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-gradient",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_palette:
 *
 * Retrieve the currently active palette.
 *
 * This procedure returns the name of the the currently active palette.
 *
 * Returns: The name of the active palette.
 *
 * Since: GIMP 2.2
 */
gchar *
gimp_context_get_palette (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-palette",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_palette:
 * @name: The name of the palette.
 *
 * Set the specified palette as the active palette.
 *
 * This procedure allows the active palette to be set by specifying its
 * name. The name is simply a string which corresponds to one of the
 * names of the installed palettes. If no matching palette is found,
 * this procedure will return an error. Otherwise, the specified
 * palette becomes active and will be used in all subsequent palette
 * operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_palette (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-palette",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_context_get_font:
 *
 * Retrieve the currently active font.
 *
 * This procedure returns the name of the currently active font.
 *
 * Returns: The name of the active font.
 *
 * Since: GIMP 2.2
 */
gchar *
gimp_context_get_font (void)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gchar *name = NULL;

  return_vals = gimp_run_procedure ("gimp-context-get-font",
				    &nreturn_vals,
				    GIMP_PDB_END);

  if (return_vals[0].data.d_status == GIMP_PDB_SUCCESS)
    name = g_strdup (return_vals[1].data.d_string);

  gimp_destroy_params (return_vals, nreturn_vals);

  return name;
}

/**
 * gimp_context_set_font:
 * @name: The name of the font.
 *
 * Set the specified font as the active font.
 *
 * This procedure allows the active font to be set by specifying its
 * name. The name is simply a string which corresponds to one of the
 * names of the installed fonts. If no matching font is found, this
 * procedure will return an error. Otherwise, the specified font
 * becomes active and will be used in all subsequent font operations.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.2
 */
gboolean
gimp_context_set_font (const gchar *name)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-context-set-font",
				    &nreturn_vals,
				    GIMP_PDB_STRING, name,
				    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
