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

#include <glib-object.h>

#include "libgimpbase/gimpbase.h"

#include "core-types.h"

#include "paint-funcs/paint-funcs.h"

#include "base/pixel-processor.h"
#include "base/pixel-region.h"

#include "gimpchannel.h"
#include "gimpchannel-combine.h"


void
gimp_channel_add_segment (GimpChannel *mask,
                          gint         x,
                          gint         y,
                          gint         width,
                          gint         value)
{
  PixelRegion  maskPR;
  guchar      *data;
  gint         val;
  gint         x2;
  gpointer     pr;

  g_return_if_fail (GIMP_IS_CHANNEL (mask));

  /*  check horizontal extents...  */
  x2 = x + width;
  x2 = CLAMP (x2, 0, GIMP_ITEM (mask)->width);
  x  = CLAMP (x,  0, GIMP_ITEM (mask)->width);
  width = x2 - x;
  if (!width)
    return;

  if (y < 0 || y > GIMP_ITEM (mask)->height)
    return;

  pixel_region_init (&maskPR, GIMP_DRAWABLE (mask)->tiles,
                     x, y, width, 1, TRUE);

  for (pr = pixel_regions_register (1, &maskPR);
       pr != NULL;
       pr = pixel_regions_process (pr))
    {
      data = maskPR.data;
      width = maskPR.w;
      while (width--)
        {
          val = *data + value;
          if (val > 255)
            val = 255;
          *data++ = val;
        }
    }
}

void
gimp_channel_sub_segment (GimpChannel *mask,
                          gint         x,
                          gint         y,
                          gint         width,
                          gint         value)
{
  PixelRegion  maskPR;
  guchar      *data;
  gint         val;
  gint         x2;
  gpointer     pr;

  g_return_if_fail (GIMP_IS_CHANNEL (mask));

  /*  check horizontal extents...  */
  x2 = x + width;
  x2 = CLAMP (x2, 0, GIMP_ITEM (mask)->width);
  x =  CLAMP (x,  0, GIMP_ITEM (mask)->width);
  width = x2 - x;

  if (! width)
    return;

  if (y < 0 || y > GIMP_ITEM (mask)->height)
    return;

  pixel_region_init (&maskPR, GIMP_DRAWABLE (mask)->tiles,
                     x, y, width, 1, TRUE);

  for (pr = pixel_regions_register (1, &maskPR);
       pr != NULL;
       pr = pixel_regions_process (pr))
    {
      data = maskPR.data;
      width = maskPR.w;
      while (width--)
        {
          val = *data - value;
          if (val < 0)
            val = 0;
          *data++ = val;
        }
    }
}

void
gimp_channel_combine_rect (GimpChannel    *mask,
                           GimpChannelOps  op,
                           gint            x,
                           gint            y,
                           gint            w,
                           gint            h)
{
  PixelRegion maskPR;
  guchar      color;

  g_return_if_fail (GIMP_IS_CHANNEL (mask));

  if (! gimp_rectangle_intersect (x, y, w, h,
                                  0, 0,
                                  GIMP_ITEM (mask)->width,
                                  GIMP_ITEM (mask)->height,
                                  &x, &y, &w, &h))
    return;

  pixel_region_init (&maskPR, GIMP_DRAWABLE (mask)->tiles,
                     x, y, w, h, TRUE);

  if (op == GIMP_CHANNEL_OP_ADD || op == GIMP_CHANNEL_OP_REPLACE)
    color = OPAQUE_OPACITY;
  else
    color = TRANSPARENT_OPACITY;

  color_region (&maskPR, &color);

  /*  Determine new boundary  */
  if (mask->bounds_known && (op == GIMP_CHANNEL_OP_ADD) && !mask->empty)
    {
      if (x < mask->x1)
        mask->x1 = x;
      if (y < mask->y1)
        mask->y1 = y;
      if ((x + w) > mask->x2)
        mask->x2 = (x + w);
      if ((y + h) > mask->y2)
        mask->y2 = (y + h);
    }
  else if (op == GIMP_CHANNEL_OP_REPLACE || mask->empty)
    {
      mask->empty = FALSE;
      mask->x1    = x;
      mask->y1    = y;
      mask->x2    = x + w;
      mask->y2    = y + h;
    }
  else
    mask->bounds_known = FALSE;

  mask->x1 = CLAMP (mask->x1, 0, GIMP_ITEM (mask)->width);
  mask->y1 = CLAMP (mask->y1, 0, GIMP_ITEM (mask)->height);
  mask->x2 = CLAMP (mask->x2, 0, GIMP_ITEM (mask)->width);
  mask->y2 = CLAMP (mask->y2, 0, GIMP_ITEM (mask)->height);

  gimp_drawable_update (GIMP_DRAWABLE (mask), x, y, w, h);
}

