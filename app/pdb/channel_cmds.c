/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2000 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"


#include <glib-object.h>

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "core/gimpchannel.h"
#include "core/gimpimage.h"

#include "libgimpcolor/gimpcolor.h"

static ProcRecord channel_new_proc;
static ProcRecord channel_copy_proc;
static ProcRecord channel_delete_proc;
static ProcRecord channel_combine_masks_proc;
static ProcRecord channel_get_name_proc;
static ProcRecord channel_set_name_proc;
static ProcRecord channel_get_visible_proc;
static ProcRecord channel_set_visible_proc;
static ProcRecord channel_get_show_masked_proc;
static ProcRecord channel_set_show_masked_proc;
static ProcRecord channel_get_opacity_proc;
static ProcRecord channel_set_opacity_proc;
static ProcRecord channel_get_color_proc;
static ProcRecord channel_set_color_proc;
static ProcRecord channel_get_tattoo_proc;
static ProcRecord channel_set_tattoo_proc;

void
register_channel_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &channel_new_proc);
  procedural_db_register (gimp, &channel_copy_proc);
  procedural_db_register (gimp, &channel_delete_proc);
  procedural_db_register (gimp, &channel_combine_masks_proc);
  procedural_db_register (gimp, &channel_get_name_proc);
  procedural_db_register (gimp, &channel_set_name_proc);
  procedural_db_register (gimp, &channel_get_visible_proc);
  procedural_db_register (gimp, &channel_set_visible_proc);
  procedural_db_register (gimp, &channel_get_show_masked_proc);
  procedural_db_register (gimp, &channel_set_show_masked_proc);
  procedural_db_register (gimp, &channel_get_opacity_proc);
  procedural_db_register (gimp, &channel_set_opacity_proc);
  procedural_db_register (gimp, &channel_get_color_proc);
  procedural_db_register (gimp, &channel_set_color_proc);
  procedural_db_register (gimp, &channel_get_tattoo_proc);
  procedural_db_register (gimp, &channel_set_tattoo_proc);
}

static Argument *
channel_new_invoker (Gimp     *gimp,
                     Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gint32 width;
  gint32 height;
  gchar *name;
  gdouble opacity;
  GimpRGB color;
  GimpChannel *channel = NULL;

  gimage = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (gimage))
    success = FALSE;

  width = args[1].value.pdb_int;
  if (width <= 0)
    success = FALSE;

  height = args[2].value.pdb_int;
  if (height <= 0)
    success = FALSE;

  name = (gchar *) args[3].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  opacity = args[4].value.pdb_float;
  if (opacity < 0.0 || opacity > 100.0)
    success = FALSE;

  color = args[5].value.pdb_color;

  if (success)
    {
      GimpRGB rgb_color = color;
    
      rgb_color.a = opacity / 100.0;
      channel = gimp_channel_new (gimage, width, height, name, &rgb_color);
      success = channel != NULL;
    }

  return_args = procedural_db_return_args (&channel_new_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (channel));

  return return_args;
}

static ProcArg channel_new_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image to which to add the channel"
  },
  {
    GIMP_PDB_INT32,
    "width",
    "The channel width: (0 < width)"
  },
  {
    GIMP_PDB_INT32,
    "height",
    "The channel height: (0 < height)"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The channel name"
  },
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The channel opacity: (0 <= opacity <= 100)"
  },
  {
    GIMP_PDB_COLOR,
    "color",
    "The channel compositing color"
  }
};

static ProcArg channel_new_outargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The newly created channel"
  }
};

static ProcRecord channel_new_proc =
{
  "gimp_channel_new",
  "Create a new channel.",
  "This procedure creates a new channel with the specified width and height. Name, opacity, and color are also supplied parameters. The new channel still needs to be added to the image, as this is not automatic. Add the new channel with the 'gimp_image_add_channel' command. Other attributes such as channel show masked, should be set with explicit procedure calls. The channel's contents are undefined initially.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  6,
  channel_new_inargs,
  1,
  channel_new_outargs,
  { { channel_new_invoker } }
};

