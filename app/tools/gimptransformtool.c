/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2001 Spencer Kimball, Peter Mattis, and others
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

#include <stdlib.h>

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"
#include "gui/gui-types.h"

#include "base/base-config.h"
#include "base/pixel-region.h"
#include "base/pixel-surround.h"
#include "base/tile-manager.h"
#include "base/tile.h"

#include "paint-funcs/paint-funcs.h"

#include "core/gimp.h"
#include "core/gimpchannel.h"
#include "core/gimpcontext.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimpimage-mask.h"
#include "core/gimplayer.h"
#include "core/gimpmarshal.h"
#include "core/gimptoolinfo.h"

#include "gui/info-dialog.h"

#include "display/gimpdisplay.h"
#include "display/gimpdisplay-foreach.h"
#include "display/gimpdisplayshell.h"

#include "tool_manager.h"
#include "tool_options.h"
#include "transform_options.h"
#include "gimptransformtool.h"
#include "gimpperspectivetool.h"
#include "gimprotatetool.h"
#include "gimpscaletool.h"
#include "gimpsheartool.h"
#include "gimpfliptool.h"

#include "app_procs.h"
#include "floating_sel.h"
#include "undo.h"
#include "path_transform.h"

#include "libgimp/gimpintl.h"


enum
{
  TRANSFORM,
  LAST_SIGNAL
};

#define HANDLE 10

#define BILINEAR(jk,j1k,jk1,j1k1,dx,dy) \
                ((1-dy) * (jk + dx * (j1k - jk)) + \
		    dy  * (jk1 + dx * (j1k1 - jk1)))

/* access interleaved pixels */
#define CUBIC_ROW(dx, row, step) \
  gimp_transform_tool_cubic(dx, (row)[0], (row)[step], (row)[step+step], (row)[step+step+step])
#define CUBIC_SCALED_ROW(dx, row, step, i) \
  gimp_transform_tool_cubic(dx, (row)[0] * (row)[i], \
            (row)[step] * (row)[step + i], \
            (row)[step+step]* (row)[step+step + i], \
            (row)[step+step+step] * (row)[step+step+step + i])

#define REF_TILE(i,x,y) \
     tile[i] = tile_manager_get_tile (float_tiles, x, y, TRUE, FALSE); \
     src[i] = tile_data_pointer (tile[i], (x) % TILE_WIDTH, (y) % TILE_HEIGHT);


/*  forward function declarations  */
static void      gimp_transform_tool_bounds    (GimpTransformTool      *tool,
                                                GimpDisplay            *gdisp);
static void      gimp_transform_tool_recalc    (GimpTransformTool      *tool,
					        GimpDisplay            *gdisp);
static void      gimp_transform_tool_doit      (GimpTransformTool      *tool,
					        GimpDisplay            *gdisp);
static gdouble   gimp_transform_tool_cubic     (gdouble                 dx,
					        gint                    jm1,
					        gint                    j,
					        gint                    jp1,
					        gint                    jp2);
static void    gimp_transform_tool_setup_grid  (GimpTransformTool      *tool);
static void    gimp_transform_tool_grid_recalc (GimpTransformTool      *gimp_transform_tool);
static void    gimp_transform_tool_init        (GimpTransformTool      *tool);
static void    gimp_transform_tool_class_init  (GimpTransformToolClass *tool);

static void    gimp_transform_tool_finalize    (GObject                *object);

static void   gimp_transform_tool_button_press (GimpTool               *tool,
                                                GimpCoords             *coords,
                                                guint32                 time,
                                                GdkModifierType         state,
			                        GimpDisplay            *gdisp);
			          
static void gimp_transform_tool_button_release (GimpTool               *tool,
                                                GimpCoords             *coords,
                                                guint32                 time,
			                        GdkModifierType         state,
			                        GimpDisplay            *gdisp);
			                        
static void    gimp_transform_tool_motion      (GimpTool               *tool,
                                                GimpCoords             *coords,
                                                guint32                 time,
		                                GdkModifierType         state,
		                                GimpDisplay            *gdisp);
		                                
static void  gimp_transform_tool_cursor_update (GimpTool               *tool,
                                                GimpCoords             *coords,
	                 		        GdkModifierType         state,
			                        GimpDisplay            *gdisp);
			                        
static void    gimp_transform_tool_control     (GimpTool               *tool,
			                        ToolAction              action,
			                        GimpDisplay            *gdisp);

static void    gimp_transform_tool_draw        (GimpDrawTool           *draw_tool);


/*  variables  */
static TranInfo           old_trans_info;
InfoDialog               *transform_info        = NULL;
static gboolean           transform_info_inited = FALSE;

static GimpDrawToolClass *parent_class = NULL;

static guint   gimp_transform_tool_signals[LAST_SIGNAL] = { 0 };


GType
gimp_transform_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpTransformToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_transform_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpTransformTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_transform_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_DRAW_TOOL,
					  "GimpTransformTool", 
                                          &tool_info, 0);
    }

  return tool_type;
}

static void
gimp_transform_tool_class_init (GimpTransformToolClass *klass)
{
  GObjectClass      *object_class;
  GimpToolClass     *tool_class;
  GimpDrawToolClass *draw_class;

  object_class = G_OBJECT_CLASS (klass);
  tool_class   = GIMP_TOOL_CLASS (klass);
  draw_class   = GIMP_DRAW_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  gimp_transform_tool_signals[TRANSFORM] =
    g_signal_new ("transform",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GimpTransformToolClass, transform),
		  NULL, NULL,
		  gimp_cclosure_marshal_POINTER__POINTER_INT,
		  G_TYPE_POINTER, 2,
		  G_TYPE_POINTER,
		  G_TYPE_INT);

  object_class->finalize     = gimp_transform_tool_finalize;

  tool_class->control        = gimp_transform_tool_control;
  tool_class->button_press   = gimp_transform_tool_button_press;
  tool_class->button_release = gimp_transform_tool_button_release;
  tool_class->motion         = gimp_transform_tool_motion;
  tool_class->cursor_update  = gimp_transform_tool_cursor_update;

  draw_class->draw           = gimp_transform_tool_draw;
}

static void
gimp_transform_tool_init (GimpTransformTool *tr_tool)
{
  GimpTool *tool = GIMP_TOOL (tr_tool);
  gint      i;

  tr_tool->function = TRANSFORM_CREATING;
  tr_tool->original = NULL;

  for (i = 0; i < TRAN_INFO_SIZE; i++)
    tr_tool->trans_info[i] = 0;

  tr_tool->grid_coords = tr_tool->tgrid_coords = NULL;

  /* FIXME */
  tr_tool->interactive = TRUE;

  tool->scroll_lock = TRUE;   /*  Disallow scrolling  */
  tool->preserve    = FALSE;  /*  Don't preserve on drawable change  */

}

