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

#include <stdlib.h>
#include <stdarg.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpbase/gimpbase.h"

#include "tools-types.h"

#include "base/boundary.h"

#include "core/gimpimage.h"
#include "core/gimpimage-guides.h"
#include "core/gimpimage-mask.h"
#include "core/gimpimage-undo.h"
#include "core/gimplayer.h"
#include "core/gimplayer-floating-sel.h"
#include "core/gimplist.h"
#include "core/gimpundostack.h"

#include "display/gimpdisplay.h"
#include "display/gimpdisplayshell.h"
#include "display/gimpdisplayshell-appearance.h"
#include "display/gimpdisplayshell-selection.h"
#include "display/gimpdisplayshell-transform.h"

#include "gimpdrawtool.h"
#include "gimpeditselectiontool.h"
#include "gimptoolcontrol.h"
#include "tool_manager.h"

#include "gimp-intl.h"


#define EDIT_SELECT_SCROLL_LOCK FALSE
#define ARROW_VELOCITY          25


#define GIMP_TYPE_EDIT_SELECTION_TOOL            (gimp_edit_selection_tool_get_type ())
#define GIMP_EDIT_SELECTION_TOOL(obj)            (GTK_CHECK_CAST ((obj), GIMP_TYPE_EDIT_SELECTION_TOOL, GimpEditSelectionTool))
#define GIMP_EDIT_SELECTION_TOOL_CLASS(klass)    (GTK_CHECK_CLASS_CAST ((klass), GIMP_TYPE_EDIT_SELECTION_TOOL, GimpEditSelectionToolClass))
#define GIMP_IS_EDIT_SELECTION_TOOL(obj)         (GTK_CHECK_TYPE ((obj), GIMP_TYPE_EDIT_SELECTION_TOOL))
#define GIMP_IS_EDIT_SELECTION_TOOL_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_EDIT_SELECTION_TOOL))


typedef struct _GimpEditSelectionTool      GimpEditSelectionTool;
typedef struct _GimpEditSelectionToolClass GimpEditSelectionToolClass;

struct _GimpEditSelectionTool
{
  GimpDrawTool        parent_instance;

  gint                origx, origy;      /*  last x and y coords             */
  gint                cumlx, cumly;      /*  cumulative changes to x and yed */
  gint                x, y;              /*  current x and y coords          */
  gint                num_segs_in;       /* Num seg in selection boundary    */
  gint                num_segs_out;      /* Num seg in selection boundary    */
  BoundSeg           *segs_in;           /* Pointer to the channel sel. segs */
  BoundSeg           *segs_out;          /* Pointer to the channel sel. segs */

  gint                x1, y1;            /*  bounding box of selection mask  */
  gint                x2, y2;

  EditType            edit_type;         /*  translate the mask or layer?    */

  gboolean            first_move;        /*  we undo_freeze after the first  */
};

struct _GimpEditSelectionToolClass
{
  GimpDrawToolClass parent_class;
};


static GType   gimp_edit_selection_tool_get_type   (void) G_GNUC_CONST;

static void    gimp_edit_selection_tool_class_init (GimpEditSelectionToolClass *klass);
static void    gimp_edit_selection_tool_init       (GimpEditSelectionTool *edit_selection_tool);

static void    gimp_edit_selection_tool_button_release (GimpTool        *tool,
                                                        GimpCoords      *coords,
                                                        guint32          time,
                                                        GdkModifierType  state,
                                                        GimpDisplay     *gdisp);
static void    gimp_edit_selection_tool_motion         (GimpTool        *tool,
                                                        GimpCoords      *coords,
                                                        guint32          time,
                                                        GdkModifierType  state,
                                                        GimpDisplay     *gdisp);
void           gimp_edit_selection_tool_arrow_key      (GimpTool        *tool,
                                                        GdkEventKey     *kevent,
                                                        GimpDisplay     *gdisp);

static void    gimp_edit_selection_tool_draw           (GimpDrawTool    *tool);


static GimpDrawToolClass *parent_class = NULL;


static GType
gimp_edit_selection_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpEditSelectionToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_edit_selection_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpEditSelectionTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_edit_selection_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_DRAW_TOOL,
					  "GimpEditSelectionTool", 
                                          &tool_info, 0);
    }

  return tool_type;
}