/**
 * gimp_channel_combine_ellipse:
 * @mask:      the channel with which to combine the ellipse
 * @op:        whether to replace, add to, or subtract from the current
 *             contents
 * @x:         x coordinate of upper left corner of ellipse
 * @y:         y coordinate of upper left corner of ellipse
 * @w:         width of ellipse bounding box
 * @h:         height of ellipse bounding box
 * @antialias: if %TRUE, antialias the ellipse
 *
 * Mainly used for elliptical selections.  If @op is
 * %GIMP_CHANNEL_OP_REPLACE or %GIMP_CHANNEL_OP_ADD, sets pixels
 * within the ellipse to 255.  If @op is %GIMP_CHANNEL_OP_SUBTRACT,
 * sets pixels within to zero.  If @antialias is %TRUE, pixels that
 * impinge on the edge of the ellipse are set to intermediate values,
 * depending on how much they overlap.
 **/
void
gimp_channel_combine_ellipse (GimpChannel    *mask,
                              GimpChannelOps  op,
                              gint            x,
                              gint            y,
                              gint            w,
                              gint            h,
                              gboolean        antialias)
{
  gint   i, j;
  gfloat a, b;
  gfloat a_sqr, b_sqr, aob_sqr;
  gfloat cx, cy;

  g_return_if_fail (GIMP_IS_CHANNEL (mask));

  if (! gimp_rectangle_intersect (x, y, w, h,
                                  0, 0,
                                  GIMP_ITEM (mask)->width,
                                  GIMP_ITEM (mask)->height,
                                  NULL, NULL, NULL, NULL))
    return;

  a = w / 2.0;
  b = h / 2.0;

  a_sqr = SQR (a);
  b_sqr = SQR (b);

  aob_sqr = a_sqr / b_sqr;

  cx = x + a;
  cy = y + b;

  for (i = y; i < (y + h); i++)
    {
      if (i >= 0 && i < GIMP_ITEM (mask)->height)
        {
          if (!antialias)
            {
              gfloat y_sqr = (i + 0.5 - cy) * (i + 0.5 - cy);
              gfloat rad   = sqrt (a_sqr - a_sqr * y_sqr / (gdouble) b_sqr);
              gint   x1    = ROUND (cx - rad);
              gint   x2    = ROUND (cx + rad);

              switch (op)
                {
                case GIMP_CHANNEL_OP_ADD:
                case GIMP_CHANNEL_OP_REPLACE:
                  gimp_channel_add_segment (mask, x1, i, (x2 - x1), 255);
                  break;

                case GIMP_CHANNEL_OP_SUBTRACT:
                  gimp_channel_sub_segment (mask, x1, i, (x2 - x1), 255);
                  break;

                default:
                  g_return_if_reached ();
                  break;
                }
            }
          else  /*  antialiasing  */
            {
              /* algorithm changed 7-18-04, because the previous one did not
               * work well for eccentric ellipses.  The new algorithm
               * measures the distance to the ellipse in the X and Y directions,
               * and uses trigonometry to approximate the distance to the
               * ellipse as the distance to the hypotenuse of a right triangle
               * whose legs are the X and Y distances.  (WES)
               */

              gint   val, last;
              gint   x0;
              gfloat xj, yi;
              gfloat xdist, ydist;
              gfloat r;
              gfloat dist;

              x0 = x;
              yi = ABS (i + 0.5 - cy);

              last = 0;

              for (j = x; j < (x + w); j++)
                {
                  xj =  ABS (j + 0.5 - cx);

                  if (yi < b)
                    xdist = xj - a * sqrt (1 - yi * yi / b_sqr);
                  else
                    xdist = 100.0;  /* anything large will work */

                  if (xj < a)
                    ydist = yi - b * sqrt (1 - xj * xj / a_sqr);
                  else
                    ydist = 100.0;  /* anything large will work */

                  r = hypot (xdist, ydist);
                  if (r < 0.001)
                    dist = 0.;
                  else
                    dist = xdist * ydist / r; /* trig formula for distance
                                               * to hypotenuse
                                               */

                  if (xdist < 0.0)
                    dist *= -1;

                  if (dist < -0.5)
                    val = 255;
                  else if (dist < 0.5)
                    val = (gint) (255 * (1 - (dist + 0.5)));
                  else
                    val = 0;

                  if (last != val && last)
                    {
                      switch (op)
                        {
                        case GIMP_CHANNEL_OP_ADD:
                        case GIMP_CHANNEL_OP_REPLACE:
                          gimp_channel_add_segment (mask, x0, i, j - x0, last);
                          break;

                        case GIMP_CHANNEL_OP_SUBTRACT:
                          gimp_channel_sub_segment (mask, x0, i, j - x0, last);
                          break;

                        default:
                          g_return_if_reached ();
                          break;
                        }
                    }

                  if (last != val)
                    {
                      x0 = j;
                      last = val;

                      /*  because we are symetric accross the y axis we can
                       *  skip ahead a bit if we are inside the ellipse
                       */
                      if (val == 255 && j < cx)
                        j = cx + (cx - j) - 1;
                    }
                }

              if (last)
                {
                  switch (op)
                    {
                    case GIMP_CHANNEL_OP_ADD:
                    case GIMP_CHANNEL_OP_REPLACE:
                      gimp_channel_add_segment (mask, x0, i, j - x0, last);
                      break;

                    case GIMP_CHANNEL_OP_SUBTRACT:
                      gimp_channel_sub_segment (mask, x0, i, j - x0, last);
                      break;

                    default:
                      g_return_if_reached ();
                      break;
                    }
                }
            }
        }
    }

  /*  determine new boundary  */
  if (mask->bounds_known && (op == GIMP_CHANNEL_OP_ADD) && !mask->empty)
    {
      if (x < mask->x1)
        mask->x1 = x;
      if (y < mask->y1)
        mask->y1 = y;
      if ((x + w) > mask->x2)
        mask->x2 = (x + w);
      if ((y + h) > mask->y2)
        mask->y2 = (y + h);
    }
  else if (op == GIMP_CHANNEL_OP_REPLACE || mask->empty)
    {
      mask->empty = FALSE;
      mask->x1    = x;
      mask->y1    = y;
      mask->x2    = x + w;
      mask->y2    = y + h;
    }
  else
    {
      mask->bounds_known = FALSE;
    }

  mask->x1 = CLAMP (mask->x1, 0, GIMP_ITEM (mask)->width);
  mask->y1 = CLAMP (mask->y1, 0, GIMP_ITEM (mask)->height);
  mask->x2 = CLAMP (mask->x2, 0, GIMP_ITEM (mask)->width);
  mask->y2 = CLAMP (mask->y2, 0, GIMP_ITEM (mask)->height);

  gimp_drawable_update (GIMP_DRAWABLE (mask), x, y, w, h);
}