TileManager *
gimp_transform_tool_transform (GimpTransformTool   *tool,
                               GimpDisplay         *gdisp,
			       TransformState       state)
{
  TileManager *retval;

  g_return_val_if_fail (tool, NULL);
  g_return_val_if_fail (GIMP_IS_TRANSFORM_TOOL (tool), NULL);

  g_signal_emit (G_OBJECT (tool), gimp_transform_tool_signals[TRANSFORM], 0,
                 gdisp, state, &retval);

  return retval;
}

static void
transform_ok_callback (GtkWidget *widget,
		       gpointer   data)
{
  GimpTool *tool;

  tool = GIMP_TOOL(data);
  gimp_transform_tool_doit (GIMP_TRANSFORM_TOOL(tool), tool->gdisp);
}

static void
transform_reset_callback (GtkWidget *widget,
			  gpointer   data)
{
  GimpTransformTool   *tool;
  GimpDrawTool        *dr_tool;
  gint                 i;

  tool    = GIMP_TRANSFORM_TOOL (data);
  dr_tool = GIMP_DRAW_TOOL (data);

  /*  stop the current tool drawing process  */
  gimp_draw_tool_pause (dr_tool);

  /*  Restore the previous transformation info  */
  for (i = 0; i < TRAN_INFO_SIZE; i++)
    tool->trans_info [i] = old_trans_info [i];

  /*  recalculate the tool's transformation matrix  */
  gimp_transform_tool_recalc (tool, GIMP_TOOL(tool)->gdisp);

  /*  resume drawing the current tool  */
  gimp_draw_tool_resume (dr_tool);
}

