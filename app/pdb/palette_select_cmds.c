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


#include <gtk/gtk.h>

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "gui/gui-types.h"
#include "procedural_db.h"

#include "core/gimp.h"
#include "core/gimpcontainer.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "gui/palette-select.h"

static ProcRecord palettes_popup_proc;
static ProcRecord palettes_close_popup_proc;
static ProcRecord palettes_set_popup_proc;

void
register_palette_select_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &palettes_popup_proc);
  procedural_db_register (gimp, &palettes_close_popup_proc);
  procedural_db_register (gimp, &palettes_set_popup_proc);
}

static Argument *
palettes_popup_invoker (Gimp     *gimp,
                        Argument *args)
{
  gboolean success = TRUE;
  gchar *palette_callback;
  gchar *popup_title;
  gchar *initial_palette;
  ProcRecord *proc;

  palette_callback = (gchar *) args[0].value.pdb_pointer;
  if (palette_callback == NULL)
    success = FALSE;

  popup_title = (gchar *) args[1].value.pdb_pointer;
  if (popup_title == NULL)
    success = FALSE;

  initial_palette = (gchar *) args[2].value.pdb_pointer;

  if (success)
    {
      if (! gimp->no_interface &&
	  (proc = procedural_db_lookup (gimp, palette_callback)))
	{
	  palette_select_new (gimp, popup_title,
			      initial_palette,
			      palette_callback);
	}
      else
	{
	  success = FALSE;
	}
    }

  return procedural_db_return_args (&palettes_popup_proc, success);
}

static ProcArg palettes_popup_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "palette_callback",
    "The callback PDB proc to call when palette selection is made"
  },
  {
    GIMP_PDB_STRING,
    "popup_title",
    "Title to give the palette popup window"
  },
  {
    GIMP_PDB_STRING,
    "initial_palette",
    "The name of the palette to set as the first selected"
  }
};

static ProcRecord palettes_popup_proc =
{
  "gimp_palettes_popup",
  "Invokes the Gimp palette selection.",
  "This procedure popups the palette selection dialog.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  GIMP_INTERNAL,
  3,
  palettes_popup_inargs,
  0,
  NULL,
  { { palettes_popup_invoker } }
};

static Argument *
palettes_close_popup_invoker (Gimp     *gimp,
                              Argument *args)
{
  gboolean success = TRUE;
  gchar *palette_callback;
  ProcRecord *proc;
  PaletteSelect *psp;

  palette_callback = (gchar *) args[0].value.pdb_pointer;
  if (palette_callback == NULL)
    success = FALSE;

  if (success)
    {
      if (! gimp->no_interface &&
	  (proc = procedural_db_lookup (gimp, palette_callback)) &&
	  (psp = palette_select_get_by_callback (palette_callback)))
	{
	  palette_select_free (psp);
	}
      else
	{
	  success = FALSE;
	}
    }

  return procedural_db_return_args (&palettes_close_popup_proc, success);
}

static ProcArg palettes_close_popup_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "palette_callback",
    "The name of the callback registered for this popup"
  }
};

static ProcRecord palettes_close_popup_proc =
{
  "gimp_palettes_close_popup",
  "Popdown the Gimp palette selection.",
  "This procedure closes an opened palette selection dialog.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  GIMP_INTERNAL,
  1,
  palettes_close_popup_inargs,
  0,
  NULL,
  { { palettes_close_popup_invoker } }
};

static Argument *
palettes_set_popup_invoker (Gimp     *gimp,
                            Argument *args)
{
  gboolean success = TRUE;
  gchar *palette_callback;
  gchar *palette_name;
  ProcRecord *proc;
  PaletteSelect *psp;

  palette_callback = (gchar *) args[0].value.pdb_pointer;
  if (palette_callback == NULL)
    success = FALSE;

  palette_name = (gchar *) args[1].value.pdb_pointer;
  if (palette_name == NULL)
    success = FALSE;

  if (success)
    {
      if (! gimp->no_interface &&
	  (proc = procedural_db_lookup (gimp, palette_callback)) &&
	  (psp = palette_select_get_by_callback (palette_callback)))
	{
	  GimpPalette *active = (GimpPalette *)
	    gimp_container_get_child_by_name (gimp->palette_factory->container,
					      palette_name);
    
	  success = (active != NULL);
    
	  if (success)
	    gimp_context_set_palette (psp->context, active);
	}
      else
	success = FALSE;
    }

  return procedural_db_return_args (&palettes_set_popup_proc, success);
}

static ProcArg palettes_set_popup_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "palette_callback",
    "The name of the callback registered for this popup"
  },
  {
    GIMP_PDB_STRING,
    "palette_name",
    "The name of the palette to set as selected"
  }
};

static ProcRecord palettes_set_popup_proc =
{
  "gimp_palettes_set_popup",
  "Sets the current palette selection in a popup.",
  "Sets the current palette selection in a popup.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  GIMP_INTERNAL,
  2,
  palettes_set_popup_inargs,
  0,
  NULL,
  { { palettes_set_popup_invoker } }
};