static Argument *
channel_copy_invoker (Gimp     *gimp,
                      Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;
  GimpChannel *copy = NULL;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  if (success)
    success = (copy = GIMP_CHANNEL (gimp_item_duplicate (GIMP_ITEM (channel), G_TYPE_FROM_INSTANCE (channel), FALSE))) != NULL;

  return_args = procedural_db_return_args (&channel_copy_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_ID (GIMP_ITEM (copy));

  return return_args;
}

static ProcArg channel_copy_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel to copy"
  }
};

static ProcArg channel_copy_outargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel_copy",
    "The newly copied channel"
  }
};

static ProcRecord channel_copy_proc =
{
  "gimp_channel_copy",
  "Copy a channel.",
  "This procedure copies the specified channel and returns the copy.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_copy_inargs,
  1,
  channel_copy_outargs,
  { { channel_copy_invoker } }
};

static Argument *
channel_delete_invoker (Gimp     *gimp,
                        Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  if (success)
    {
      if (! gimp_item_get_image (GIMP_ITEM (channel)))
	{
	  g_object_unref (channel);
	  success = TRUE;
	}
    
    }

  return procedural_db_return_args (&channel_delete_proc, success);
}

static ProcArg channel_delete_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel to delete"
  }
};

static ProcRecord channel_delete_proc =
{
  "gimp_channel_delete",
  "Delete a channel.",
  "This procedure deletes the specified channel. This must not be done if the gimage containing this channel was already deleted or if the channel was already removed from the image. The only case in which this procedure is useful is if you want to get rid of a channel which has not yet been added to an image.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_delete_inargs,
  0,
  NULL,
  { { channel_delete_invoker } }
};

static Argument *
channel_combine_masks_invoker (Gimp     *gimp,
                               Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel1;
  GimpChannel *channel2;
  gint32 operation;
  gint32 offx;
  gint32 offy;

  channel1 = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel1))
    success = FALSE;

  channel2 = (GimpChannel *) gimp_item_get_by_ID (gimp, args[1].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel2))
    success = FALSE;

  operation = args[2].value.pdb_int;
  if (operation < GIMP_CHANNEL_OP_ADD || operation > GIMP_CHANNEL_OP_INTERSECT)
    success = FALSE;

  offx = args[3].value.pdb_int;

  offy = args[4].value.pdb_int;

  if (success)
    {
      gimp_channel_combine_mask (channel1, channel2, operation, offx, offy);
    }

  return procedural_db_return_args (&channel_combine_masks_proc, success);
}

static ProcArg channel_combine_masks_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel1",
    "The channel1"
  },
  {
    GIMP_PDB_CHANNEL,
    "channel2",
    "The channel2"
  },
  {
    GIMP_PDB_INT32,
    "operation",
    "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }"
  },
  {
    GIMP_PDB_INT32,
    "offx",
    "x offset between upper left corner of channels: (second - first)"
  },
  {
    GIMP_PDB_INT32,
    "offy",
    "y offset between upper left corner of channels: (second - first)"
  }
};

static ProcRecord channel_combine_masks_proc =
{
  "gimp_channel_combine_masks",
  "Combine two channel masks.",
  "This procedure combines two channel masks. The result is stored in the first channel.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  5,
  channel_combine_masks_inargs,
  0,
  NULL,
  { { channel_combine_masks_invoker } }
};

static Argument *
channel_get_name_invoker (Gimp     *gimp,
                          Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  return_args = procedural_db_return_args (&channel_get_name_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_object_get_name (GIMP_OBJECT (channel)));

  return return_args;
}

static ProcArg channel_get_name_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_name_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The channel name"
  }
};

static ProcRecord channel_get_name_proc =
{
  "gimp_channel_get_name",
  "Get the name of the specified channel.",
  "This procedure returns the specified channel's name.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_get_name_inargs,
  1,
  channel_get_name_outargs,
  { { channel_get_name_invoker } }
};

static Argument *
channel_set_name_invoker (Gimp     *gimp,
                          Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gchar *name;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    gimp_item_rename (GIMP_ITEM (channel), name);

  return procedural_db_return_args (&channel_set_name_proc, success);
}

static ProcArg channel_set_name_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_STRING,
    "name",
    "The new channel name"
  }
};