static void
gimp_channel_combine_sub_region_add (gpointer     unused,
                                     PixelRegion *srcPR,
                                     PixelRegion *destPR)
{
  guchar *src, *dest;
  gint    x, y, val;

  src  = srcPR->data;
  dest = destPR->data;

  for (y = 0; y < srcPR->h; y++)
    {
      for (x = 0; x < srcPR->w; x++)
        {
          val = dest[x] + src[x];
          if (val > 255)
            dest[x] = 255;
          else
            dest[x] = val;
        }
      src  += srcPR->rowstride;
      dest += destPR->rowstride;
    }
}

static void
gimp_channel_combine_sub_region_sub (gpointer     unused,
                                     PixelRegion *srcPR,
                                     PixelRegion *destPR)
{
  guchar *src, *dest;
  gint    x, y;

  src  = srcPR->data;
  dest = destPR->data;

  for (y = 0; y < srcPR->h; y++)
    {
      for (x = 0; x < srcPR->w; x++)
        {
          if (src[x] > dest[x])
            dest[x] = 0;
          else
            dest[x]-= src[x];
        }
      src  += srcPR->rowstride;
      dest += destPR->rowstride;
    }
}

static void
gimp_channel_combine_sub_region_intersect (gpointer     unused,
                                           PixelRegion *srcPR,
                                           PixelRegion *destPR)
{
  guchar *src, *dest;
  gint    x, y;

  src  = srcPR->data;
  dest = destPR->data;

  for (y = 0; y < srcPR->h; y++)
    {
      for (x = 0; x < srcPR->w; x++)
        {
          dest[x] = MIN (dest[x], src[x]);
        }

      src  += srcPR->rowstride;
      dest += destPR->rowstride;
    }
}

