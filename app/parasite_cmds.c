/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1999 Spencer Kimball and Peter Mattis
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

#include "procedural_db.h"

#include "drawable.h"
#include "gimpdrawable.h"
#include "gimpimage.h"
#include "gimpparasite.h"

#include "libgimp/parasite.h"

static ProcRecord parasite_new_proc;
static ProcRecord parasite_find_proc;
static ProcRecord parasite_attach_proc;
static ProcRecord parasite_detach_proc;
static ProcRecord parasite_list_proc;
static ProcRecord drawable_parasite_find_proc;
static ProcRecord drawable_parasite_attach_proc;
static ProcRecord drawable_parasite_detach_proc;
static ProcRecord drawable_parasite_list_proc;
static ProcRecord image_parasite_find_proc;
static ProcRecord image_parasite_attach_proc;
static ProcRecord image_parasite_detach_proc;
static ProcRecord image_parasite_list_proc;

void
register_parasite_procs (void)
{
  procedural_db_register (&parasite_new_proc);
  procedural_db_register (&parasite_find_proc);
  procedural_db_register (&parasite_attach_proc);
  procedural_db_register (&parasite_detach_proc);
  procedural_db_register (&parasite_list_proc);
  procedural_db_register (&drawable_parasite_find_proc);
  procedural_db_register (&drawable_parasite_attach_proc);
  procedural_db_register (&drawable_parasite_detach_proc);
  procedural_db_register (&drawable_parasite_list_proc);
  procedural_db_register (&image_parasite_find_proc);
  procedural_db_register (&image_parasite_attach_proc);
  procedural_db_register (&image_parasite_detach_proc);
  procedural_db_register (&image_parasite_list_proc);
}

static Argument *
parasite_new_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *name;
  gint32 flags;
  gint32 size;
  gchar *data;
  Parasite *parasite = NULL;

  name = (gchar *) args[0].value.pdb_pointer;

  flags = args[1].value.pdb_int;

  size = args[2].value.pdb_int;
  if (size < 0)
    success = FALSE;

  data = (gchar *) args[3].value.pdb_pointer;

  if (success)
    {
      if (size > 0 && data == NULL)
	success = FALSE;
      else
	success = (parasite = parasite_new (name, flags, size, data)) != NULL;
    }

  return_args = procedural_db_return_args (&parasite_new_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = parasite;

  return return_args;
}

static ProcArg parasite_new_inargs[] =
{
  {
    PDB_STRING,
    "name",
    "The name of the parasite to create"
  },
  {
    PDB_INT32,
    "flags",
    "The flags (1 == persistance)"
  },
  {
    PDB_INT32,
    "size",
    "The size of the data in bytes"
  },
  {
    PDB_STRING,
    "data",
    "The data"
  }
};

static ProcArg parasite_new_outargs[] =
{
  {
    PDB_PARASITE,
    "parasite",
    "The new parasite"
  }
};

static ProcRecord parasite_new_proc =
{
  "gimp_parasite_new",
  "Creates a new parasite.",
  "Creates a new parasite unatached to to any image or drawable.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  4,
  parasite_new_inargs,
  1,
  parasite_new_outargs,
  { { parasite_new_invoker } }
};

static Argument *
parasite_find_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *name;
  Parasite *parasite = NULL;

  name = (gchar *) args[0].value.pdb_pointer;

  parasite = parasite_copy (gimp_parasite_find (name));
  success = parasite != NULL;

  return_args = procedural_db_return_args (&parasite_find_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = parasite;

  return return_args;
}

static ProcArg parasite_find_inargs[] =
{
  {
    PDB_STRING,
    "name",
    "The name of the parasite to find"
  }
};

static ProcArg parasite_find_outargs[] =
{
  {
    PDB_PARASITE,
    "parasite",
    "The found parasite"
  }
};

static ProcRecord parasite_find_proc =
{
  "gimp_parasite_find",
  "Finds the named parasite.",
  "Finds and returns the named parasite that was previously attached to the gimp.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  1,
  parasite_find_inargs,
  1,
  parasite_find_outargs,
  { { parasite_find_invoker } }
};

static Argument *
parasite_attach_invoker (Argument *args)
{
  gboolean success = TRUE;
  Parasite *parasite;

  parasite = (Parasite *) args[0].value.pdb_pointer;
  if (parasite == NULL)
    success = FALSE;

  if (success)
    gimp_parasite_attach (parasite);

  return procedural_db_return_args (&parasite_attach_proc, success);
}

static ProcArg parasite_attach_inargs[] =
{
  {
    PDB_PARASITE,
    "parasite",
    "The parasite to attach to the gimp"
  }
};

static ProcRecord parasite_attach_proc =
{
  "gimp_parasite_attach",
  "Add a parasite to the gimp.",
  "This procedure attaches a parasite to the gimp. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  1,
  parasite_attach_inargs,
  0,
  NULL,
  { { parasite_attach_invoker } }
};

static Argument *
parasite_detach_invoker (Argument *args)
{
  gboolean success = TRUE;
  gchar *name;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    gimp_parasite_detach (name);

  return procedural_db_return_args (&parasite_detach_proc, success);
}

