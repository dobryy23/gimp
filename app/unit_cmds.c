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

#include "procedural_db.h"

#include "libgimp/gimpunit.h"

static ProcRecord unit_get_number_of_units_proc;
static ProcRecord unit_get_number_of_built_in_units_proc;
static ProcRecord unit_new_proc;
static ProcRecord unit_get_deletion_flag_proc;
static ProcRecord unit_set_deletion_flag_proc;
static ProcRecord unit_get_identifier_proc;
static ProcRecord unit_get_factor_proc;
static ProcRecord unit_get_digits_proc;
static ProcRecord unit_get_symbol_proc;
static ProcRecord unit_get_abbreviation_proc;
static ProcRecord unit_get_singular_proc;
static ProcRecord unit_get_plural_proc;

void
register_unit_procs (void)
{
  procedural_db_register (&unit_get_number_of_units_proc);
  procedural_db_register (&unit_get_number_of_built_in_units_proc);
  procedural_db_register (&unit_new_proc);
  procedural_db_register (&unit_get_deletion_flag_proc);
  procedural_db_register (&unit_set_deletion_flag_proc);
  procedural_db_register (&unit_get_identifier_proc);
  procedural_db_register (&unit_get_factor_proc);
  procedural_db_register (&unit_get_digits_proc);
  procedural_db_register (&unit_get_symbol_proc);
  procedural_db_register (&unit_get_abbreviation_proc);
  procedural_db_register (&unit_get_singular_proc);
  procedural_db_register (&unit_get_plural_proc);
}

static Argument *
unit_get_number_of_units_invoker (Argument *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&unit_get_number_of_units_proc, TRUE);
  return_args[1].value.pdb_int = gimp_unit_get_number_of_units ();

  return return_args;
}

static ProcArg unit_get_number_of_units_outargs[] =
{
  {
    PDB_INT32,
    "num_units",
    "The number of units"
  }
};

static ProcRecord unit_get_number_of_units_proc =
{
  "gimp_unit_get_number_of_units",
  "Returns the number of units.",
  "This procedure returns the number of defined units.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  unit_get_number_of_units_outargs,
  { { unit_get_number_of_units_invoker } }
};

static Argument *
unit_get_number_of_built_in_units_invoker (Argument *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&unit_get_number_of_built_in_units_proc, TRUE);
  return_args[1].value.pdb_int = gimp_unit_get_number_of_units ();

  return return_args;
}

static ProcArg unit_get_number_of_built_in_units_outargs[] =
{
  {
    PDB_INT32,
    "num_units",
    "The number of built-in units"
  }
};

static ProcRecord unit_get_number_of_built_in_units_proc =
{
  "gimp_unit_get_number_of_built_in_units",
  "Returns the number of built-in units.",
  "This procedure returns the number of defined units built-in to the GIMP.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  0,
  NULL,
  1,
  unit_get_number_of_built_in_units_outargs,
  { { unit_get_number_of_built_in_units_invoker } }
};

static Argument *
unit_new_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *identifier;
  gdouble factor;
  gint32 digits;
  gchar *symbol;
  gchar *abbreviation;
  gchar *singular;
  gchar *plural;
  GimpUnit unit = 0;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL)
    success = FALSE;

  factor = args[1].value.pdb_float;

  digits = args[2].value.pdb_int;

  symbol = (gchar *) args[3].value.pdb_pointer;
  if (symbol == NULL)
    success = FALSE;

  abbreviation = (gchar *) args[4].value.pdb_pointer;
  if (abbreviation == NULL)
    success = FALSE;

  singular = (gchar *) args[5].value.pdb_pointer;
  if (singular == NULL)
    success = FALSE;

  plural = (gchar *) args[6].value.pdb_pointer;
  if (plural == NULL)
    success = FALSE;

  if (success)
    unit = gimp_unit_new (identifier, factor, digits, symbol, abbreviation,
			  singular, plural);

  return_args = procedural_db_return_args (&unit_new_proc, success);

  if (success)
    return_args[1].value.pdb_int = unit;

  return return_args;
}