static void
gimp_edit_selection_tool_class_init (GimpEditSelectionToolClass *klass)
{
  GimpToolClass     *tool_class;
  GimpDrawToolClass *draw_class;

  tool_class   = GIMP_TOOL_CLASS (klass);
  draw_class   = GIMP_DRAW_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  tool_class->button_release = gimp_edit_selection_tool_button_release;
  tool_class->motion         = gimp_edit_selection_tool_motion;

  draw_class->draw	     = gimp_edit_selection_tool_draw;
}

static void
gimp_edit_selection_tool_init (GimpEditSelectionTool *edit_selection_tool)
{
  GimpTool *tool;

  tool = GIMP_TOOL (edit_selection_tool);

  gimp_tool_control_set_scroll_lock (tool->control, EDIT_SELECT_SCROLL_LOCK);
  gimp_tool_control_set_motion_mode (tool->control, GIMP_MOTION_MODE_COMPRESS);

  edit_selection_tool->origx      = 0;
  edit_selection_tool->origy      = 0;

  edit_selection_tool->cumlx      = 0;
  edit_selection_tool->cumly      = 0;

  edit_selection_tool->first_move = TRUE;
}

static void
gimp_edit_selection_tool_calc_coords (GimpEditSelectionTool *edit_select,
                                      GimpDisplay           *gdisp,
                                      gdouble                x,
                                      gdouble                y)
{
  GimpDisplayShell *shell;
  gdouble           x1, y1;
  gdouble           dx, dy;

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  dx = x - edit_select->origx;
  dy = y - edit_select->origy;
  
  x1 = edit_select->x1 + dx;
  y1 = edit_select->y1 + dy;

  edit_select->x = (gint) floor (x1) - (edit_select->x1 - edit_select->origx);
  edit_select->y = (gint) floor (y1) - (edit_select->y1 - edit_select->origy);
}