void
gimp_channel_combine_mask (GimpChannel    *mask,
                           GimpChannel    *add_on,
                           GimpChannelOps  op,
                           gint            off_x,
                           gint            off_y)
{
  PixelRegion srcPR, destPR;
  gint        x, y, w, h;

  g_return_if_fail (GIMP_IS_CHANNEL (mask));
  g_return_if_fail (GIMP_IS_CHANNEL (add_on));

  if (! gimp_rectangle_intersect (off_x, off_y,
                                  GIMP_ITEM (add_on)->width,
                                  GIMP_ITEM (add_on)->height,
                                  0, 0,
                                  GIMP_ITEM (mask)->width,
                                  GIMP_ITEM (mask)->height,
                                  &x, &y, &w, &h))
    return;

  pixel_region_init (&srcPR, GIMP_DRAWABLE (add_on)->tiles,
                     x - off_x, y - off_y, w, h, FALSE);
  pixel_region_init (&destPR, GIMP_DRAWABLE (mask)->tiles,
                     x, y, w, h, TRUE);

  switch (op)
    {
    case GIMP_CHANNEL_OP_ADD:
    case GIMP_CHANNEL_OP_REPLACE:
      pixel_regions_process_parallel ((PixelProcessorFunc)
                                      gimp_channel_combine_sub_region_add,
                                      NULL, 2, &srcPR, &destPR);
      break;

    case GIMP_CHANNEL_OP_SUBTRACT:
      pixel_regions_process_parallel ((PixelProcessorFunc)
                                      gimp_channel_combine_sub_region_sub,
                                      NULL, 2, &srcPR, &destPR);
      break;

    case GIMP_CHANNEL_OP_INTERSECT:
      pixel_regions_process_parallel ((PixelProcessorFunc)
                                      gimp_channel_combine_sub_region_intersect,
                                      NULL, 2, &srcPR, &destPR);
      break;

    default:
      g_warning ("%s: unknown operation type", G_STRFUNC);
      break;
    }

  mask->bounds_known = FALSE;

  gimp_drawable_update (GIMP_DRAWABLE (mask), x, y, w, h);
}