static ProcArg unit_new_inargs[] =
{
  {
    PDB_STRING,
    "identifier",
    "The new unit's identifier"
  },
  {
    PDB_FLOAT,
    "factor",
    "The new unit's factor"
  },
  {
    PDB_INT32,
    "digits",
    "The new unit's digits"
  },
  {
    PDB_STRING,
    "symbol",
    "The new unit's symbol"
  },
  {
    PDB_STRING,
    "abbreviation",
    "The new unit's abbreviation"
  },
  {
    PDB_STRING,
    "singular",
    "The new unit's singular form"
  },
  {
    PDB_STRING,
    "plural",
    "The new unit's plural form"
  }
};

static ProcArg unit_new_outargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The new unit's ID"
  }
};

static ProcRecord unit_new_proc =
{
  "gimp_unit_new",
  "Creates a new unit and returns it's integer ID.",
  "This procedure creates a new unit and returns it's integer ID. Note that the new unit will have it's deletion flag set to TRUE, so you will have to set it to FALSE with gimp_unit_set_deletion_flag to make it persistent.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  7,
  unit_new_inargs,
  1,
  unit_new_outargs,
  { { unit_new_invoker } }
};

static Argument *
unit_get_deletion_flag_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_deletion_flag_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_unit_get_deletion_flag (unit);

  return return_args;
}

static ProcArg unit_get_deletion_flag_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_deletion_flag_outargs[] =
{
  {
    PDB_INT32,
    "deletion_flag",
    "The unit's deletion flag"
  }
};

static ProcRecord unit_get_deletion_flag_proc =
{
  "gimp_unit_get_deletion_flag",
  "Returns the deletion flag of the unit.",
  "This procedure returns the deletion flag of the unit. If this value is TRUE the unit's definition will not be saved in the user's unitrc file on gimp exit.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_deletion_flag_inargs,
  1,
  unit_get_deletion_flag_outargs,
  { { unit_get_deletion_flag_invoker } }
};

static Argument *
unit_set_deletion_flag_invoker (Argument *args)
{
  gboolean success = TRUE;
  GimpUnit unit;
  gboolean deletion_flag;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  deletion_flag = args[1].value.pdb_int ? TRUE : FALSE;

  if (success)
    gimp_unit_set_deletion_flag (unit, deletion_flag);

  return procedural_db_return_args (&unit_set_deletion_flag_proc, success);
}

static ProcArg unit_set_deletion_flag_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  },
  {
    PDB_INT32,
    "deletion_flag",
    "The new deletion flag of the unit"
  }
};

static ProcRecord unit_set_deletion_flag_proc =
{
  "gimp_unit_set_deletion_flag",
  "Sets the deletion flag of a unit.",
  "This procedure sets the unit's deletion flag. If the deletion flag of a unit is TRUE on gimp exit, this unit's definition will not be saved in the user's unitrc.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  2,
  unit_set_deletion_flag_inargs,
  0,
  NULL,
  { { unit_set_deletion_flag_invoker } }
};

static Argument *
unit_get_identifier_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_identifier_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_unit_get_identifier (unit));

  return return_args;
}

static ProcArg unit_get_identifier_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_identifier_outargs[] =
{
  {
    PDB_STRING,
    "identifier",
    "The unit's textual identifier"
  }
};

static ProcRecord unit_get_identifier_proc =
{
  "gimp_unit_get_identifier",
  "Returns the textual identifier of the unit.",
  "This procedure returns the textual identifier of the unit. For built-in units it will be the english singular form of the unit's name. For user-defined units this should equal to the singular form.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_identifier_inargs,
  1,
  unit_get_identifier_outargs,
  { { unit_get_identifier_invoker } }
};

static Argument *
unit_get_factor_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_factor_proc, success);

  if (success)
    return_args[1].value.pdb_float = gimp_unit_get_factor (unit);

  return return_args;
}

static ProcArg unit_get_factor_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_factor_outargs[] =
{
  {
    PDB_FLOAT,
    "factor",
    "The unit's factor"
  }
};