void
init_edit_selection (GimpTool    *tool,
		     GimpDisplay *gdisp,
		     GimpCoords  *coords,
		     EditType     edit_type)
{
  GimpEditSelectionTool *edit_select;
  GimpDisplayShell      *shell;
  GimpDrawable          *active_drawable;
  gint                   off_x, off_y;
  const gchar           *undo_desc;

  edit_select = g_object_new (GIMP_TYPE_EDIT_SELECTION_TOOL, NULL);

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  /*  Make a check to see if it should be a floating selection translation  */
  if (edit_type == EDIT_MASK_TO_LAYER_TRANSLATE &&
      gimp_image_floating_sel (gdisp->gimage))
    {
      edit_type = EDIT_FLOATING_SEL_TRANSLATE;
    }

  if (edit_type == EDIT_LAYER_TRANSLATE)
    {
      GimpLayer *layer;

      layer = gimp_image_get_active_layer (gdisp->gimage);

      if (gimp_layer_is_floating_sel (layer))
	edit_type = EDIT_FLOATING_SEL_TRANSLATE;
    }

  if (edit_type == EDIT_MASK_TRANSLATE)
    undo_desc = _("Move Selection");
  else if (edit_type == EDIT_LAYER_TRANSLATE)
    undo_desc = _("Move Layer");
  else
    undo_desc = _("Move Floating Layer");

  gimp_image_undo_group_start (gdisp->gimage,
                               edit_type == EDIT_MASK_TRANSLATE ?
                               GIMP_UNDO_GROUP_MASK :
                               GIMP_UNDO_GROUP_ITEM_DISPLACE,
                               undo_desc);

  active_drawable = gimp_image_active_drawable (gdisp->gimage);

  gimp_item_offsets (GIMP_ITEM (active_drawable), &off_x, &off_y);

  edit_select->edit_type = edit_type;

  edit_select->x = edit_select->origx = coords->x - off_x;
  edit_select->y = edit_select->origy = coords->y - off_y;

  gimp_image_mask_boundary (gdisp->gimage,
                            &edit_select->segs_in,
                            &edit_select->segs_out,
                            &edit_select->num_segs_in,
                            &edit_select->num_segs_out);

  edit_select->segs_in  = g_memdup (edit_select->segs_in,
                                    edit_select->num_segs_in *
                                    sizeof (BoundSeg));
  edit_select->segs_out = g_memdup (edit_select->segs_out,
                                    edit_select->num_segs_out *
                                    sizeof (BoundSeg));

  /*  find the bounding box of the selection mask -
   *  this is used for the case of a EDIT_MASK_TO_LAYER_TRANSLATE,
   *  where the translation will result in floating the selection
   *  mask and translating the resulting layer
   */
  gimp_drawable_mask_bounds (active_drawable,
			     &edit_select->x1, &edit_select->y1,
			     &edit_select->x2, &edit_select->y2);

  gimp_edit_selection_tool_calc_coords (edit_select, gdisp,
                                        edit_select->origx,
                                        edit_select->origy);

  {
    gint x1, y1, x2, y2;

    switch (edit_select->edit_type)
      {
      case EDIT_MASK_TRANSLATE:
        gimp_image_mask_bounds (gdisp->gimage, &x1, &y1, &x2, &y2);
        break;

      case EDIT_MASK_TO_LAYER_TRANSLATE:
      case EDIT_MASK_COPY_TO_LAYER_TRANSLATE:
        x1 = edit_select->x1 + off_x;
        y1 = edit_select->y1 + off_y;
        x2 = edit_select->x2 + off_x;
        y2 = edit_select->y2 + off_y;
        break;

      case EDIT_LAYER_TRANSLATE:
      case EDIT_FLOATING_SEL_TRANSLATE:
        x1 = off_x;
        y1 = off_y;
        x2 = x1 + gimp_item_width  (GIMP_ITEM (active_drawable));
        y2 = y1 + gimp_item_height (GIMP_ITEM (active_drawable));

        if (gimp_item_get_linked (GIMP_ITEM (active_drawable)))
          {
            GList *list;

            /*  Expand the rectangle to include all linked layers as well  */
            for (list = GIMP_LIST (gdisp->gimage->layers)->list;
                 list;
                 list = g_list_next (list))
              {
                GimpItem *item = list->data;

                if ((item != (GimpItem *) active_drawable) &&
                    gimp_item_get_linked (item))
                  {
                    gint x3, y3, x4, y4;

                    gimp_item_offsets (item, &x3, &y3);

                    x4 = x3 + gimp_item_width  (item);
                    y4 = y3 + gimp_item_height (item);

                    if (x3 < x1)
                      x1 = x3;
                    if (y3 < y1)
                      y1 = y3;
                    if (x4 > x2)
                      x2 = x4;
                    if (y4 > y2)
                      y2 = y4;
                  }
              }
          }
        break;
      }

    gimp_tool_control_set_snap_offsets (GIMP_TOOL (edit_select)->control,
                                        x1 - coords->x,
                                        y1 - coords->y,
                                        x2 - x1,
                                        y2 - y1);
  }

  gimp_tool_control_activate (GIMP_TOOL (edit_select)->control);
  GIMP_TOOL (edit_select)->gdisp = gdisp;

  g_object_ref (edit_select);

  tool_manager_push_tool (gdisp->gimage->gimp,
			  GIMP_TOOL (edit_select));

  /*  pause the current selection  */
  gimp_display_shell_selection_visibility (shell, GIMP_SELECTION_PAUSE);

  /* initialize the statusbar display */
  gimp_tool_push_status (tool, _("Move: 0, 0"));

  gimp_draw_tool_start (GIMP_DRAW_TOOL (edit_select), gdisp);
}


