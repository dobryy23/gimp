/*
 * This is the Glass Tile plug-in for the GIMP 0.99
 * Version 1.01
 *
 * Copyright (C) 1997 Karl-Johan Andersson (t96kja@student.tdb.uu.se)
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
 *
 */

/*
 * This filter divide the image into square "glass"-blocks in which
 * the image is refracted. 
 * 
 * The alpha-channel is left unchanged.
 * 
 * Please send any comments or suggestions to
 * Karl-Johan Andersson (t96kja@student.tdb.uu.se)
 * 
 */ 

#include "config.h"

#include <stdlib.h>

#include <gtk/gtk.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "libgimp/stdplugins-intl.h"

/* --- Typedefs --- */
typedef struct
{
  gint xblock;
  gint yblock;
} GlassValues;

typedef struct
{
    gint run;
} GlassInterface;

/* --- Declare local functions --- */
static void query (void);
static void run   (gchar   *name,
		   gint     nparams,
		   GParam  *param,
		   gint    *nreturn_vals,
		   GParam **return_vals);

static gint glass_dialog      (void);
static void glass_ok_callback (GtkWidget *widget,
			       gpointer   data);

static void glasstile (GDrawable *drawable);

/* --- Variables --- */
GPlugInInfo PLUG_IN_INFO =
{
  NULL,    /* init_proc */
  NULL,    /* quit_proc */
  query,   /* query_proc */
  run,     /* run_proc */
};
static GlassValues gtvals =
{
    20,    /* tile width  */
    20     /* tile height */
};
static GlassInterface gt_int =
{
  FALSE    /* run */
};

/* --- Functions --- */

MAIN ()

static void
query (void) 
{
  static GParamDef args[] =
  {
    { PARAM_INT32, "run_mode", "Interactive, non-interactive" },
    { PARAM_IMAGE, "image", "Input image (unused)" },
    { PARAM_DRAWABLE, "drawable", "Input drawable" },
    { PARAM_INT32, "tilex", "Tile width (10 - 50)" },
    { PARAM_INT32, "tiley", "Tile height (10 - 50)" },
  };
  static GParamDef *return_vals = NULL;
  static int nargs = sizeof (args) / sizeof (args[0]);
  static int nreturn_vals = 0; 

  INIT_I18N();

  gimp_install_procedure ("plug_in_glasstile",
			  "Divide the image into square glassblocks",
			  "More here later",
			  "Karl-Johan Andersson", /* Author */
			  "Karl-Johan Andersson", /* Copyright */
			  "1997",
			  N_("<Image>/Filters/Glass Effects/Glass Tile..."),
			  "RGB*, GRAY*",
			  PROC_PLUG_IN,
			  nargs, nreturn_vals,
			  args, return_vals);
}

