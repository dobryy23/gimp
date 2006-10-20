/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpselectiontools_pdb.c
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
 * gimp_by_color_select:
 * @drawable_ID: The affected drawable.
 * @color: The color to select.
 * @threshold: Threshold in intensity levels.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius: Radius for feather operation.
 * @sample_merged: Use the composite image, not the drawable.
 *
 * Create a selection by selecting all pixels (in the specified
 * drawable) with the same (or similar) color to that specified.
 *
 * This tool creates a selection over the specified image. A by-color
 * selection is determined by the supplied color under the constraints
 * of the specified threshold. Essentially, all pixels (in the
 * drawable) that have color sufficiently close to the specified color
 * (as determined by the threshold value) are included in the
 * selection. The antialiasing parameter allows the final selection
 * mask to contain intermediate values based on close misses to the
 * threshold bar. Feathering can be enabled optionally and is
 * controlled with the 'feather-radius' parameter. If the
 * 'sample-merged' parameter is TRUE, the data of the composite image
 * will be used instead of that for the specified drawable. This is
 * equivalent to sampling for colors after merging all visible layers.
 * In the case of a merged sampling, the supplied drawable is ignored.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_by_color_select (gint32          drawable_ID,
                      const GimpRGB  *color,
                      gint            threshold,
                      GimpChannelOps  operation,
                      gboolean        antialias,
                      gboolean        feather,
                      gdouble         feather_radius,
                      gboolean        sample_merged)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-by-color-select",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_COLOR, color,
                                    GIMP_PDB_INT32, threshold,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius,
                                    GIMP_PDB_INT32, sample_merged,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_by_color_select_full:
 * @drawable_ID: The affected drawable.
 * @color: The color to select.
 * @threshold: Threshold in intensity levels.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius_x: Radius for feather operation in X direction.
 * @feather_radius_y: Radius for feather operation in Y direction.
 * @sample_merged: Use the composite image, not the drawable.
 * @select_transparent: Whether to consider transparent pixels for selection. If TRUE, transparency is considered as a unique selectable color.
 * @select_criterion: The criterion used to determine color similarity. SELECT_CRITERION_COMPOSITE is the standard choice.
 *
 * Create a selection by selecting all pixels (in the specified
 * drawable) with the same (or similar) color to that specified.
 *
 * This tool creates a selection over the specified image. A by-color
 * selection is determined by the supplied color under the constraints
 * of the specified threshold. Essentially, all pixels (in the
 * drawable) that have color sufficiently close to the specified color
 * (as determined by the threshold value) are included in the
 * selection. To select transparent regions, the color specified must
 * also have minimum alpha. The antialiasing parameter allows the final
 * selection mask to contain intermediate values based on close misses
 * to the threshold bar. Feathering can be enabled optionally and is
 * controlled with the 'feather-radius' parameter. If the
 * 'sample-merged' parameter is TRUE, the data of the composite image
 * will be used instead of that for the specified drawable. This is
 * equivalent to sampling for colors after merging all visible layers.
 * In the case of a merged sampling, the supplied drawable is ignored.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.4
 */