static ProcRecord unit_get_factor_proc =
{
  "gimp_unit_get_factor",
  "Returns the factor of the unit.",
  "This procedure returns the unit's factor which indicates how many units make up an inch. Note that asking for the factor of \"pixels\" will produce an error.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_factor_inargs,
  1,
  unit_get_factor_outargs,
  { { unit_get_factor_invoker } }
};

static Argument *
unit_get_digits_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_digits_proc, success);

  if (success)
    return_args[1].value.pdb_int = gimp_unit_get_digits (unit);

  return return_args;
}

static ProcArg unit_get_digits_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_digits_outargs[] =
{
  {
    PDB_INT32,
    "digits",
    "The unit's number of digits"
  }
};

static ProcRecord unit_get_digits_proc =
{
  "gimp_unit_get_digits",
  "Returns the number of digits of the unit.",
  "This procedure returns the number of digits you should provide in input or output functions to get approximately the same accuracy as with two digits and inches. Note that asking for the digits of \"pixels\" will produce an error.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_digits_inargs,
  1,
  unit_get_digits_outargs,
  { { unit_get_digits_invoker } }
};

static Argument *
unit_get_symbol_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_symbol_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_unit_get_symbol (unit));

  return return_args;
}

static ProcArg unit_get_symbol_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_symbol_outargs[] =
{
  {
    PDB_STRING,
    "symbol",
    "The unit's symbol"
  }
};

static ProcRecord unit_get_symbol_proc =
{
  "gimp_unit_get_symbol",
  "Returns the symbol of the unit.",
  "This procedure returns the symbol of the unit (\"''\" for inches).",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_symbol_inargs,
  1,
  unit_get_symbol_outargs,
  { { unit_get_symbol_invoker } }
};

static Argument *
unit_get_abbreviation_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_abbreviation_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_unit_get_abbreviation (unit));

  return return_args;
}

static ProcArg unit_get_abbreviation_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_abbreviation_outargs[] =
{
  {
    PDB_STRING,
    "abbreviation",
    "The unit's abbreviation"
  }
};

static ProcRecord unit_get_abbreviation_proc =
{
  "gimp_unit_get_abbreviation",
  "Returns the abbreviation of the unit.",
  "This procedure returns the abbreviation of the unit (\"in\" for inches).",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_abbreviation_inargs,
  1,
  unit_get_abbreviation_outargs,
  { { unit_get_abbreviation_invoker } }
};

static Argument *
unit_get_singular_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_singular_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_unit_get_singular (unit));

  return return_args;
}

static ProcArg unit_get_singular_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_singular_outargs[] =
{
  {
    PDB_STRING,
    "singular",
    "The unit's singular form"
  }
};

static ProcRecord unit_get_singular_proc =
{
  "gimp_unit_get_singular",
  "Returns the singular form of the unit.",
  "This procedure returns the singular form of the unit.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_singular_inargs,
  1,
  unit_get_singular_outargs,
  { { unit_get_singular_invoker } }
};

static Argument *
unit_get_plural_invoker (Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  GimpUnit unit;

  unit = args[0].value.pdb_int;
  if (unit < GIMP_UNIT_PIXEL || unit >= gimp_unit_get_number_of_units ())
    success = FALSE;

  return_args = procedural_db_return_args (&unit_get_plural_proc, success);

  if (success)
    return_args[1].value.pdb_pointer = g_strdup (gimp_unit_get_plural (unit));

  return return_args;
}

static ProcArg unit_get_plural_inargs[] =
{
  {
    PDB_INT32,
    "unit_id",
    "The unit's integer ID"
  }
};

static ProcArg unit_get_plural_outargs[] =
{
  {
    PDB_STRING,
    "plural",
    "The unit's plural form"
  }
};

static ProcRecord unit_get_plural_proc =
{
  "gimp_unit_get_plural",
  "Returns the plural form of the unit.",
  "This procedure returns the plural form of the unit.",
  "Michael Natterer",
  "Michael Natterer",
  "1999",
  PDB_INTERNAL,
  1,
  unit_get_plural_inargs,
  1,
  unit_get_plural_outargs,
  { { unit_get_plural_invoker } }
};