static void
run (gchar   *name,
     gint     nparams,
     GParam  *param,
     gint    *nreturn_vals,
     GParam **return_vals)
{
  static GParam values[1];
  GDrawable *drawable;
  GRunModeType run_mode;
  GStatusType status = STATUS_SUCCESS;
  
  run_mode = param[0].data.d_int32;
  
  *nreturn_vals = 1;
  *return_vals = values;
  
  values[0].type = PARAM_STATUS;
  values[0].data.d_status = status;

  /*  Get the specified drawable  */
  drawable = gimp_drawable_get (param[2].data.d_drawable);
  
  switch (run_mode)
    {
    case RUN_INTERACTIVE:
      INIT_I18N_UI();
      /*  Possibly retrieve data  */
      gimp_get_data ("plug_in_glasstile", &gtvals);
      
      /*  First acquire information with a dialog  */
      if (! glass_dialog ())
	{
	  gimp_drawable_detach (drawable);
	  return;
	}
      break;
      
    case RUN_NONINTERACTIVE:
      INIT_I18N();
      /*  Make sure all the arguments are there!  */
      if (nparams != 5)
	status = STATUS_CALLING_ERROR;
      if (status == STATUS_SUCCESS)
	{
	  gtvals.xblock = (gint) param[3].data.d_int32;
	  gtvals.yblock = (gint) param[4].data.d_int32;
	}
      if (gtvals.xblock < 10 || gtvals.xblock > 50) 
	status = STATUS_CALLING_ERROR;
      if (gtvals.yblock < 10 || gtvals.yblock > 50) 
	status = STATUS_CALLING_ERROR;
      break;
      
    case RUN_WITH_LAST_VALS:
      INIT_I18N();
      /*  Possibly retrieve data  */
      gimp_get_data ("plug_in_glasstile", &gtvals);
      break;
      
    default:
      break;
    }

  if (status == STATUS_SUCCESS)
    {
      /*  Make sure that the drawable is gray or RGB color  */
      if (gimp_drawable_is_rgb (drawable->id) || gimp_drawable_is_gray (drawable->id))
	{
	  gimp_progress_init ( _("Glass Tile..."));
	  gimp_tile_cache_ntiles (2 * (drawable->width / gimp_tile_width () + 1));
	  
	  glasstile (drawable);
	  
	  if (run_mode != RUN_NONINTERACTIVE)
	    gimp_displays_flush (); 
	  /*  Store data  */
	  if (run_mode == RUN_INTERACTIVE)
	    gimp_set_data ("plug_in_glasstile", &gtvals, sizeof (GlassValues));
	}
      else
	{
	  /* gimp_message ("glasstile: cannot operate on indexed color images"); */
	  status = STATUS_EXECUTION_ERROR;
	}
    }
    
  values[0].data.d_status = status;
  
  gimp_drawable_detach (drawable);
}

static gint
glass_dialog (void)
{
  GtkWidget *dlg;
  GtkWidget *frame;
  GtkWidget *table;
  GtkObject *adj;
  gchar **argv;
  gint    argc;

  argc    = 1;
  argv    = g_new (gchar *, 1);
  argv[0] = g_strdup ("glasstile");

  gtk_init (&argc, &argv);
  gtk_rc_parse (gimp_gtkrc ());

  dlg = gimp_dialog_new (_("Glass Tile"), "glasstile",
			 gimp_plugin_help_func, "filters/glasstile.html",
			 GTK_WIN_POS_MOUSE,
			 FALSE, TRUE, FALSE,

			 _("OK"), glass_ok_callback,
			 NULL, NULL, NULL, TRUE, FALSE,
			 _("Cancel"), gtk_widget_destroy,
			 NULL, 1, NULL, FALSE, TRUE,

			 NULL);

  gtk_signal_connect (GTK_OBJECT (dlg), "destroy",
		      GTK_SIGNAL_FUNC (gtk_main_quit),
		      NULL);

  /*  Parameter settings  */
  frame = gtk_frame_new (_("Parameter Settings"));
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_IN);
  gtk_container_border_width (GTK_CONTAINER (frame), 6);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dlg)->vbox), frame, TRUE, TRUE, 0);

  table = gtk_table_new (2, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 4);
  gtk_table_set_row_spacings (GTK_TABLE (table), 2);
  gtk_container_border_width (GTK_CONTAINER (table), 4);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Horizontal scale - Width */
  adj = gimp_scale_entry_new (GTK_TABLE (table), 0, 0,
			      _("Tile Width:"), 150, 0,
			      gtvals.xblock, 10, 50, 1, 5, 0,
			      NULL, NULL);
  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
		      GTK_SIGNAL_FUNC (gimp_int_adjustment_update),
		      &gtvals.xblock);

  /* Horizontal scale - Height */
  adj = gimp_scale_entry_new (GTK_TABLE (table), 0, 1,
			      _("Tile Height:"), 150, 0,
			      gtvals.yblock, 10, 50, 1, 5, 0,
			      NULL, NULL);
  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
		      GTK_SIGNAL_FUNC (gimp_int_adjustment_update),
		      &gtvals.yblock);

  gtk_widget_show (frame);
  gtk_widget_show (table);

  gtk_widget_show (dlg);

  gtk_main ();
  gdk_flush ();

  return gt_int.run;
}

static void
glass_ok_callback (GtkWidget *widget,
		   gpointer   data)
{
  gt_int.run = TRUE;

  gtk_widget_destroy (GTK_WIDGET (data));
}