gboolean
gimp_by_color_select_full (gint32               drawable_ID,
                           const GimpRGB       *color,
                           gint                 threshold,
                           GimpChannelOps       operation,
                           gboolean             antialias,
                           gboolean             feather,
                           gdouble              feather_radius_x,
                           gdouble              feather_radius_y,
                           gboolean             sample_merged,
                           gboolean             select_transparent,
                           GimpSelectCriterion  select_criterion)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-by-color-select-full",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_COLOR, color,
                                    GIMP_PDB_INT32, threshold,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius_x,
                                    GIMP_PDB_FLOAT, feather_radius_y,
                                    GIMP_PDB_INT32, sample_merged,
                                    GIMP_PDB_INT32, select_transparent,
                                    GIMP_PDB_INT32, select_criterion,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_ellipse_select:
 * @image_ID: The image.
 * @x: x coordinate of upper-left corner of ellipse bounding box.
 * @y: y coordinate of upper-left corner of ellipse bounding box.
 * @width: The width of the ellipse.
 * @height: The height of the ellipse.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius: Radius for feather operation.
 *
 * Create an elliptical selection over the specified image.
 *
 * This tool creates an elliptical selection over the specified image.
 * The elliptical region can be either added to, subtracted from, or
 * replace the contents of the previous selection mask. If antialiasing
 * is turned on, the edges of the elliptical region will contain
 * intermediate values which give the appearance of a sharper, less
 * pixelized edge. This should be set as TRUE most of the time. If the
 * feather option is enabled, the resulting selection is blurred before
 * combining. The blur is a gaussian blur with the specified feather
 * radius.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_ellipse_select (gint32         image_ID,
                     gdouble        x,
                     gdouble        y,
                     gdouble        width,
                     gdouble        height,
                     GimpChannelOps operation,
                     gboolean       antialias,
                     gboolean       feather,
                     gdouble        feather_radius)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-ellipse-select",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_FLOAT, x,
                                    GIMP_PDB_FLOAT, y,
                                    GIMP_PDB_FLOAT, width,
                                    GIMP_PDB_FLOAT, height,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_free_select:
 * @image_ID: The image.
 * @num_segs: Number of points (count 1 coordinate as two points).
 * @segs: Array of points: { p1.x, p1.y, p2.x, p2.y, ..., pn.x, pn.y}.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius: Radius for feather operation.
 *
 * Create a polygonal selection over the specified image.
 *
 * This tool creates a polygonal selection over the specified image.
 * The polygonal region can be either added to, subtracted from, or
 * replace the contents of the previous selection mask. The polygon is
 * specified through an array of floating point numbers and its length.
 * The length of array must be 2n, where n is the number of points.
 * Each point is defined by 2 floating point values which correspond to
 * the x and y coordinates. If the final point does not connect to the
 * starting point, a connecting segment is automatically added. If the
 * feather option is enabled, the resulting selection is blurred before
 * combining. The blur is a gaussian blur with the specified feather
 * radius.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_free_select (gint32          image_ID,
                  gint            num_segs,
                  const gdouble  *segs,
                  GimpChannelOps  operation,
                  gboolean        antialias,
                  gboolean        feather,
                  gdouble         feather_radius)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-free-select",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_INT32, num_segs,
                                    GIMP_PDB_FLOATARRAY, segs,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_fuzzy_select:
 * @drawable_ID: The affected drawable.
 * @x: x coordinate of initial seed fill point: (image coordinates).
 * @y: y coordinate of initial seed fill point: (image coordinates).
 * @threshold: Threshold in intensity levels.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius: Radius for feather operation.
 * @sample_merged: Use the composite image, not the drawable.
 *
 * Create a fuzzy selection starting at the specified coordinates on
 * the specified drawable.
 *
 * This tool creates a fuzzy selection over the specified image. A
 * fuzzy selection is determined by a seed fill under the constraints
 * of the specified threshold. Essentially, the color at the specified
 * coordinates (in the drawable) is measured and the selection expands
 * outwards from that point to any adjacent pixels which are not
 * significantly different (as determined by the threshold value). This
 * process continues until no more expansion is possible. The
 * antialiasing parameter allows the final selection mask to contain
 * intermediate values based on close misses to the threshold bar at
 * pixels along the seed fill boundary. Feathering can be enabled
 * optionally and is controlled with the 'feather-radius' paramter. If
 * the 'sample-merged' parameter is TRUE, the data of the composite
 * image will be used instead of that for the specified drawable. This
 * is equivalent to sampling for colors after merging all visible
 * layers. In the case of a merged sampling, the supplied drawable is
 * ignored. If the sample is merged, the specified coordinates are
 * relative to the image origin; otherwise, they are relative to the
 * drawable's origin.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_fuzzy_select (gint32         drawable_ID,
                   gdouble        x,
                   gdouble        y,
                   gint           threshold,
                   GimpChannelOps operation,
                   gboolean       antialias,
                   gboolean       feather,
                   gdouble        feather_radius,
                   gboolean       sample_merged)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-fuzzy-select",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_FLOAT, x,
                                    GIMP_PDB_FLOAT, y,
                                    GIMP_PDB_INT32, threshold,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius,
                                    GIMP_PDB_INT32, sample_merged,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_fuzzy_select_full:
 * @drawable_ID: The affected drawable.
 * @x: x coordinate of initial seed fill point: (image coordinates).
 * @y: y coordinate of initial seed fill point: (image coordinates).
 * @threshold: Threshold in intensity levels.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius_x: Radius for feather operation in X direction.
 * @feather_radius_y: Radius for feather operation in Y direction.
 * @sample_merged: Use the composite image, not the drawable.
 * @select_transparent: Whether to consider transparent pixels for selection. If TRUE, transparency is considered as a unique selectable color.
 * @select_criterion: The criterion used to determine color similarity. SELECT_CRITERION_COMPOSITE is the standard choice.
 *
 * Create a fuzzy selection starting at the specified coordinates on
 * the specified drawable.
 *
 * This tool creates a fuzzy selection over the specified image. A
 * fuzzy selection is determined by a seed fill under the constraints
 * of the specified threshold. Essentially, the color at the specified
 * coordinates (in the drawable) is measured and the selection expands
 * outwards from that point to any adjacent pixels which are not
 * significantly different (as determined by the threshold value). This
 * process continues until no more expansion is possible. The
 * antialiasing parameter allows the final selection mask to contain
 * intermediate values based on close misses to the threshold bar at
 * pixels along the seed fill boundary. Feathering can be enabled
 * optionally and is controlled with the 'feather-radius' paramter. If
 * the 'sample-merged' parameter is TRUE, the data of the composite
 * image will be used instead of that for the specified drawable. This
 * is equivalent to sampling for colors after merging all visible
 * layers. In the case of a merged sampling, the supplied drawable is
 * ignored. If the sample is merged, the specified coordinates are
 * relative to the image origin; otherwise, they are relative to the
 * drawable's origin.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.4
 */