static void
gimp_edit_selection_tool_button_release (GimpTool        *tool,
                                         GimpCoords      *coords,
                                         guint32          time,
					 GdkModifierType  state,
					 GimpDisplay     *gdisp)
{
  GimpEditSelectionTool *edit_select;
  GimpDisplayShell      *shell;
  GimpLayer             *layer;

  edit_select = GIMP_EDIT_SELECTION_TOOL (tool);

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  /*  resume the current selection  */
  gimp_display_shell_selection_visibility (shell, GIMP_SELECTION_RESUME);

  gimp_tool_pop_status (tool);

  /*  Stop and free the selection core  */
  gimp_draw_tool_stop (GIMP_DRAW_TOOL (edit_select));

  tool_manager_pop_tool (gdisp->gimage->gimp);

  gimp_tool_control_halt (tool_manager_get_active (gdisp->gimage->gimp)->control);

  /* EDIT_MASK_TRANSLATE is performed here at movement end, not 'live' like
   *  the other translation types.
   */
  if (edit_select->edit_type == EDIT_MASK_TRANSLATE)
    {
      gimp_edit_selection_tool_calc_coords (edit_select,
                                            gdisp,
                                            coords->x,
                                            coords->y);

      /* move the selection -- whether there has been net movement or not!
       * (to ensure that there's something on the undo stack)
       */
      gimp_image_mask_translate (gdisp->gimage,
                                 edit_select->cumlx,
                                 edit_select->cumly,
                                 TRUE);

      if (edit_select->first_move)
	{
	  gimp_image_undo_freeze (gdisp->gimage);
	  edit_select->first_move = FALSE;
	}
    }

  /* thaw the undo again */
  gimp_image_undo_thaw (gdisp->gimage);

  if (edit_select->cumlx != 0 || edit_select->cumly != 0)
    {
      layer = gimp_image_get_active_layer (gdisp->gimage);

      if (! (state & GDK_BUTTON3_MASK)                   &&
          edit_select->edit_type == EDIT_LAYER_TRANSLATE &&
          gimp_item_get_linked (GIMP_ITEM (layer)))
        {
          GList    *list;
          GimpItem *item;

          /*  translate all linked channels and vectors as well  */
          for (list = GIMP_LIST (gdisp->gimage->channels)->list;
               list;
               list = g_list_next (list))
            {
              item = (GimpItem *) list->data;

              if (gimp_item_get_linked (item))
                gimp_item_translate (item,
                                     edit_select->cumlx,
                                     edit_select->cumly,
                                     TRUE);
            }

          for (list = GIMP_LIST (gdisp->gimage->vectors)->list;
               list;
               list = g_list_next (list))
            {
              item = (GimpItem *) list->data;

              if (gimp_item_get_linked (item))
                gimp_item_translate (item,
                                     edit_select->cumlx,
                                     edit_select->cumly,
                                     TRUE);
            }
        }

      gimp_viewable_invalidate_preview (GIMP_VIEWABLE (layer));
    }

  gimp_image_undo_group_end (gdisp->gimage);

  if (state & GDK_BUTTON3_MASK) /* OPERATION CANCELLED */
    {
      /* Operation cancelled - undo the undo-group! */
      gimp_image_undo (gdisp->gimage);
    }

  gimp_image_flush (gdisp->gimage);

  g_free (edit_select->segs_in);
  g_free (edit_select->segs_out);

  edit_select->segs_in      = NULL;
  edit_select->segs_out     = NULL;
  edit_select->num_segs_in  = 0;
  edit_select->num_segs_out = 0;

  g_object_unref (edit_select);
}


