/* THIS FILE IS AUTOMATICALLY GENERATED.  DO NOT EDIT */
/* REGENERATE BY EXECUTING: make-installer.py gimp-composite-mmx.o */
#include "config.h"
#include <glib-object.h>
#include <stdlib.h>
#include <stdio.h>
#include "base/base-types.h"
#include "gimp-composite.h"

#include "gimp-composite-mmx.h"


#ifdef USE_MMX
#ifdef ARCH_X86
#if __GNUC__ >= 3
static void (*gimp_composite_mmx[GIMP_COMPOSITE_N][GIMP_PIXELFORMAT_N][GIMP_PIXELFORMAT_N][GIMP_PIXELFORMAT_N])(GimpCompositeContext *) = {
 { /* GIMP_COMPOSITE_NORMAL */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_DISSOLVE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_BEHIND */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_MULTIPLY */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_multiply_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_SCREEN */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_screen_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_OVERLAY */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_DIFFERENCE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_difference_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_ADDITION */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_addition_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_SUBTRACT */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_subtract_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_DARKEN */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_darken_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_LIGHTEN */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_lighten_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_HUE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_SATURATION */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_COLOR_ONLY */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_VALUE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_DIVIDE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_divide_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_DODGE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_dodge_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_BURN */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_burn_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_HARDLIGHT */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_SOFTLIGHT */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_GRAIN_EXTRACT */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_GRAIN_MERGE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_COLOR_ERASE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_ERASE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_REPLACE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_ANTI_ERASE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_BLEND */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_SHADE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_SWAP */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_swap_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_SCALE */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  gimp_composite_scale_rgba8_rgba8_rgba8_mmx,  },
  },
 },
 { /* GIMP_COMPOSITE_CONVERT */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
 { /* GIMP_COMPOSITE_XOR */
  { /* A = v8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = va8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgb8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
  { /* A = rgba8 */
   /* v8     */ { NULL,  NULL,  NULL,  NULL,  },
   /* va8    */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgb8   */ { NULL,  NULL,  NULL,  NULL,  },
   /* rgba8  */ { NULL,  NULL,  NULL,  NULL,  },
  },
 },
};
#endif
#endif
#endif


void
gimp_composite_mmx_install (void)
{
#ifdef USE_MMX
#ifdef ARCH_X86
#if __GNUC__ >= 3
  int mode, a, b, d;

  for (mode = 0; mode < GIMP_COMPOSITE_N; mode++) {
    for (a = 0; a < GIMP_PIXELFORMAT_N; a++) {
      for (b = 0; b < GIMP_PIXELFORMAT_N; b++) {
        for (d = 0; d < GIMP_PIXELFORMAT_N; d++) {
          if (gimp_composite_mmx[mode][a][b][d]) {
            gimp_composite_function[mode][a][b][d] = gimp_composite_mmx[mode][a][b][d];
            if (gimp_composite_options.bits & GIMP_COMPOSITE_OPTION_VERBOSE) {
              printf("gimp_composite_install: %s %s %s %s: %p\n", gimp_composite_mode_astext(mode), gimp_composite_pixelformat_astext(a),  gimp_composite_pixelformat_astext(b), gimp_composite_pixelformat_astext(d), gimp_composite_function[mode][a][b][d]);
            }
          }
        }
      }
    }
  }
#endif
#endif
#endif
  gimp_composite_mmx_init();
}