static void
gimp_transform_tool_finalize (GObject *object)
{
  GimpTransformTool *tr_tool;

  tr_tool = GIMP_TRANSFORM_TOOL (object);

  if (tr_tool->original)
    {
      tile_manager_destroy (tr_tool->original);
      tr_tool->original = NULL;
    }

  if (transform_info)
    {
      info_dialog_free (transform_info);
      transform_info        = NULL;
      transform_info_inited = FALSE;
    }

  if (tr_tool->grid_coords)
    {
      g_free (tr_tool->grid_coords);
      tr_tool->grid_coords = NULL;
    }

  if (tr_tool->tgrid_coords)
    {
      g_free (tr_tool->tgrid_coords);
      tr_tool->tgrid_coords = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_transform_tool_control (GimpTool    *tool,
			     ToolAction   action,
			     GimpDisplay *gdisp)
{
  GimpDrawTool      *dr_tool;
  GimpTransformTool *tr_tool;

  dr_tool = GIMP_DRAW_TOOL (tool);
  tr_tool = GIMP_TRANSFORM_TOOL (tool);

  switch (action)
    {
    case PAUSE:
      break;

    case RESUME:
      gimp_transform_tool_recalc (tr_tool, gdisp);
      break;

    case HALT:
      gimp_transform_tool_reset (tr_tool, gdisp);
      break;

    default:
      break;
    }

  if (GIMP_TOOL_CLASS (parent_class)->control)
    GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static void
gimp_transform_tool_button_press (GimpTool        *tool,
                                  GimpCoords      *coords,
                                  guint32          time,
                                  GdkModifierType  state,
			          GimpDisplay     *gdisp)
{
  GimpTransformTool *gt_tool;
  GimpDrawTool      *draw_tool;
  GimpDisplayShell  *shell;
  GimpDrawable      *drawable;
  gdouble            dist;
  gdouble            closest_dist;
  gint               i;
  gint               off_x, off_y;

  gt_tool   = GIMP_TRANSFORM_TOOL (tool);
  draw_tool = GIMP_DRAW_TOOL (tool);

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  drawable = gimp_image_active_drawable (gdisp->gimage);

  if (gt_tool->function == TRANSFORM_CREATING && tool->state == ACTIVE)
    {
      /*  Save the current transformation info  */
      for (i = 0; i < TRAN_INFO_SIZE; i++)
	old_trans_info [i] = gt_tool->trans_info [i];
    }

  /*  if we have already displayed the bounding box and handles,
   *  check to make sure that the display which currently owns the
   *  tool is the one which just received the button pressed event
   */
  if ((gdisp == tool->gdisp) && gt_tool->interactive)
    {
      /*  start drawing the bounding box and handles...  */
      gimp_draw_tool_start (GIMP_DRAW_TOOL (tool), shell->canvas->window);

      closest_dist = gimp_draw_tool_calc_distance (draw_tool, gdisp,
                                                   coords->x, coords->y,
                                                   gt_tool->tx1, gt_tool->ty1);
      gt_tool->function = TRANSFORM_HANDLE_1;

      dist = gimp_draw_tool_calc_distance (draw_tool, gdisp,
                                           coords->x, coords->y,
                                           gt_tool->tx2, gt_tool->ty2);
      if (dist < closest_dist)
	{
	  closest_dist = dist;
	  gt_tool->function = TRANSFORM_HANDLE_2;
	}

      dist = gimp_draw_tool_calc_distance (draw_tool, gdisp,
                                           coords->x, coords->y,
                                           gt_tool->tx3, gt_tool->ty3);
      if (dist < closest_dist)
	{
	  closest_dist = dist;
	  gt_tool->function = TRANSFORM_HANDLE_3;
	}

      dist = gimp_draw_tool_calc_distance (draw_tool, gdisp,
                                           coords->x, coords->y,
                                           gt_tool->tx4, gt_tool->ty4);
      if (dist < closest_dist)
	{
	  closest_dist = dist;
	  gt_tool->function = TRANSFORM_HANDLE_4;
	}

      if (gimp_draw_tool_in_radius (draw_tool, gdisp,
                                    coords->x, coords->y,
                                    gt_tool->tcx, gt_tool->tcy,
                                    HANDLE >> 1))
	{
	  gt_tool->function = TRANSFORM_HANDLE_CENTER;
	}

      /*  Save the current pointer position  */
      gt_tool->lastx = gt_tool->startx = coords->x;
      gt_tool->lasty = gt_tool->starty = coords->y;

      gdk_pointer_grab (shell->canvas->window, FALSE,
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON1_MOTION_MASK |
			GDK_BUTTON_RELEASE_MASK,
			NULL, NULL, time);

      tool->state = ACTIVE;
      return;
    }

  /*  Initialisation stuff: if the cursor is clicked inside the current
   *  selection, show the bounding box and handles...
   */
  gimp_drawable_offsets (drawable, &off_x, &off_y);

  if (coords->x >= off_x &&
      coords->y >= off_y &&
      coords->x < (off_x + gimp_drawable_width (drawable)) &&
      coords->y < (off_y + gimp_drawable_height (drawable)))
    {
      if (gimage_mask_is_empty (gdisp->gimage) ||
	  gimage_mask_value (gdisp->gimage, coords->x, coords->y))
	{
	  if (GIMP_IS_LAYER (drawable) &&
	      gimp_layer_get_mask (GIMP_LAYER (drawable)))
	    {
	      g_message (_("Transformations do not work on\n"
			   "layers that contain layer masks."));
	      tool->state = INACTIVE;
	      return;
	    }

	  /*  If the tool is already active, clear the current state
	   *  and reset
	   */
	  if (tool->state == ACTIVE)
	    gimp_transform_tool_reset (gt_tool, gdisp);

	  /*  Set the pointer to the active display  */
	  tool->gdisp    = gdisp;
	  tool->drawable = drawable;
	  tool->state    = ACTIVE;

	  /*  Grab the pointer if we're in non-interactive mode  */
	  if (! gt_tool->interactive)
	    gdk_pointer_grab (shell->canvas->window, FALSE,
			      GDK_POINTER_MOTION_HINT_MASK |
                              GDK_BUTTON1_MOTION_MASK |
                              GDK_BUTTON_RELEASE_MASK,
			      NULL, NULL, time);

	  /*  Find the transform bounds for some tools (like scale,
	   *  perspective) that actually need the bounds for
	   *  initializing
	   */
	  gimp_transform_tool_bounds (gt_tool, gdisp);

	  /*  Initialize the transform tool */
	  gimp_transform_tool_transform (gt_tool, gdisp, TRANSFORM_INIT);

	  if (transform_info && !transform_info_inited)
	    {
	      GType tool_type;

	      tool_type =
		gimp_context_get_tool (gimp_get_user_context (gdisp->gimage->gimp))->tool_type;

	      gimp_dialog_create_action_area
		(GIMP_DIALOG (transform_info->shell),

		 /* FIXME: this does not belong here */
		 (tool_type == GIMP_TYPE_ROTATE_TOOL)      ? _("Rotate")    :
		 (tool_type == GIMP_TYPE_SCALE_TOOL)       ? _("Scale")     :
		 (tool_type == GIMP_TYPE_SHEAR_TOOL)       ? _("Shear")     :
		 (tool_type == GIMP_TYPE_PERSPECTIVE_TOOL) ? _("Transform") :
		 "EEK",
		 transform_ok_callback,
		 tool, NULL, NULL, TRUE, FALSE,

		 _("Reset"), transform_reset_callback,
		 tool, NULL, NULL, FALSE, FALSE,

		 NULL);

	      transform_info_inited = TRUE;
	    }

	  /*  Recalculate the transform tool  */
	  gimp_transform_tool_recalc (gt_tool, gdisp);

	  /*  recall this function to find which handle we're dragging  */
	  if (gt_tool->interactive)
	    gimp_transform_tool_button_press (tool, coords, time, state, gdisp);
	}
    }
}

static void
gimp_transform_tool_button_release (GimpTool        *tool,
                                    GimpCoords      *coords,
                                    guint32          time,
			            GdkModifierType  state,
			            GimpDisplay     *gdisp)
{
  GimpTransformTool *gt_tool;
  gint               i;

  gt_tool = GIMP_TRANSFORM_TOOL (tool);

  /*  if we are creating, there is nothing to be done...exit  */
  if (gt_tool->function == TRANSFORM_CREATING && gt_tool->interactive)
    return;

  /*  release of the pointer grab  */
  gdk_pointer_ungrab (time);
  gdk_flush ();

  /*  if the 3rd button isn't pressed, transform the selected mask  */
  if (! (state & GDK_BUTTON3_MASK))
    {
      /* Shift-clicking is another way to approve the transform  */
      if ((state & GDK_SHIFT_MASK) || GIMP_IS_FLIP_TOOL (tool))
	{
	  gimp_transform_tool_doit (gt_tool, gdisp);
	}
      else
	{
	  /*  Only update the paths preview */
	  path_transform_current_path (gdisp->gimage,
				       gt_tool->transform, TRUE);
	}
    }
  else
    {
      /*  stop the current tool drawing process  */
      gimp_draw_tool_pause (GIMP_DRAW_TOOL (tool));

      /*  Restore the previous transformation info  */
      for (i = 0; i < TRAN_INFO_SIZE; i++)
	gt_tool->trans_info [i] = old_trans_info [i];

      /*  recalculate the tool's transformation matrix  */
      gimp_transform_tool_recalc (gt_tool, gdisp);

      /*  resume drawing the current tool  */
      gimp_draw_tool_resume (GIMP_DRAW_TOOL (tool));

      /* Update the paths preview */
      path_transform_current_path (gdisp->gimage,
				   gt_tool->transform, TRUE);
    }

  /*  if this tool is non-interactive, make it inactive after use  */
  if (! gt_tool->interactive)
    tool->state = INACTIVE;
}

static void
gimp_transform_tool_doit (GimpTransformTool  *gt_tool,
		          GimpDisplay        *gdisp)
{
  GimpDisplayShell *shell;
  GimpTool         *tool;
  TileManager      *new_tiles;
  TransformUndo    *tu;
  PathUndo         *pundo;
  gboolean          new_layer;
  gint              i;

  gimp_set_busy (gdisp->gimage->gimp);

  tool = GIMP_TOOL (gt_tool);

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  /* undraw the tool before we muck around with the transform matrix */
  gimp_draw_tool_pause (GIMP_DRAW_TOOL (gt_tool));

  /*  We're going to dirty this image, but we want to keep the tool
   *  around
   */
  tool->preserve = TRUE;

  /*  Start a transform undo group  */
  undo_push_group_start (gdisp->gimage, TRANSFORM_CORE_UNDO);

  /* With the old UI, if original is NULL, then this is the
   * first transformation. In the new UI, it is always so, right?
   */
  g_assert (gt_tool->original == NULL);

  /* If we're in interactive mode, we need to copy the current
   *  selection to the transform tool's private selection pointer, so
   *  that the original source can be repeatedly modified.
   */
  tool->drawable = gimp_image_active_drawable (gdisp->gimage);

  gt_tool->original = gimp_transform_tool_cut (gdisp->gimage,
					       tool->drawable,
					       &new_layer);

  pundo = path_transform_start_undo (gdisp->gimage);

  /*  Send the request for the transformation to the tool...
   */
  new_tiles = gimp_transform_tool_transform (gt_tool, gdisp,
					     TRANSFORM_FINISH);

  gimp_transform_tool_transform (gt_tool, gdisp, TRANSFORM_INIT);

  gimp_transform_tool_recalc (gt_tool, gdisp);

  if (new_tiles)
    {
      /*  paste the new transformed image to the gimage...also implement
       *  undo...
       */
      /*  FIXME: we should check if the drawable is still valid  */
      gimp_transform_tool_paste (gdisp->gimage, tool->drawable,
				 new_tiles, new_layer);

      /*  create and initialize the transform_undo structure  */
      tu = g_new0 (TransformUndo, 1);
      tu->tool_ID   = tool->ID;
      tu->tool_type = G_TYPE_FROM_INSTANCE (tool);

      for (i = 0; i < TRAN_INFO_SIZE; i++)
	tu->trans_info[i] = old_trans_info[i];

      tu->original  = NULL;
      tu->path_undo = pundo;

      /*  Make a note of the new current drawable (since we may have
       *  a floating selection, etc now.
       */
      tool->drawable = gimp_image_active_drawable (gdisp->gimage);

      undo_push_transform (gdisp->gimage, tu);
    }

  /*  push the undo group end  */
  undo_push_group_end (gdisp->gimage);

  /*  We're done dirtying the image, and would like to be restarted
   *  if the image gets dirty while the tool exists
   */
  tool->preserve = FALSE;

#ifdef __GNUC__
#warning FIXME: investigate why display update was done here
#endif
#if 0
  /*  Flush the gdisplays  */
  if (gdisp->disp_xoffset || gdisp->disp_yoffset)
    {
      gint x, y;

      x = shell->disp_width;
      y = shell->disp_height;

      if (gdisp->disp_yoffset)
	{
	  gimp_display_shell_add_expose_area (shell,
                                              0, 0,
                                              gdisp->disp_width,
                                              gdisp->disp_yoffset);
	  gimp_display_shell_add_expose_area (shell,
                                              0, gdisp->disp_yoffset + y,
                                              gdisp->disp_width, gdisp->disp_height);
	}

      if (gdisp->disp_xoffset)
	{
	  gimp_display_shell_add_expose_area (shell,
                                              0, 0,
                                              gdisp->disp_xoffset, gdisp->disp_height);
	  gimp_display_shell_add_expose_area (shell,
                                              gdisp->disp_xoffset + x, 0,
                                              gdisp->disp_width, gdisp->disp_height);
	}
    }
#endif

  gimp_unset_busy (gdisp->gimage->gimp);

  gdisplays_flush ();

  gimp_transform_tool_reset (gt_tool, gdisp);

  /*  if this tool is non-interactive, make it inactive after use  */
  if (!gt_tool->interactive)
    tool->state = INACTIVE;
}

static void
gimp_transform_tool_motion (GimpTool        *tool,
                            GimpCoords      *coords,
                            guint32          time,
		            GdkModifierType  state,
		            GimpDisplay     *gdisp)
{
  GimpTransformTool *tr_tool;

  tr_tool = GIMP_TRANSFORM_TOOL (tool);

  /*  if we are creating or this tool is non-interactive, there is
   *  nothing to be done so exit.
   */
  if (tr_tool->function == TRANSFORM_CREATING || ! tr_tool->interactive)
    return;

  gimp_draw_tool_pause (GIMP_DRAW_TOOL (tool));

  tr_tool->curx  = coords->x;
  tr_tool->cury  = coords->y;
  tr_tool->state = state;

  /*  recalculate the tool's transformation matrix  */
  gimp_transform_tool_transform (tr_tool, gdisp, TRANSFORM_MOTION);

  tr_tool->lastx = tr_tool->curx;
  tr_tool->lasty = tr_tool->cury;

  gimp_draw_tool_resume (GIMP_DRAW_TOOL (tool));
}

static void
gimp_transform_tool_cursor_update (GimpTool        *tool,
                                   GimpCoords      *coords,
			           GdkModifierType  state,
			           GimpDisplay     *gdisp)
{
  GimpTransformTool *tr_tool;
  GimpDrawable      *drawable;
  GdkCursorType      ctype = GDK_TOP_LEFT_ARROW;

  tr_tool = GIMP_TRANSFORM_TOOL (tool);

  if ((drawable = gimp_image_active_drawable (gdisp->gimage)))
    {
      gint off_x, off_y;

      gimp_drawable_offsets (drawable, &off_x, &off_y);

      if (GIMP_IS_LAYER (drawable) &&
          gimp_layer_get_mask (GIMP_LAYER (drawable)))
	{
	  ctype = GIMP_BAD_CURSOR;
	}
      else if (coords->x >= off_x &&
	       coords->y >= off_y &&
	       coords->x < (off_x + drawable->width) &&
	       coords->y < (off_y + drawable->height))
	{
	  if (gimage_mask_is_empty (gdisp->gimage) ||
	      gimage_mask_value (gdisp->gimage, coords->x, coords->y))
	    {
	      ctype = GIMP_MOUSE_CURSOR;
	    }
	}
    }

  gimp_display_shell_install_tool_cursor (GIMP_DISPLAY_SHELL (gdisp->shell),
                                          ctype,
                                          tool->tool_cursor,
                                          GIMP_CURSOR_MODIFIER_NONE);
}

static void
gimp_transform_tool_draw (GimpDrawTool *draw_tool)
{
  GimpTransformTool *tr_tool;
  gint               i, k, gci;

  tr_tool = GIMP_TRANSFORM_TOOL (draw_tool);

  /*  draw the bounding box  */
  gimp_draw_tool_draw_line (draw_tool,
                            tr_tool->tx1, tr_tool->ty1,
                            tr_tool->tx2, tr_tool->ty2,
                            FALSE);
  gimp_draw_tool_draw_line (draw_tool,
                            tr_tool->tx2, tr_tool->ty2,
                            tr_tool->tx4, tr_tool->ty4,
                            FALSE);
  gimp_draw_tool_draw_line (draw_tool,
                            tr_tool->tx3, tr_tool->ty3,
                            tr_tool->tx4, tr_tool->ty4,
                            FALSE);
  gimp_draw_tool_draw_line (draw_tool,
                            tr_tool->tx3, tr_tool->ty3,
                            tr_tool->tx1, tr_tool->ty1,
                            FALSE);

  /*  Draw the grid */

  if ((tr_tool->grid_coords != NULL) &&
      (tr_tool->tgrid_coords != NULL) /* FIXME!!! this doesn't belong here &&
      ((tool->type != PERSPECTIVE)  ||
       ((tr_tool->transform[0][0] >=0.0) &&
	(tr_tool->transform[1][1] >=0.0)) */ ) 
    {
      gci = 0;
      k = tr_tool->ngx + tr_tool->ngy;

      for (i = 0; i < k; i++)
	{
          gimp_draw_tool_draw_line (draw_tool,
                                    tr_tool->tgrid_coords[gci],
                                    tr_tool->tgrid_coords[gci + 1],
                                    tr_tool->tgrid_coords[gci + 2],
                                    tr_tool->tgrid_coords[gci + 3],
                                    FALSE);
	  gci += 4;
	}
    }

  /*  draw the tool handles  */
  gimp_draw_tool_draw_rectangle_by_center (draw_tool, FALSE,
                                           tr_tool->tx1, tr_tool->ty1,
                                           HANDLE, HANDLE,
                                           FALSE);
  gimp_draw_tool_draw_rectangle_by_center (draw_tool, FALSE,
                                           tr_tool->tx2, tr_tool->ty2,
                                           HANDLE, HANDLE,
                                           FALSE);
  gimp_draw_tool_draw_rectangle_by_center (draw_tool, FALSE,
                                           tr_tool->tx3, tr_tool->ty3,
                                           HANDLE, HANDLE,
                                           FALSE);
  gimp_draw_tool_draw_rectangle_by_center (draw_tool, FALSE,
                                           tr_tool->tx4, tr_tool->ty4,
                                           HANDLE, HANDLE,
                                           FALSE);

  /*  draw the center  */
  gimp_draw_tool_draw_arc_by_center (draw_tool, TRUE,
                                     tr_tool->tcx, tr_tool->tcy,
                                     HANDLE >> 1,
                                     0, 23040,
                                     FALSE);

  if (gimp_transform_tool_showpath ())
    {
      GimpMatrix3 tmp_matrix;

      if (gimp_transform_tool_direction () == TRANSFORM_CORRECTIVE)
	{
	  gimp_matrix3_invert (tr_tool->transform, tmp_matrix);
	}
      else
	{
	  gimp_matrix3_duplicate (tr_tool->transform, tmp_matrix);
	}

      path_transform_draw_current (GIMP_TOOL (draw_tool)->gdisp,
                                   draw_tool, tmp_matrix);
    }
}

void
gimp_transform_tool_transform_bounding_box (GimpTransformTool *tr_tool)
{
  GimpTool  *tool;
  gint       i, k;
  gint       gci;

  tool = GIMP_TOOL (tr_tool);

  gimp_matrix3_transform_point (tr_tool->transform,
				tr_tool->x1, tr_tool->y1,
				&tr_tool->tx1, &tr_tool->ty1);
  gimp_matrix3_transform_point (tr_tool->transform,
				tr_tool->x2, tr_tool->y1,
				&tr_tool->tx2, &tr_tool->ty2);
  gimp_matrix3_transform_point (tr_tool->transform,
				tr_tool->x1, tr_tool->y2,
				&tr_tool->tx3, &tr_tool->ty3);
  gimp_matrix3_transform_point (tr_tool->transform,
				tr_tool->x2, tr_tool->y2,
				&tr_tool->tx4, &tr_tool->ty4);

  gimp_matrix3_transform_point (tr_tool->transform,
				tr_tool->cx, tr_tool->cy,
				&tr_tool->tcx, &tr_tool->tcy);

  if (tr_tool->grid_coords != NULL &&
      tr_tool->tgrid_coords != NULL)
    {
      gci = 0;
      k  = (tr_tool->ngx + tr_tool->ngy) * 2;

      for (i = 0; i < k; i++)
	{
	  gimp_matrix3_transform_point (tr_tool->transform,
					tr_tool->grid_coords[gci],
					tr_tool->grid_coords[gci+1],
					&(tr_tool->tgrid_coords[gci]),
					&(tr_tool->tgrid_coords[gci+1]));
	  gci += 2;
	}
    }
}

void
gimp_transform_tool_reset (GimpTransformTool *tr_tool,
		           GimpDisplay       *gdisp)
{
  GimpTool *tool;

  tool = GIMP_TOOL (tr_tool);

  if (tr_tool->original)
    {
      tile_manager_destroy (tr_tool->original);
      tr_tool->original = NULL;
    }

  /*  inactivate the tool  */
  tr_tool->function = TRANSFORM_CREATING;
  gimp_draw_tool_stop (GIMP_DRAW_TOOL (tr_tool));
  info_dialog_popdown (transform_info);

  tool->state    = INACTIVE;
  tool->gdisp    = NULL;
  tool->drawable = NULL;
}

static void
gimp_transform_tool_bounds (GimpTransformTool *tr_tool,
		            GimpDisplay       *gdisp)
{
  TileManager  *tiles;
  GimpDrawable *drawable;
  gint          offset_x, offset_y;

  tiles    = tr_tool->original;
  drawable = gimp_image_active_drawable (gdisp->gimage);

  /*  find the boundaries  */
  if (tiles)
    {
      tile_manager_get_offsets (tiles,
				&tr_tool->x1, &tr_tool->y1);
				
      tr_tool->x2 = tr_tool->x1 + tile_manager_width (tiles);
      tr_tool->y2 = tr_tool->y1 + tile_manager_height (tiles);
    }
  else
    {
      gimp_drawable_offsets (drawable, &offset_x, &offset_y);
      gimp_drawable_mask_bounds (drawable,
				 &tr_tool->x1, &tr_tool->y1,
				 &tr_tool->x2, &tr_tool->y2);
      tr_tool->x1 += offset_x;
      tr_tool->y1 += offset_y;
      tr_tool->x2 += offset_x;
      tr_tool->y2 += offset_y;
    }

  tr_tool->cx = (tr_tool->x1 + tr_tool->x2) / 2;
  tr_tool->cy = (tr_tool->y1 + tr_tool->y2) / 2;

  /*  changing the bounds invalidates any grid we may have  */
  gimp_transform_tool_grid_recalc (tr_tool);
}

void
gimp_transform_tool_grid_density_changed (void)
{
  GimpTransformTool *tr_tool;
  GimpDrawTool      *dr_tool;

  /* EEEK!!! */ 
  tr_tool = GIMP_TRANSFORM_TOOL (tool_manager_get_active (the_gimp));

  dr_tool = GIMP_DRAW_TOOL (tr_tool);

  if (tr_tool->function == TRANSFORM_CREATING)
    return;

  gimp_draw_tool_pause (dr_tool);

  gimp_transform_tool_grid_recalc (tr_tool);
  gimp_transform_tool_transform_bounding_box (tr_tool);

  gimp_draw_tool_resume (dr_tool);
}

void
gimp_transform_tool_showpath_changed (gint type /* a truly undescriptive name */)
{
  GimpTransformTool *tr_tool;

  /* EEEEEEEK!!! */ 
  tr_tool = GIMP_TRANSFORM_TOOL (tool_manager_get_active (the_gimp));

  if (tr_tool->function == TRANSFORM_CREATING)
    return;

  if (type)
    gimp_draw_tool_pause (GIMP_DRAW_TOOL (tr_tool));
  else
    gimp_draw_tool_resume (GIMP_DRAW_TOOL (tr_tool));
}

static void
gimp_transform_tool_grid_recalc (GimpTransformTool *tr_tool)
{
  if (tr_tool->grid_coords != NULL)
    {
      g_free (tr_tool->grid_coords);
      tr_tool->grid_coords = NULL;
    }

  if (tr_tool->tgrid_coords != NULL)
    {
      g_free (tr_tool->tgrid_coords);
      tr_tool->tgrid_coords = NULL;
    }

  if (gimp_transform_tool_show_grid ())
    gimp_transform_tool_setup_grid (tr_tool);
}

static void
gimp_transform_tool_setup_grid (GimpTransformTool *tr_tool)
{
  GimpTool *tool;
  gint      i, gci;
  gdouble  *coords;

  tool = GIMP_TOOL (tr_tool);

  /*  We use the gimp_transform_tool_grid_size function only here, even
   *  if the user changes the grid size in the middle of an
   *  operation, nothing happens.
   */
  tr_tool->ngx = (tr_tool->x2 - tr_tool->x1) / gimp_transform_tool_grid_size ();
  if (tr_tool->ngx > 0)
    tr_tool->ngx--;

  tr_tool->ngy = (tr_tool->y2 - tr_tool->y1) / gimp_transform_tool_grid_size ();
  if (tr_tool->ngy > 0)
    tr_tool->ngy--;

  tr_tool->grid_coords = coords =
    g_new (gdouble, (tr_tool->ngx + tr_tool->ngy) * 4);

  tr_tool->tgrid_coords =
    g_new (gdouble, (tr_tool->ngx + tr_tool->ngy) * 4);

  gci = 0;

  for (i = 1; i <= tr_tool->ngx; i++)
    {
      coords[gci] = tr_tool->x1 +
	((gdouble) i) / (tr_tool->ngx + 1) *
	(tr_tool->x2 - tr_tool->x1);
      coords[gci+1] = tr_tool->y1;
      coords[gci+2] = coords[gci];
      coords[gci+3] = tr_tool->y2;
      gci += 4;
    }

  for (i = 1; i <= tr_tool->ngy; i++)
    {
      coords[gci] = tr_tool->x1;
      coords[gci+1] = tr_tool->y1 +
	((gdouble) i) / (tr_tool->ngy + 1) *
	(tr_tool->y2 - tr_tool->y1);
      coords[gci+2] = tr_tool->x2;
      coords[gci+3] = coords[gci+1];
      gci += 4;
    }
}

static void
gimp_transform_tool_recalc (GimpTransformTool *tr_tool,
		            GimpDisplay       *gdisp)
{
  gimp_transform_tool_bounds (tr_tool, gdisp);

  gimp_transform_tool_transform (tr_tool, gdisp, TRANSFORM_RECALC);
}

/*  Actually carry out a transformation  */
TileManager *
gimp_transform_tool_do (GimpImage        *gimage,
                        GimpDrawable     *drawable,
                        TileManager      *float_tiles,
                        gboolean          interpolation,
                        GimpMatrix3       matrix,
                        GimpProgressFunc  progress_callback,
                        gpointer          progress_data)
{
  PixelRegion  destPR;
  TileManager *tiles;
  GimpMatrix3  m;
  GimpMatrix3  im;
  gint         itx, ity;
  gint         tx1, ty1, tx2, ty2;
  gint         width, height;
  gint         alpha;
  gint         bytes, b;
  gint         x, y;
  gint         sx, sy;
  gint         x1, y1, x2, y2;
  gdouble      xinc, yinc, winc;
  gdouble      tx, ty, tw;
  gdouble      ttx = 0.0, tty = 0.0;
  guchar      *dest;
  guchar      *d;
  guchar      *src[16];
  Tile        *tile[16];
  guchar       bg_col[MAX_CHANNELS];
  gint         i;
  gdouble      a_val, a_recip;
  gint         newval;

  PixelSurround surround;

  alpha = 0;

  /*  turn interpolation off for simple transformations (e.g. rot90)  */
  if (gimp_matrix3_is_simple (matrix) ||
      base_config->interpolation_type == NEAREST_NEIGHBOR_INTERPOLATION)
    interpolation = FALSE;

  /*  Get the background color  */
  gimp_image_get_background (gimage, drawable, bg_col);

  switch (gimp_drawable_type (drawable))
    {
    case RGB_GIMAGE: case RGBA_GIMAGE:
      bg_col[ALPHA_PIX] = TRANSPARENT_OPACITY;
      alpha = ALPHA_PIX;
      break;
    case GRAY_GIMAGE: case GRAYA_GIMAGE:
      bg_col[ALPHA_G_PIX] = TRANSPARENT_OPACITY;
      alpha = ALPHA_G_PIX;
      break;
    case INDEXED_GIMAGE: case INDEXEDA_GIMAGE:
      bg_col[ALPHA_I_PIX] = TRANSPARENT_OPACITY;
      alpha = ALPHA_I_PIX;
      /*  If the gimage is indexed color, ignore smoothing value  */
      interpolation = FALSE;
      break;
    default:
      g_assert_not_reached ();
      break;
    }

  /*  enable rotating un-floated non-layers  */
  if (tile_manager_bpp (float_tiles) == 1)
    {
      bg_col[0] = OPAQUE_OPACITY;

      /*  setting alpha = 0 will cause the channel's value to be treated
       *  as alpha and the color channel loops never to be entered
       */
      alpha = 0;
    }

  if (gimp_transform_tool_direction () == TRANSFORM_CORRECTIVE)
    {
      /*  keep the original matrix here, so we dont need to recalculate
	  the inverse later  */
      gimp_matrix3_duplicate (matrix, m);
      gimp_matrix3_invert (matrix, im);
      matrix = im;
    }
  else
    {
      /*  Find the inverse of the transformation matrix  */
      gimp_matrix3_invert (matrix, m);
    }

  path_transform_current_path (gimage, matrix, FALSE);

  tile_manager_get_offsets (float_tiles, &x1, &y1);
  x2 = x1 + tile_manager_width (float_tiles);
  y2 = y1 + tile_manager_height (float_tiles);

  /*  Find the bounding coordinates  */
  if (alpha == 0 || (tool_manager_get_active (gimage->gimp) &&
		     gimp_transform_tool_clip ()))
    {
      tx1 = x1;
      ty1 = y1;
      tx2 = x2;
      ty2 = y2;
    }
  else
    {
      gdouble dx1, dy1;
      gdouble dx2, dy2;
      gdouble dx3, dy3;
      gdouble dx4, dy4;

      gimp_matrix3_transform_point (matrix, x1, y1, &dx1, &dy1);
      gimp_matrix3_transform_point (matrix, x2, y1, &dx2, &dy2);
      gimp_matrix3_transform_point (matrix, x1, y2, &dx3, &dy3);
      gimp_matrix3_transform_point (matrix, x2, y2, &dx4, &dy4);

      tx1 = MIN (dx1, dx2);
      tx1 = MIN (tx1, dx3);
      tx1 = MIN (tx1, dx4);
      ty1 = MIN (dy1, dy2);
      ty1 = MIN (ty1, dy3);
      ty1 = MIN (ty1, dy4);

      tx2 = MAX (dx1, dx2);
      tx2 = MAX (tx2, dx3);
      tx2 = MAX (tx2, dx4);
      ty2 = MAX (dy1, dy2);
      ty2 = MAX (ty2, dy3);
      ty2 = MAX (ty2, dy4);
    }

  /*  Get the new temporary buffer for the transformed result  */
  tiles = tile_manager_new ((tx2 - tx1), (ty2 - ty1),
			    tile_manager_bpp (float_tiles));
  pixel_region_init (&destPR, tiles, 0, 0, (tx2 - tx1), (ty2 - ty1), TRUE);
  tile_manager_set_offsets (tiles, tx1, ty1);

  /* initialise the pixel_surround accessor */
  if (interpolation)
    {
      if (base_config->interpolation_type == CUBIC_INTERPOLATION)
	{
	  pixel_surround_init (&surround, float_tiles, 4, 4, bg_col);
	}
      else
	{
	  pixel_surround_init (&surround, float_tiles, 2, 2, bg_col);
	}
    }
  else
    {
      /* not actually useful, keeps the code cleaner */
      pixel_surround_init (&surround, float_tiles, 1, 1, bg_col);
    }

  width  = tile_manager_width (tiles);
  height = tile_manager_height (tiles);
  bytes  = tile_manager_bpp (tiles);

  dest = g_new (guchar, width * bytes);

  xinc = m[0][0];
  yinc = m[1][0];
  winc = m[2][0];

  /* these loops could be rearranged, depending on which bit of code
   * you'd most like to write more than once.
   */

  for (y = ty1; y < ty2; y++)
    {
      if (progress_callback && !(y & 0xf))
	(* progress_callback) (ty1, ty2, y, progress_data);

      /* set up inverse transform steps */
      tx = xinc * tx1 + m[0][1] * y + m[0][2];
      ty = yinc * tx1 + m[1][1] * y + m[1][2];
      tw = winc * tx1 + m[2][1] * y + m[2][2];

      d = dest;
      for (x = tx1; x < tx2; x++)
	{
	  /*  normalize homogeneous coords  */
	  if (tw == 0.0)
	    {
	      g_warning ("homogeneous coordinate = 0...\n");
	    }
	  else if (tw != 1.0)
	    {
	      ttx = tx / tw;
	      tty = ty / tw;
	    }
	  else
	    {
	      ttx = tx;
	      tty = ty;
	    }

          /*  Set the destination pixels  */

          if (interpolation)
       	    {
              if (base_config->interpolation_type == CUBIC_INTERPOLATION)
       	        {
                  /*  ttx & tty are the subpixel coordinates of the point in
		   *  the original selection's floating buffer.
		   *  We need the four integer pixel coords around them:
		   *  itx to itx + 3, ity to ity + 3
                   */
                  itx = floor (ttx);
                  ity = floor (tty);

		  /* check if any part of our region overlaps the buffer */

                  if ((itx + 2) >= x1 && (itx - 1) < x2 &&
                      (ity + 2) >= y1 && (ity - 1) < y2 )
                    {
                      guchar  *data;
                      gint     row;
                      gdouble  dx, dy;
                      guchar  *start;

		      /* lock the pixel surround */
                      data = pixel_surround_lock (&surround,
						  itx - 1 - x1, ity - 1 - y1);

                      row = pixel_surround_rowstride (&surround);

                      /* the fractional error */
                      dx = ttx - itx;
                      dy = tty - ity;

		      /* calculate alpha of result */
		      start = &data[alpha];
		      a_val = gimp_transform_tool_cubic
			(dy,
			 CUBIC_ROW (dx, start, bytes),
			 CUBIC_ROW (dx, start + row, bytes),
			 CUBIC_ROW (dx, start + row + row, bytes),
			 CUBIC_ROW (dx, start + row + row + row, bytes));

		      if (a_val <= 0.0)
			{
			  a_recip = 0.0;
			  d[alpha] = 0;
			}
		      else if (a_val > 255.0)
			{
			  a_recip = 1.0 / a_val;
			  d[alpha] = 255;
			}
		      else
			{
			  a_recip = 1.0 / a_val;
			  d[alpha] = RINT(a_val);
			}

		      /*  for colour channels c,
		       *  result = bicubic (c * alpha) / bicubic (alpha)
		       *
		       *  never entered for alpha == 0
		       */
		      for (i = -alpha; i < 0; ++i)
			{
			  start = &data[alpha];
			  newval =
			    RINT (a_recip *
				  gimp_transform_tool_cubic
				  (dy,
				   CUBIC_SCALED_ROW (dx, start, bytes, i),
				   CUBIC_SCALED_ROW (dx, start + row, bytes, i),
				   CUBIC_SCALED_ROW (dx, start + row + row, bytes, i),
				   CUBIC_SCALED_ROW (dx, start + row + row + row, bytes, i)));
			  if (newval <= 0)
			    {
			      *d++ = 0;
			    }
			  else if (newval > 255)
			    {
			      *d++ = 255;
			    }
			  else
			    {
			      *d++ = newval;
			    }
			}

		      /*  alpha already done  */
		      d++;

		      pixel_surround_release (&surround);
		    }
                  else /* not in source range */
                    {
                      /*  increment the destination pointers  */
                      for (b = 0; b < bytes; b++)
                        *d++ = bg_col[b];
                    }
                }

       	      else  /*  linear  */
                {
                  itx = floor (ttx);
                  ity = floor (tty);

		  /*  expand source area to cover interpolation region
		   *  (which runs from itx to itx + 1, same in y)
		   */
                  if ((itx + 1) >= x1 && itx < x2 &&
                      (ity + 1) >= y1 && ity < y2 )
                    {
                      guchar  *data;
                      gint     row;
                      double   dx, dy;
                      guchar  *chan;

		      /* lock the pixel surround */
                      data = pixel_surround_lock (&surround, itx - x1, ity - y1);

                      row = pixel_surround_rowstride (&surround);

                      /* the fractional error */
                      dx = ttx - itx;
                      dy = tty - ity;

		      /* calculate alpha value of result pixel */
		      chan = &data[alpha];
		      a_val = BILINEAR (chan[0], chan[bytes], chan[row],
					chan[row+bytes], dx, dy);
		      if (a_val <= 0.0)
			{
			  a_recip = 0.0;
			  d[alpha] = 0.0;
			}
		      else if (a_val >= 255.0)
			{
			  a_recip = 1.0 / a_val;
			  d[alpha] = 255;
			}
		      else
			{
			  a_recip = 1.0 / a_val;
			  d[alpha] = RINT (a_val);
			}

		      /*  for colour channels c,
		       *  result = bilinear (c * alpha) / bilinear (alpha)
		       *
		       *  never entered for alpha == 0
		       */
		      for (i = -alpha; i < 0; ++i)
			{
			  chan = &data[alpha];
			  newval =
			    RINT (a_recip *
				  BILINEAR (chan[0] * chan[i],
					    chan[bytes] * chan[bytes+i],
					    chan[row] * chan[row+i],
					    chan[row+bytes] * chan[row+bytes+i],
					    dx, dy));
			  if (newval <= 0)
			    {
			      *d++ = 0;
			    }
			  else if (newval > 255)
			    {
			      *d++ = 255;
			    }
			  else
			    {
			      *d++ = newval;
			    }
			}

		      /*  alpha already done  */
		      d++;

                      pixel_surround_release (&surround);
		    }
                  else /* not in source range */
                    {
                      /*  increment the destination pointers  */
                      for (b = 0; b < bytes; b++)
                        *d++ = bg_col[b];
                    }
		}
	    }
          else  /*  no interpolation  */
            {
              itx = floor (ttx);
              ity = floor (tty);

              if (itx >= x1 && itx < x2 &&
                  ity >= y1 && ity < y2 )
                {
                  /*  x, y coordinates into source tiles  */
                  sx = itx - x1;
                  sy = ity - y1;

                  REF_TILE (0, sx, sy);

                  for (b = 0; b < bytes; b++)
                    *d++ = src[0][b];

                  tile_release (tile[0], FALSE);
		}
              else /* not in source range */
                {
                  /*  increment the destination pointers  */
                  for (b = 0; b < bytes; b++)
                    *d++ = bg_col[b];
                }
	    }

	  /*  increment the transformed coordinates  */
	  tx += xinc;
	  ty += yinc;
	  tw += winc;
	}

      /*  set the pixel region row  */
      pixel_region_set_row (&destPR, 0, (y - ty1), width, dest);
    }

  pixel_surround_clear (&surround);

  g_free (dest);

  return tiles;
}

TileManager *
gimp_transform_tool_cut (GimpImage    *gimage,
		         GimpDrawable *drawable,
		         gboolean     *new_layer)
{
  TileManager *tiles;

  /*  extract the selected mask if there is a selection  */
  if (! gimage_mask_is_empty (gimage))
    {
      /* set the keep_indexed flag to FALSE here, since we use
	 gimp_layer_new_from_tiles() later which assumes that the tiles
	 are either RGB or GRAY.  Eeek!!!              (Sven)
       */
      tiles = gimage_mask_extract (gimage, drawable, TRUE, FALSE, TRUE);
      *new_layer = TRUE;
    }
  /*  otherwise, just copy the layer  */
  else
    {
      if (GIMP_IS_LAYER (drawable))
	tiles = gimage_mask_extract (gimage, drawable, FALSE, TRUE, TRUE);
      else
	tiles = gimage_mask_extract (gimage, drawable, FALSE, TRUE, FALSE);
      *new_layer = FALSE;
    }

  return tiles;
}


/*  Paste a transform to the gdisplay  */
gboolean
gimp_transform_tool_paste (GimpImage    *gimage,
		           GimpDrawable *drawable,
		           TileManager  *tiles,
		           gboolean      new_layer)
{
  GimpLayer   *layer   = NULL;
  GimpChannel *channel = NULL;
  GimpLayer   *floating_layer;

  if (new_layer)
    {
      layer =
	gimp_layer_new_from_tiles (gimage,
				   gimp_drawable_type_with_alpha (drawable),
				   tiles,
				   _("Transformation"),
				   OPAQUE_OPACITY, NORMAL_MODE);
      if (! layer)
        {
          g_warning ("%s: gimp_layer_new_frome_tiles() failed",
		     G_GNUC_FUNCTION);
          return FALSE;
        }

      tile_manager_get_offsets (tiles, 
				&(GIMP_DRAWABLE (layer)->offset_x),
				&(GIMP_DRAWABLE (layer)->offset_y));

      /*  Start a group undo  */
      undo_push_group_start (gimage, EDIT_PASTE_UNDO);

      floating_sel_attach (layer, drawable);

      /*  End the group undo  */
      undo_push_group_end (gimage);

      /*  Free the tiles  */
      tile_manager_destroy (tiles);

      return TRUE;
    }
  else
    {
      if (GIMP_IS_LAYER (drawable))
	layer = GIMP_LAYER (drawable);
      else if (GIMP_IS_CHANNEL (drawable))
	channel = GIMP_CHANNEL (drawable);
      else
	return FALSE;

      if (layer)
	gimp_layer_add_alpha (layer);

      floating_layer = gimp_image_floating_sel (gimage);

      if (floating_layer)
	floating_sel_relax (floating_layer, TRUE);

      gimp_image_update (gimage,
                         drawable->offset_x,
                         drawable->offset_y,
                         drawable->width,
                         drawable->height);

      /*  Push an undo  */
      if (layer)
	undo_push_layer_mod (gimage, layer);
      else if (channel)
	undo_push_channel_mod (gimage, channel);

      /*  set the current layer's data  */
      drawable->tiles = tiles;

      /*  Fill in the new layer's attributes  */
      drawable->width    = tile_manager_width (tiles);
      drawable->height   = tile_manager_height (tiles);
      drawable->bytes    = tile_manager_bpp (tiles);
      tile_manager_get_offsets (tiles, 
				&drawable->offset_x, &drawable->offset_y);

      if (floating_layer)
	floating_sel_rigor (floating_layer, TRUE);

      gimp_drawable_update (drawable,
			    0, 0,
			    gimp_drawable_width (drawable),
			    gimp_drawable_height (drawable));

      /*  if we were operating on the floating selection, then it's boundary 
       *  and previews need invalidating
       */
      if (drawable == (GimpDrawable *) floating_layer)
	floating_sel_invalidate (floating_layer);

      return TRUE;
    }
}

/* Note: cubic function no longer clips result */
static gdouble
gimp_transform_tool_cubic (gdouble dx,
			   gint    jm1,
			   gint    j,
			   gint    jp1,
			   gint    jp2)
{
  gdouble result;

#if 0
  /* Equivalent to Gimp 1.1.1 and earlier - some ringing */
  result = ((( ( - jm1 + j - jp1 + jp2 ) * dx +
               ( jm1 + jm1 - j - j + jp1 - jp2 ) ) * dx +
               ( - jm1 + jp1 ) ) * dx + j );
  /* Recommended by Mitchell and Netravali - too blurred? */
  result = ((( ( - 7 * jm1 + 21 * j - 21 * jp1 + 7 * jp2 ) * dx +
               ( 15 * jm1 - 36 * j + 27 * jp1 - 6 * jp2 ) ) * dx +
               ( - 9 * jm1 + 9 * jp1 ) ) * dx + (jm1 + 16 * j + jp1) ) / 18.0;
#endif

  /* Catmull-Rom - not bad */
  result = ((( ( - jm1 + 3 * j - 3 * jp1 + jp2 ) * dx +
               ( 2 * jm1 - 5 * j + 4 * jp1 - jp2 ) ) * dx +
               ( - jm1 + jp1 ) ) * dx + (j + j) ) / 2.0;

  return result;
}