static void
gimp_edit_selection_tool_motion (GimpTool        *tool,
                                 GimpCoords      *coords,
                                 guint32          time,
				 GdkModifierType  state,
				 GimpDisplay     *gdisp)
{
  GimpEditSelectionTool *edit_select;
  GimpDisplayShell      *shell;
  gint                   off_x, off_y;
  gdouble                motion_x, motion_y;

  edit_select = GIMP_EDIT_SELECTION_TOOL (tool);

  shell = GIMP_DISPLAY_SHELL (gdisp->shell);

  gdk_flush ();

  gimp_draw_tool_pause (GIMP_DRAW_TOOL (tool));

  gimp_item_offsets (GIMP_ITEM (gimp_image_active_drawable (gdisp->gimage)),
                     &off_x, &off_y);

  motion_x = coords->x - off_x;
  motion_y = coords->y - off_y;

  /* now do the actual move. */

  gimp_edit_selection_tool_calc_coords (edit_select,
                                        gdisp,
                                        motion_x,
                                        motion_y);

  /******************************************* adam's live move *******/
  /********************************************************************/
  {
    gint x, y;

    x = edit_select->x;
    y = edit_select->y;

    /* if there has been movement, move the selection  */
    if (edit_select->origx != x || edit_select->origy != y)
      {
	gint xoffset, yoffset;
 
	xoffset = x - edit_select->origx;
	yoffset = y - edit_select->origy;

	edit_select->cumlx += xoffset;
	edit_select->cumly += yoffset;

	switch (edit_select->edit_type)
	  {
	  case EDIT_MASK_TRANSLATE:
	    /*  we don't do the actual edit selection move here.  */
	    edit_select->origx = x;
	    edit_select->origy = y;
	    break;

	  case EDIT_LAYER_TRANSLATE:
            {
              GimpLayer *floating_layer;
              GimpItem  *active_item;

              floating_layer = gimp_image_floating_sel (gdisp->gimage);
              active_item    = GIMP_ITEM (gdisp->gimage->active_layer);

              if (floating_layer)
                floating_sel_relax (floating_layer, TRUE);

              gimp_item_translate (active_item, xoffset, yoffset, TRUE);

              if (gimp_item_get_linked (active_item))
                {
                  GList *list;

                  /*  translate all linked items as well  */
                  for (list = GIMP_LIST (gdisp->gimage->layers)->list;
                       list;
                       list = g_list_next (list))
                    {
                      GimpItem *item = list->data;

                      if (item != active_item && gimp_item_get_linked (item))
                        gimp_item_translate (item, xoffset, yoffset, TRUE);
                    }
                }

              if (floating_layer)
                floating_sel_rigor (floating_layer, TRUE);

              if (edit_select->first_move)
                {
                  gimp_image_undo_freeze (gdisp->gimage);
                  edit_select->first_move = FALSE;
                }
            }
	    break;

	  case EDIT_MASK_TO_LAYER_TRANSLATE:
	  case EDIT_MASK_COPY_TO_LAYER_TRANSLATE:
	    if (! gimp_image_mask_float (gdisp->gimage, 
                                         gimp_image_active_drawable (gdisp->gimage),
                                         edit_select->edit_type ==
                                         EDIT_MASK_TO_LAYER_TRANSLATE,
                                         0, 0))
	      {
		/* no region to float, abort safely */
		gimp_draw_tool_resume (GIMP_DRAW_TOOL (tool));

		return;
	      }

	    /*  this is always the first move, since we switch to
             *  EDIT_FLOATING_SEL_TRANSLATE when finished here
             */
	    gimp_image_undo_freeze (gdisp->gimage);
	    edit_select->first_move = FALSE; 

	    edit_select->origx -= edit_select->x1;
	    edit_select->origy -= edit_select->y1;
	    edit_select->x2    -= edit_select->x1;
	    edit_select->y2    -= edit_select->y1;
	    edit_select->x1     = 0;
	    edit_select->y1     = 0;

	    edit_select->edit_type = EDIT_FLOATING_SEL_TRANSLATE;

            /* fall through */

	  case EDIT_FLOATING_SEL_TRANSLATE:
            {
              GimpLayer *layer = gimp_image_get_active_layer (gdisp->gimage);

              floating_sel_relax (layer, TRUE);
              gimp_item_translate (GIMP_ITEM (layer), xoffset, yoffset, TRUE);
              floating_sel_rigor (layer, TRUE);

              if (edit_select->first_move)
                {
                  gimp_image_undo_freeze (gdisp->gimage);
                  edit_select->first_move = FALSE;
                }
            }
	    break;

	  default:
	    g_warning ("esm / BAD FALLTHROUGH");
	  }
      }

    gimp_display_flush (gdisp);
  }
  /********************************************************************/
  /********************************************************************/

  gimp_tool_pop_status (tool);

  gimp_tool_push_status_coords (tool,
                                _("Move: "),
                                edit_select->cumlx,
                                ", ",
                                edit_select->cumly);

  gimp_draw_tool_resume (GIMP_DRAW_TOOL (tool));
}

static void
selection_transform_segs (GimpEditSelectionTool *edit_select,
			  BoundSeg              *src_segs,
			  GdkSegment            *dest_segs,
			  gint                   num_segs)
{
  GimpDisplayShell *shell;
  gint              x, y;
  gint              i;

  shell = GIMP_DISPLAY_SHELL (GIMP_TOOL (edit_select)->gdisp->shell);

  for (i = 0; i < num_segs; i++)
    {
      gimp_display_shell_transform_xy (shell,
                                       src_segs[i].x1 + edit_select->cumlx, 
                                       src_segs[i].y1 + edit_select->cumly,
                                       &x, &y,
                                       FALSE);

      dest_segs[i].x1 = CLAMP (x, -1, shell->disp_width);
      dest_segs[i].y1 = CLAMP (y, -1, shell->disp_height);

      gimp_display_shell_transform_xy (shell,
                                       src_segs[i].x2 + edit_select->cumlx, 
                                       src_segs[i].y2 + edit_select->cumly,
                                       &x, &y,
                                       FALSE);

      dest_segs[i].x2 = CLAMP (x, -1, shell->disp_width);
      dest_segs[i].y2 = CLAMP (y, -1, shell->disp_height);
    }
}