static ProcArg parasite_detach_inargs[] =
{
  {
    PDB_STRING,
    "name",
    "The name of the parasite to detach from the gimp."
  }
};

static ProcRecord parasite_detach_proc =
{
  "gimp_parasite_detach",
  "Removes a parasite from the gimp.",
  "This procedure detaches a parasite from the gimp. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  1,
  parasite_detach_inargs,
  0,
  NULL,
  { { parasite_detach_invoker } }
};

static Argument *
parasite_list_invoker (Argument *args)
{
  Argument *return_args;
  gint32 num_parasites;
  gchar **parasites;

  parasites = gimp_parasite_list (&num_parasites);

  return_args = procedural_db_return_args (&parasite_list_proc, TRUE);

  return_args[1].value.pdb_int = num_parasites;
  return_args[2].value.pdb_pointer = parasites;

  return return_args;
}

static ProcArg parasite_list_outargs[] =
{
  {
    PDB_INT32,
    "num_parasites",
    "The number of attached parasites"
  },
  {
    PDB_STRINGARRAY,
    "parasites",
    "The names of currently attached parasites"
  }
};

static ProcRecord parasite_list_proc =
{
  "gimp_parasite_list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  PDB_INTERNAL,
  0,
  NULL,
  2,
  parasite_list_outargs,
  { { parasite_list_invoker } }
};

static Argument *
drawable_parasite_find_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gchar *name;
  Parasite *parasite = NULL;

  drawable = gimp_drawable_get_ID (args[0].value.pdb_int);
  if (drawable == NULL)
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;

  if (success)
    {
      parasite = parasite_copy (gimp_drawable_parasite_find (drawable, name));
      success = parasite != NULL;
    }

  return_args = procedural_db_return_args (&drawable_parasite_find_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = parasite;

  return return_args;
}

static ProcArg drawable_parasite_find_inargs[] =
{
  {
    PDB_DRAWABLE,
    "drawable",
    "The drawable"
  },
  {
    PDB_STRING,
    "name",
    "The name of the parasite to find"
  }
};

static ProcArg drawable_parasite_find_outargs[] =
{
  {
    PDB_PARASITE,
    "parasite",
    "The found parasite"
  }
};

static ProcRecord drawable_parasite_find_proc =
{
  "gimp_drawable_parasite_find",
  "Finds the named parasite in a drawable",
  "Finds and returns the named parasite that was previously attached to a drawable.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  drawable_parasite_find_inargs,
  1,
  drawable_parasite_find_outargs,
  { { drawable_parasite_find_invoker } }
};

static Argument *
drawable_parasite_attach_invoker (Argument *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  Parasite *parasite;

  drawable = gimp_drawable_get_ID (args[0].value.pdb_int);
  if (drawable == NULL)
    success = FALSE;

  parasite = (Parasite *) args[1].value.pdb_pointer;
  if (parasite == NULL)
    success = FALSE;

  if (success)
    gimp_drawable_parasite_attach (drawable, parasite);

  return procedural_db_return_args (&drawable_parasite_attach_proc, success);
}

static ProcArg drawable_parasite_attach_inargs[] =
{
  {
    PDB_DRAWABLE,
    "drawable",
    "The drawable"
  },
  {
    PDB_PARASITE,
    "parasite",
    "The parasite to attach to a drawable"
  }
};

static ProcRecord drawable_parasite_attach_proc =
{
  "gimp_drawable_parasite_attach",
  "Add a parasite to a drawable.",
  "This procedure attaches a parasite to a drawable. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  drawable_parasite_attach_inargs,
  0,
  NULL,
  { { drawable_parasite_attach_invoker } }
};

static Argument *
drawable_parasite_detach_invoker (Argument *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  gchar *name;

  drawable = gimp_drawable_get_ID (args[0].value.pdb_int);
  if (drawable == NULL)
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    gimp_drawable_parasite_detach (drawable, name);

  return procedural_db_return_args (&drawable_parasite_detach_proc, success);
}

static ProcArg drawable_parasite_detach_inargs[] =
{
  {
    PDB_DRAWABLE,
    "drawable",
    "The drawable"
  },
  {
    PDB_STRING,
    "name",
    "The name of the parasite to detach from a drawable."
  }
};

static ProcRecord drawable_parasite_detach_proc =
{
  "gimp_drawable_parasite_detach",
  "Removes a parasite from a drawable.",
  "This procedure detaches a parasite from a drawable. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  drawable_parasite_detach_inargs,
  0,
  NULL,
  { { drawable_parasite_detach_invoker } }
};

static Argument *
drawable_parasite_list_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpDrawable *drawable;
  gint32 num_parasites;
  gchar **parasites = NULL;

  drawable = gimp_drawable_get_ID (args[0].value.pdb_int);
  if (drawable == NULL)
    success = FALSE;

  if (success)
    parasites = gimp_drawable_parasite_list (drawable, &num_parasites);

  return_args = procedural_db_return_args (&drawable_parasite_list_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = num_parasites;
      return_args[2].value.pdb_pointer = parasites;
    }

  return return_args;
}