static ProcRecord channel_set_name_proc =
{
  "gimp_channel_set_name",
  "Set the name of the specified channel.",
  "This procedure sets the specified channel's name.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  channel_set_name_inargs,
  0,
  NULL,
  { { channel_set_name_invoker } }
};

static Argument *
channel_get_visible_invoker (Gimp     *gimp,
                             Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  return_args = procedural_db_return_args (&channel_get_visible_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_drawable_get_visible (GIMP_DRAWABLE (channel));

  return return_args;
}

static ProcArg channel_get_visible_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_visible_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "visible",
    "The channel visibility"
  }
};

static ProcRecord channel_get_visible_proc =
{
  "gimp_channel_get_visible",
  "Get the visibility of the specified channel.",
  "This procedure returns the specified channel's visibility.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_get_visible_inargs,
  1,
  channel_get_visible_outargs,
  { { channel_get_visible_invoker } }
};

static Argument *
channel_set_visible_invoker (Gimp     *gimp,
                             Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gboolean visible;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  visible = args[1].value.pdb_int ? TRUE : FALSE;

  if (success)
    gimp_drawable_set_visible (GIMP_DRAWABLE (channel), visible, TRUE);

  return procedural_db_return_args (&channel_set_visible_proc, success);
}

static ProcArg channel_set_visible_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_INT32,
    "visible",
    "The new channel visibility"
  }
};

static ProcRecord channel_set_visible_proc =
{
  "gimp_channel_set_visible",
  "Set the visibility of the specified channel.",
  "This procedure sets the specified channel's visibility.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  channel_set_visible_inargs,
  0,
  NULL,
  { { channel_set_visible_invoker } }
};

static Argument *
channel_get_show_masked_invoker (Gimp     *gimp,
                                 Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  return_args = procedural_db_return_args (&channel_get_show_masked_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_channel_get_show_masked (channel);

  return return_args;
}

static ProcArg channel_get_show_masked_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_show_masked_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "show_masked",
    "The channel composite method"
  }
};

static ProcRecord channel_get_show_masked_proc =
{
  "gimp_channel_get_show_masked",
  "Get the composite method of the specified channel.",
  "This procedure returns the specified channel's composite method. If it is non-zero, then the channel is composited with the image so that masked regions are shown. Otherwise, selected regions are shown.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_get_show_masked_inargs,
  1,
  channel_get_show_masked_outargs,
  { { channel_get_show_masked_invoker } }
};

static Argument *
channel_set_show_masked_invoker (Gimp     *gimp,
                                 Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gboolean show_masked;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  show_masked = args[1].value.pdb_int ? TRUE : FALSE;

  if (success)
    gimp_channel_set_show_masked (channel, show_masked);

  return procedural_db_return_args (&channel_set_show_masked_proc, success);
}

static ProcArg channel_set_show_masked_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_INT32,
    "show_masked",
    "The new channel composite method"
  }
};

static ProcRecord channel_set_show_masked_proc =
{
  "gimp_channel_set_show_masked",
  "Set the composite method of the specified channel.",
  "This procedure sets the specified channel's composite method. If it is non-zero, then the channel is composited with the image so that masked regions are shown. Otherwise, selected regions are shown.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  channel_set_show_masked_inargs,
  0,
  NULL,
  { { channel_set_show_masked_invoker } }
};

static Argument *
channel_get_opacity_invoker (Gimp     *gimp,
                             Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  return_args = procedural_db_return_args (&channel_get_opacity_proc, success);

  if (success)
    return_args[1].value.pdb_float = gimp_channel_get_opacity (channel) * 100.0;

  return return_args;
}

static ProcArg channel_get_opacity_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_opacity_outargs[] =
{
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The channel opacity"
  }
};

static ProcRecord channel_get_opacity_proc =
{
  "gimp_channel_get_opacity",
  "Get the opacity of the specified channel.",
  "This procedure returns the specified channel's opacity.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_get_opacity_inargs,
  1,
  channel_get_opacity_outargs,
  { { channel_get_opacity_invoker } }
};