static void
gimp_edit_selection_tool_draw (GimpDrawTool *draw_tool)
{
  GimpEditSelectionTool *edit_select;
  GimpTool              *tool;
  GimpDisplay           *gdisp;
  Selection             *select;

  edit_select = GIMP_EDIT_SELECTION_TOOL (draw_tool);
  tool        = GIMP_TOOL (draw_tool);

  gdisp  = tool->gdisp;
  select = GIMP_DISPLAY_SHELL (gdisp->shell)->select;

  switch (edit_select->edit_type)
    {
    case EDIT_MASK_TRANSLATE:
      {
        GimpLayer  *layer        = gimp_image_get_active_layer (gdisp->gimage);
        gboolean    floating_sel = gimp_layer_is_floating_sel (layer);
        GdkSegment *segs_copy;

        if (! floating_sel)
          {
            segs_copy = g_new (GdkSegment, edit_select->num_segs_in);

            selection_transform_segs (edit_select,
                                      edit_select->segs_in,
                                      segs_copy,
                                      edit_select->num_segs_in);

            /*  Draw the items  */
            gdk_draw_segments (draw_tool->win, draw_tool->gc,
                               segs_copy, select->num_segs_in);

            g_free (segs_copy);
          }

        segs_copy = g_new (GdkSegment, edit_select->num_segs_out);

        selection_transform_segs (edit_select,
                                  edit_select->segs_out,
                                  segs_copy,
                                  edit_select->num_segs_out);

        /*  Draw the items  */
        gdk_draw_segments (draw_tool->win, draw_tool->gc,
                           segs_copy, select->num_segs_out);

        g_free (segs_copy);
      }
      break;

    case EDIT_MASK_TO_LAYER_TRANSLATE:
    case EDIT_MASK_COPY_TO_LAYER_TRANSLATE:
      gimp_draw_tool_draw_rectangle (draw_tool,
                                     FALSE,
                                     edit_select->x1,
                                     edit_select->y1,
                                     edit_select->x2 - edit_select->x1,
                                     edit_select->y2 - edit_select->y1,
                                     TRUE);
      break;

    case EDIT_LAYER_TRANSLATE:
      {
        GimpItem *active_item;
        gint      x1, y1, x2, y2;

        active_item = GIMP_ITEM (gdisp->gimage->active_layer);

        gimp_item_offsets (active_item, &x1, &y1);

        x2 = x1 + gimp_item_width  (active_item);
        y2 = y1 + gimp_item_height (active_item);

        if (gimp_item_get_linked (active_item))
          {
            GList *list;

            /*  Expand the rectangle to include all linked layers as well  */
            for (list = GIMP_LIST (gdisp->gimage->layers)->list;
                 list;
                 list = g_list_next (list))
              {
                GimpItem *item = list->data;

                if (item != active_item && gimp_item_get_linked (item))
                  {
                    gint x3, y3, x4, y4;

                    gimp_item_offsets (item, &x3, &y3);

                    x4 = x3 + gimp_item_width  (item);
                    y4 = y3 + gimp_item_height (item);

                    if (x3 < x1)
                      x1 = x3;
                    if (y3 < y1)
                      y1 = y3;
                    if (x4 > x2)
                      x2 = x4;
                    if (y4 > y2)
                      y2 = y4;
                  }
              }
          }

        gimp_draw_tool_draw_rectangle (draw_tool,
                                       FALSE,
                                       x1, y1,
                                       x2 - x1, y2 - y1,
                                       FALSE);
      }
      break;

    case EDIT_FLOATING_SEL_TRANSLATE:
      {
        GdkSegment *segs_copy = g_new (GdkSegment, edit_select->num_segs_in);

        /* The selection segs are in image space convert these 
         * to display space.
         * Takes care of offset/zoom etc etc.
         */
        selection_transform_segs (edit_select,
                                  edit_select->segs_in,
                                  segs_copy,
                                  edit_select->num_segs_in);

        /*  Draw the items  */
        gdk_draw_segments (draw_tool->win, draw_tool->gc,
                           segs_copy, select->num_segs_in);

        g_free (segs_copy);
      }
      break;
    }
}

/* could move this function to a more central location
 * so it can be used by other tools?
 */