static ProcArg drawable_parasite_list_inargs[] =
{
  {
    PDB_DRAWABLE,
    "drawable",
    "The drawable"
  }
};

static ProcArg drawable_parasite_list_outargs[] =
{
  {
    PDB_INT32,
    "num_parasites",
    "The number of attached parasites"
  },
  {
    PDB_STRINGARRAY,
    "parasites",
    "The names of currently attached parasites"
  }
};

static ProcRecord drawable_parasite_list_proc =
{
  "gimp_drawable_parasite_list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  PDB_INTERNAL,
  1,
  drawable_parasite_list_inargs,
  2,
  drawable_parasite_list_outargs,
  { { drawable_parasite_list_invoker } }
};

static Argument *
image_parasite_find_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gchar *name;
  Parasite *parasite = NULL;

  gimage = pdb_id_to_image (args[0].value.pdb_int);
  if (gimage == NULL)
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;

  if (success)
    {
      parasite = parasite_copy (gimp_image_parasite_find (gimage, name));
      success = parasite != NULL;
    }

  return_args = procedural_db_return_args (&image_parasite_find_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = parasite;

  return return_args;
}

static ProcArg image_parasite_find_inargs[] =
{
  {
    PDB_IMAGE,
    "image",
    "The image"
  },
  {
    PDB_STRING,
    "name",
    "The name of the parasite to find"
  }
};

static ProcArg image_parasite_find_outargs[] =
{
  {
    PDB_PARASITE,
    "parasite",
    "The found parasite"
  }
};

static ProcRecord image_parasite_find_proc =
{
  "gimp_image_parasite_find",
  "Finds the named parasite in an image",
  "Finds and returns the named parasite that was previously attached to an image.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  image_parasite_find_inargs,
  1,
  image_parasite_find_outargs,
  { { image_parasite_find_invoker } }
};

static Argument *
image_parasite_attach_invoker (Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  Parasite *parasite;

  gimage = pdb_id_to_image (args[0].value.pdb_int);
  if (gimage == NULL)
    success = FALSE;

  parasite = (Parasite *) args[1].value.pdb_pointer;
  if (parasite == NULL)
    success = FALSE;

  if (success)
    gimp_image_parasite_attach (gimage, parasite);

  return procedural_db_return_args (&image_parasite_attach_proc, success);
}

static ProcArg image_parasite_attach_inargs[] =
{
  {
    PDB_IMAGE,
    "image",
    "The image"
  },
  {
    PDB_PARASITE,
    "parasite",
    "The parasite to attach to an image"
  }
};

static ProcRecord image_parasite_attach_proc =
{
  "gimp_image_parasite_attach",
  "Add a parasite to an image.",
  "This procedure attaches a parasite to an image. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  image_parasite_attach_inargs,
  0,
  NULL,
  { { image_parasite_attach_invoker } }
};

static Argument *
image_parasite_detach_invoker (Argument *args)
{
  gboolean success = TRUE;
  GimpImage *gimage;
  gchar *name;

  gimage = pdb_id_to_image (args[0].value.pdb_int);
  if (gimage == NULL)
    success = FALSE;

  name = (gchar *) args[1].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  if (success)
    gimp_image_parasite_detach (gimage, name);

  return procedural_db_return_args (&image_parasite_detach_proc, success);
}

static ProcArg image_parasite_detach_inargs[] =
{
  {
    PDB_IMAGE,
    "image",
    "The image"
  },
  {
    PDB_STRING,
    "name",
    "The name of the parasite to detach from an image."
  }
};

static ProcRecord image_parasite_detach_proc =
{
  "gimp_image_parasite_detach",
  "Removes a parasite from an image.",
  "This procedure detaches a parasite from an image. It has no return values.",
  "Jay Cox",
  "Jay Cox",
  "1998",
  PDB_INTERNAL,
  2,
  image_parasite_detach_inargs,
  0,
  NULL,
  { { image_parasite_detach_invoker } }
};

static Argument *
image_parasite_list_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpImage *gimage;
  gint32 num_parasites;
  gchar **parasites = NULL;

  gimage = pdb_id_to_image (args[0].value.pdb_int);
  if (gimage == NULL)
    success = FALSE;

  if (success)
    parasites = gimp_image_parasite_list (gimage, &num_parasites);

  return_args = procedural_db_return_args (&image_parasite_list_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = num_parasites;
      return_args[2].value.pdb_pointer = parasites;
    }

  return return_args;
}

static ProcArg image_parasite_list_inargs[] =
{
  {
    PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcArg image_parasite_list_outargs[] =
{
  {
    PDB_INT32,
    "num_parasites",
    "The number of attached parasites"
  },
  {
    PDB_STRINGARRAY,
    "parasites",
    "The names of currently attached parasites"
  }
};

static ProcRecord image_parasite_list_proc =
{
  "gimp_image_parasite_list",
  "List all parasites.",
  "Returns a list of all currently attached parasites.",
  "Marc Lehmann",
  "Marc Lehmann",
  "1999",
  PDB_INTERNAL,
  1,
  image_parasite_list_inargs,
  2,
  image_parasite_list_outargs,
  { { image_parasite_list_invoker } }
};