gboolean
gimp_fuzzy_select_full (gint32              drawable_ID,
                        gdouble             x,
                        gdouble             y,
                        gint                threshold,
                        GimpChannelOps      operation,
                        gboolean            antialias,
                        gboolean            feather,
                        gdouble             feather_radius_x,
                        gdouble             feather_radius_y,
                        gboolean            sample_merged,
                        gboolean            select_transparent,
                        GimpSelectCriterion select_criterion)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-fuzzy-select-full",
                                    &nreturn_vals,
                                    GIMP_PDB_DRAWABLE, drawable_ID,
                                    GIMP_PDB_FLOAT, x,
                                    GIMP_PDB_FLOAT, y,
                                    GIMP_PDB_INT32, threshold,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius_x,
                                    GIMP_PDB_FLOAT, feather_radius_y,
                                    GIMP_PDB_INT32, sample_merged,
                                    GIMP_PDB_INT32, select_transparent,
                                    GIMP_PDB_INT32, select_criterion,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_rect_select:
 * @image_ID: The image.
 * @x: x coordinate of upper-left corner of rectangle.
 * @y: y coordinate of upper-left corner of rectangle.
 * @width: The width of the rectangle.
 * @height: The height of the rectangle.
 * @operation: The selection operation.
 * @feather: Feather option for selections.
 * @feather_radius: Radius for feather operation.
 *
 * Create a rectangular selection over the specified image;
 *
 * This tool creates a rectangular selection over the specified image.
 * The rectangular region can be either added to, subtracted from, or
 * replace the contents of the previous selection mask. If the feather
 * option is enabled, the resulting selection is blurred before
 * combining. The blur is a gaussian blur with the specified feather
 * radius.
 *
 * Returns: TRUE on success.
 */
gboolean
gimp_rect_select (gint32         image_ID,
                  gdouble        x,
                  gdouble        y,
                  gdouble        width,
                  gdouble        height,
                  GimpChannelOps operation,
                  gboolean       feather,
                  gdouble        feather_radius)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-rect-select",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_FLOAT, x,
                                    GIMP_PDB_FLOAT, y,
                                    GIMP_PDB_FLOAT, width,
                                    GIMP_PDB_FLOAT, height,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}

/**
 * gimp_round_rect_select:
 * @image_ID: The image.
 * @x: x coordinate of upper-left corner of rectangle.
 * @y: y coordinate of upper-left corner of rectangle.
 * @width: The width of the rectangle.
 * @height: The height of the rectangle.
 * @corner_radius_x: The corner radius in X direction.
 * @corner_radius_y: The corner radius in Y direction.
 * @operation: The selection operation.
 * @antialias: Antialiasing.
 * @feather: Feather option for selections.
 * @feather_radius_x: Radius for feather operation in X direction.
 * @feather_radius_y: Radius for feather operation in Y direction.
 *
 * Create a rectangular selection with round corners over the specified
 * image;
 *
 * This tool creates a rectangular selection with round corners over
 * the specified image. The rectangular region can be either added to,
 * subtracted from, or replace the contents of the previous selection
 * mask. If the feather option is enabled, the resulting selection is
 * blurred before combining. The blur is a gaussian blur with the
 * specified feather radius.
 *
 * Returns: TRUE on success.
 *
 * Since: GIMP 2.4
 */
gboolean
gimp_round_rect_select (gint32         image_ID,
                        gdouble        x,
                        gdouble        y,
                        gdouble        width,
                        gdouble        height,
                        gdouble        corner_radius_x,
                        gdouble        corner_radius_y,
                        GimpChannelOps operation,
                        gboolean       antialias,
                        gboolean       feather,
                        gdouble        feather_radius_x,
                        gdouble        feather_radius_y)
{
  GimpParam *return_vals;
  gint nreturn_vals;
  gboolean success = TRUE;

  return_vals = gimp_run_procedure ("gimp-round-rect-select",
                                    &nreturn_vals,
                                    GIMP_PDB_IMAGE, image_ID,
                                    GIMP_PDB_FLOAT, x,
                                    GIMP_PDB_FLOAT, y,
                                    GIMP_PDB_FLOAT, width,
                                    GIMP_PDB_FLOAT, height,
                                    GIMP_PDB_FLOAT, corner_radius_x,
                                    GIMP_PDB_FLOAT, corner_radius_y,
                                    GIMP_PDB_INT32, operation,
                                    GIMP_PDB_INT32, antialias,
                                    GIMP_PDB_INT32, feather,
                                    GIMP_PDB_FLOAT, feather_radius_x,
                                    GIMP_PDB_FLOAT, feather_radius_y,
                                    GIMP_PDB_END);

  success = return_vals[0].data.d_status == GIMP_PDB_SUCCESS;

  gimp_destroy_params (return_vals, nreturn_vals);

  return success;
}