static gint
process_event_queue_keys (GdkEventKey *kevent, 
			  ... /* GdkKeyType, GdkModifierType, value ... 0 */)
{

#define FILTER_MAX_KEYS 50

  va_list          argp;
  GdkEvent        *event;
  GList           *event_list = NULL;
  GList           *list;
  guint            keys[FILTER_MAX_KEYS];
  GdkModifierType  modifiers[FILTER_MAX_KEYS];
  gint             values[FILTER_MAX_KEYS];
  gint             i      = 0;
  gint             n_keys = 0;
  gint             value  = 0;
  gboolean         done   = FALSE;
  GtkWidget       *orig_widget;

  va_start (argp, kevent);

  while (n_keys < FILTER_MAX_KEYS && (keys[n_keys] = va_arg (argp, guint)) != 0)
    {
      modifiers[n_keys] = va_arg (argp, GdkModifierType);
      values[n_keys]    = va_arg (argp, gint);
      n_keys++;
    }

  va_end (argp);

  for (i = 0; i < n_keys; i++)
    if (kevent->keyval                 == keys[i] &&
        (kevent->state & modifiers[i]) == modifiers[i])
      value += values[i];

  orig_widget = gtk_get_event_widget ((GdkEvent *) kevent);

  while (gdk_events_pending () > 0 && ! done)
    {
      gboolean discard_event = FALSE;

      event = gdk_event_get ();

      if (! event || orig_widget != gtk_get_event_widget (event))
        {
          done = TRUE;
        }
      else
        {
          if (event->any.type == GDK_KEY_PRESS)
            {
              for (i = 0; i < n_keys; i++)
                if (event->key.keyval                 == keys[i] &&
                    (event->key.state & modifiers[i]) == modifiers[i])
                  {
                    discard_event = TRUE;
                    value += values[i];
                  }

              if (! discard_event)
                done = TRUE;
            }
          /* should there be more types here? */
          else if (event->any.type != GDK_KEY_RELEASE &&
                   event->any.type != GDK_MOTION_NOTIFY &&
                   event->any.type != GDK_EXPOSE)
            done = FALSE;
        }

      if (! event)
        ; /* Do nothing */
      else if (! discard_event)
        event_list = g_list_prepend (event_list, event);
      else
        gdk_event_free (event);
    }

  event_list = g_list_reverse (event_list);

  /* unget the unused events and free the list */
  for (list = event_list; list; list = g_list_next (list))
    {
      gdk_event_put ((GdkEvent *) list->data);
      gdk_event_free ((GdkEvent *) list->data);
    }

  g_list_free (event_list);

  return value;

#undef FILTER_MAX_KEYS
}