/*  -  Filter function  -  */
static void
glasstile (GDrawable *drawable)
{
  GPixelRgn srcPR, destPR;
  gint width, height;
  gint bytes;
  guchar *dest, *d, *ad;
  guchar *cur_row;
  gint row, col, i, iwidth;
  gint x1, y1, x2, y2;

  gint rutbredd, xpixel1, xpixel2;
  gint ruthojd , ypixel2;
  gint xhalv, xoffs, xmitt, xplus;
  gint yhalv, yoffs, ymitt, yplus, cbytes;
      
  /* Get the input area. This is the bounding box of the selection in
   *  the image (or the entire image if there is no selection). Only
   *  operating on the input area is simply an optimization. It doesn't
   *  need to be done for correct operation. (It simply makes it go
   *  faster, since fewer pixels need to be operated on).
   */
  gimp_drawable_mask_bounds (drawable->id, &x1, &y1, &x2, &y2);

  /* Get the size of the input image. (This will/must be the same
   *  as the size of the output image.
   */
  width = drawable->width;
  height = drawable->height;
  bytes = drawable->bpp;
  
  /*  allocate row buffers  */
  cur_row  = (guchar *) malloc ((x2 - x1) * bytes);
  dest = (guchar *) malloc ((x2 - x1) * bytes);
  ad = (guchar *) malloc ((x2 - x1) * bytes);

  /*  initialize the pixel regions  */
  gimp_pixel_rgn_init (&srcPR, drawable, 0, 0, width, height, FALSE, FALSE);
  gimp_pixel_rgn_init (&destPR, drawable, 0, 0, width, height, TRUE, TRUE);

  rutbredd = gtvals.xblock;
  ruthojd = gtvals.yblock;

  xhalv = rutbredd / 2;
  yhalv = ruthojd / 2;
  cbytes = bytes;
  if (cbytes%2 == 0) cbytes--; 
  iwidth = width - x1;
  xplus = rutbredd % 2;
  yplus = ruthojd % 2;

  ymitt = y1;
  yoffs = 0;
  /*  Loop through the rows */
  for (row = y1; row < y2; row++)
    {
      d = dest;
      ypixel2 = ymitt + yoffs * 2;      
      if (ypixel2 < height) 
	gimp_pixel_rgn_get_row (&srcPR, cur_row, x1, ypixel2, iwidth);
      else
	gimp_pixel_rgn_get_row (&srcPR, cur_row, x1, y2-1, iwidth);
      if (cbytes != bytes) /* Alpha check */
	gimp_pixel_rgn_get_row (&srcPR, ad, x1, row, (x2-x1));
      
      yoffs++;
      if (yoffs == yhalv) {
	ymitt += ruthojd;
	yoffs =- yhalv;
	yoffs -= yplus;
      }      
      xmitt = 0;
      xoffs = 0;
      for (col = 0; col < (x2 - x1); col++) /* one pixel */
	{
	  xpixel1 = (xmitt + xoffs) * bytes;
	  xpixel2 = (xmitt + xoffs * 2) * bytes;
	  if (xpixel2 < ((x2 - x1) * bytes)) {
	    for (i = 0; i < cbytes; i++) 
	      d[xpixel1 + i] = cur_row[xpixel2 + i];
	  }
	  else {
	    for (i = 0; i < cbytes; i++)
	      d[xpixel1 + i]=cur_row[xpixel1 + i];
	  }
	  if (cbytes != bytes) /* Alpha channel unchanged */
	    d[xpixel1 + cbytes] = ad[xpixel1 + cbytes]; 
	  xoffs++;
	  if (xoffs == xhalv) {
	    xmitt += rutbredd;
	    xoffs =- xhalv;
	    xoffs -= xplus;
	  }
	}
      /*  Store the dest  */
      gimp_pixel_rgn_set_row (&destPR, dest, x1, row, iwidth);
      
      if ((row % 5) == 0)
	gimp_progress_update ((double) row / (double) (y2 - y1));
    }

  /*  Update region  */
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->id, TRUE);
  gimp_drawable_update (drawable->id, x1, y1, (x2 - x1), (y2 - y1));

  free (cur_row);
  free (dest);
  free (ad);
}