static Argument *
channel_set_opacity_invoker (Gimp     *gimp,
                             Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gdouble opacity;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  opacity = args[1].value.pdb_float;
  if (opacity < 0.0 || opacity > 100.0)
    success = FALSE;

  if (success)
    gimp_channel_set_opacity (channel, opacity / 100.0, TRUE);

  return procedural_db_return_args (&channel_set_opacity_proc, success);
}

static ProcArg channel_set_opacity_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The new channel opacity (0 <= opacity <= 100)"
  }
};

static ProcRecord channel_set_opacity_proc =
{
  "gimp_channel_set_opacity",
  "Set the opacity of the specified channel.",
  "This procedure sets the specified channel's opacity.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  channel_set_opacity_inargs,
  0,
  NULL,
  { { channel_set_opacity_invoker } }
};

static Argument *
channel_get_color_invoker (Gimp     *gimp,
                           Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;
  GimpRGB color;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  if (success)
    {
      gimp_channel_get_color (channel, &color);
    }

  return_args = procedural_db_return_args (&channel_get_color_proc, success);

  if (success)
    return_args[1].value.pdb_color = color;

  return return_args;
}

static ProcArg channel_get_color_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_color_outargs[] =
{
  {
    GIMP_PDB_COLOR,
    "color",
    "The channel compositing color"
  }
};

static ProcRecord channel_get_color_proc =
{
  "gimp_channel_get_color",
  "Get the compositing color of the specified channel.",
  "This procedure returns the specified channel's compositing color.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  channel_get_color_inargs,
  1,
  channel_get_color_outargs,
  { { channel_get_color_invoker } }
};

static Argument *
channel_set_color_invoker (Gimp     *gimp,
                           Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  GimpRGB color;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  color = args[1].value.pdb_color;

  if (success)
    {
      GimpRGB rgb_color = color;
    
      rgb_color.a = channel->color.a;
      gimp_channel_set_color (channel, &rgb_color, TRUE);
    }

  return procedural_db_return_args (&channel_set_color_proc, success);
}

static ProcArg channel_set_color_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_COLOR,
    "color",
    "The new channel compositing color"
  }
};

static ProcRecord channel_set_color_proc =
{
  "gimp_channel_set_color",
  "Set the compositing color of the specified channel.",
  "This procedure sets the specified channel's compositing color.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  2,
  channel_set_color_inargs,
  0,
  NULL,
  { { channel_set_color_invoker } }
};

static Argument *
channel_get_tattoo_invoker (Gimp     *gimp,
                            Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  return_args = procedural_db_return_args (&channel_get_tattoo_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_item_get_tattoo (GIMP_ITEM (channel));

  return return_args;
}

static ProcArg channel_get_tattoo_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcArg channel_get_tattoo_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "tattoo",
    "The channel tattoo"
  }
};

static ProcRecord channel_get_tattoo_proc =
{
  "gimp_channel_get_tattoo",
  "Get the tattoo of the specified channel.",
  "This procedure returns the specified channel's tattoo. A tattoo is a unique and permanent identifier attached to a channel that can be used to uniquely identify a channel within an image even between sessions.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  GIMP_INTERNAL,
  1,
  channel_get_tattoo_inargs,
  1,
  channel_get_tattoo_outargs,
  { { channel_get_tattoo_invoker } }
};

static Argument *
channel_set_tattoo_invoker (Gimp     *gimp,
                            Argument *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gint32 tattoo;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_CHANNEL (channel))
    success = FALSE;

  tattoo = args[1].value.pdb_int;
  if (tattoo == 0)
    success = FALSE;

  if (success)
    gimp_item_set_tattoo (GIMP_ITEM (channel), tattoo);

  return procedural_db_return_args (&channel_set_tattoo_proc, success);
}

static ProcArg channel_set_tattoo_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_INT32,
    "tattoo",
    "The new channel tattoo"
  }
};

static ProcRecord channel_set_tattoo_proc =
{
  "gimp_channel_set_tattoo",
  "Set the tattoo of the specified channel.",
  "This procedure sets the specified channel's tattoo. A tattoo is a unique and permanent identifier attached to a channel that can be used to uniquely identify a channel within an image even between sessions.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  GIMP_INTERNAL,
  2,
  channel_set_tattoo_inargs,
  0,
  NULL,
  { { channel_set_tattoo_invoker } }
};