void
gimp_edit_selection_tool_arrow_key (GimpTool    *tool,
				    GdkEventKey *kevent,
				    GimpDisplay *gdisp)
{
  gint          mask_inc_x      = 0;
  gint          mask_inc_y      = 0;
  gint          inc_x           = 0;
  gint          inc_y           = 0;
  GimpUndo     *undo;
  gboolean      push_undo       = TRUE;
  gboolean      translate_mask  = FALSE;
  gboolean      translate_layer = FALSE;
  GimpLayer    *layer           = NULL;
  EditType      edit_type       = EDIT_MASK_TRANSLATE;
  GimpUndoType  undo_type;

  /*  check for mask translation first because the translate_layer
   *  modifiers match the translate_mask ones...
   */
  mask_inc_x = 
    process_event_queue_keys (kevent,
			      GDK_Left, (GDK_MOD1_MASK | GDK_SHIFT_MASK),
                              -1 * ARROW_VELOCITY,

			      GDK_Left, GDK_MOD1_MASK,
                              -1,

			      GDK_Right, (GDK_MOD1_MASK | GDK_SHIFT_MASK),
                              1 * ARROW_VELOCITY,

			      GDK_Right, GDK_MOD1_MASK,
                              1,

			      0);

  mask_inc_y =
    process_event_queue_keys (kevent,
			      GDK_Up, (GDK_MOD1_MASK | GDK_SHIFT_MASK),
                              -1 * ARROW_VELOCITY,

			      GDK_Up, GDK_MOD1_MASK,
                              -1,

			      GDK_Down, (GDK_MOD1_MASK | GDK_SHIFT_MASK),
                              1 * ARROW_VELOCITY,

			      GDK_Down, GDK_MOD1_MASK,
                              1,

			      0);

  if (mask_inc_x != 0 || mask_inc_y != 0)
    {
      translate_mask = TRUE;
    }
  else
    {
      inc_x = process_event_queue_keys (kevent,
                                        GDK_Left, GDK_SHIFT_MASK,
                                        -1 * ARROW_VELOCITY,

                                        GDK_Left, 0,
                                        -1,

                                        GDK_Right, GDK_SHIFT_MASK,
                                        1 * ARROW_VELOCITY,

                                        GDK_Right, 0,
                                        1,

                                        0);

      inc_y = process_event_queue_keys (kevent,
                                        GDK_Up, GDK_SHIFT_MASK,
                                        -1 * ARROW_VELOCITY,

                                        GDK_Up, 0,
                                        -1,

                                        GDK_Down, GDK_SHIFT_MASK,
                                        1 * ARROW_VELOCITY,

                                        GDK_Down, 0,
                                        1,

                                        0);

      if (inc_x != 0 || inc_y != 0)
        {
          translate_layer = TRUE;

          layer = gimp_image_get_active_layer (gdisp->gimage);

          if (gimp_layer_is_floating_sel (layer))
            edit_type = EDIT_FLOATING_SEL_TRANSLATE;
          else
            edit_type = EDIT_LAYER_TRANSLATE;
        }
      else
        {
          return;
        }
    }

  if (translate_mask)
    undo_type = GIMP_UNDO_GROUP_MASK;
  else
    undo_type = GIMP_UNDO_GROUP_ITEM_DISPLACE;

  undo = gimp_undo_stack_peek (gdisp->gimage->undo_stack);

  if (GIMP_IS_UNDO_STACK (undo) && undo->undo_type == undo_type)
    {
      if (g_object_get_data (G_OBJECT (undo), "edit-selection-tool") ==
          (gpointer) tool                                                 &&
          g_object_get_data (G_OBJECT (undo), "edit-selection-mask") ==
          GINT_TO_POINTER (translate_mask)                                &&
          g_object_get_data (G_OBJECT (undo), "edit-selection-layer") ==
          (gpointer) layer                                                &&
          g_object_get_data (G_OBJECT (undo), "edit-selection-type") ==
          GINT_TO_POINTER (edit_type))
        {
          push_undo = FALSE;
        }
    }

  if (push_undo)
    {
      const gchar *undo_desc;

      if (translate_mask)
        undo_desc = _("Move Selection");
      else if (edit_type == EDIT_FLOATING_SEL_TRANSLATE)
        undo_desc = _("Move Floating Layer");
      else
        undo_desc = _("Move Layer");

      if (gimp_image_undo_group_start (gdisp->gimage, undo_type, undo_desc))
        {
          undo = gimp_undo_stack_peek (gdisp->gimage->undo_stack);

          if (GIMP_IS_UNDO_STACK (undo) && undo->undo_type == undo_type)
            {
              g_object_set_data (G_OBJECT (undo), "edit-selection-tool",
                                 tool);
              g_object_set_data (G_OBJECT (undo), "edit-selection-mask",
                                 GINT_TO_POINTER (translate_mask));
              g_object_set_data (G_OBJECT (undo), "edit-selection-layer",
                                 layer);
              g_object_set_data (G_OBJECT (undo), "edit-selection-type",
                                 GINT_TO_POINTER (edit_type));
            }
        }
    }

  if (translate_mask)
    {
      gimp_image_mask_translate (gdisp->gimage, mask_inc_x, mask_inc_y,
                                 push_undo);
    }
  else if (translate_layer)
    {
      switch (edit_type)
	{
	case EDIT_MASK_TRANSLATE:
	case EDIT_MASK_TO_LAYER_TRANSLATE:
        case EDIT_MASK_COPY_TO_LAYER_TRANSLATE:
          /*  this won't happen  */
	  break;

	case EDIT_LAYER_TRANSLATE:
          {
            GimpLayer *floating_layer;
            GimpItem  *active_item;

            floating_layer = gimp_image_floating_sel (gdisp->gimage);
            active_item    = GIMP_ITEM (gdisp->gimage->active_layer);

            if (floating_layer)
              floating_sel_relax (floating_layer, push_undo);

            gimp_item_translate (active_item, inc_x, inc_y, push_undo);

            if (gimp_item_get_linked (active_item))
              {
                GList *list;

                /*  translate all linked items as well  */
                for (list = GIMP_LIST (gdisp->gimage->layers)->list;
                     list; 
                     list = g_list_next (list))
                  {
                    GimpItem *item = list->data;

                    if (item != active_item && gimp_item_get_linked (item))
                      gimp_item_translate (item, inc_x, inc_y, push_undo);
                  }
              }

            if (floating_layer)
              floating_sel_rigor (floating_layer, push_undo);
          }
	  break;

	case EDIT_FLOATING_SEL_TRANSLATE:
	  floating_sel_relax (layer, push_undo);
	  gimp_item_translate (GIMP_ITEM (layer), inc_x, inc_y, push_undo);
	  floating_sel_rigor (layer, push_undo);
	  break;
	}
    }

  if (push_undo)
    gimp_image_undo_group_end (gdisp->gimage);

  gimp_image_flush (gdisp->gimage);
}
