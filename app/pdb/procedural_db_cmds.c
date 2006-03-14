/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "core/gimp.h"
#include "plug-in/plug-in-data.h"
#include "procedural-db-query.h"

static ProcRecord procedural_db_temp_name_proc;
static ProcRecord procedural_db_dump_proc;
static ProcRecord procedural_db_query_proc;
static ProcRecord procedural_db_proc_info_proc;
static ProcRecord procedural_db_proc_arg_proc;
static ProcRecord procedural_db_proc_val_proc;
static ProcRecord procedural_db_get_data_proc;
static ProcRecord procedural_db_get_data_size_proc;
static ProcRecord procedural_db_set_data_proc;

void
register_procedural_db_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &procedural_db_temp_name_proc);
  procedural_db_register (gimp, &procedural_db_dump_proc);
  procedural_db_register (gimp, &procedural_db_query_proc);
  procedural_db_register (gimp, &procedural_db_proc_info_proc);
  procedural_db_register (gimp, &procedural_db_proc_arg_proc);
  procedural_db_register (gimp, &procedural_db_proc_val_proc);
  procedural_db_register (gimp, &procedural_db_get_data_proc);
  procedural_db_register (gimp, &procedural_db_get_data_size_proc);
  procedural_db_register (gimp, &procedural_db_set_data_proc);
}

static Argument *
procedural_db_temp_name_invoker (Gimp         *gimp,
                                 GimpContext  *context,
                                 GimpProgress *progress,
                                 Argument     *args)
{
  Argument *return_args;
  gchar *temp_name;

  static gint proc_number = 0;

  temp_name = g_strdup_printf ("temp-procedure-number-%d", proc_number++);

  return_args = procedural_db_return_args (&procedural_db_temp_name_proc, TRUE);
  return_args[1].value.pdb_pointer = temp_name;

  return return_args;
}

static ProcArg procedural_db_temp_name_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "temp-name",
    "A unique temporary name for a temporary PDB entry"
  }
};

static ProcRecord procedural_db_temp_name_proc =
{
  "gimp-procedural-db-temp-name",
  "gimp-procedural-db-temp-name",
  "Generates a unique temporary PDB name.",
  "This procedure generates a temporary PDB entry name that is guaranteed to be unique. It is mainly used by the interactive popup dialogs to generate a PDB entry name.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0,
  NULL,
  1,
  procedural_db_temp_name_outargs,
  { { procedural_db_temp_name_invoker } }
};

static Argument *
procedural_db_dump_invoker (Gimp         *gimp,
                            GimpContext  *context,
                            GimpProgress *progress,
                            Argument     *args)
{
  gboolean success = TRUE;
  gchar *filename;

  filename = (gchar *) args[0].value.pdb_pointer;
  if (filename == NULL)
    success = FALSE;

  if (success)
    {
      success = procedural_db_dump (gimp, filename);
    }

  return procedural_db_return_args (&procedural_db_dump_proc, success);
}

static ProcArg procedural_db_dump_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "filename",
    "The dump filename"
  }
};

static ProcRecord procedural_db_dump_proc =
{
  "gimp-procedural-db-dump",
  "gimp-procedural-db-dump",
  "Dumps the current contents of the procedural database",
  "This procedure dumps the contents of the procedural database to the specified file. The file will contain all of the information provided for each registered procedure. This file is in a format appropriate for use with the supplied \"pdb_self_doc.el\" Elisp script, which generates a texinfo document.",
  "Spencer Kimball & Josh MacDonald",
  "Spencer Kimball & Josh MacDonald & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  procedural_db_dump_inargs,
  0,
  NULL,
  { { procedural_db_dump_invoker } }
};

static Argument *
procedural_db_query_invoker (Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *name;
  gchar *blurb;
  gchar *help;
  gchar *author;
  gchar *copyright;
  gchar *date;
  gchar *proc_type;
  gint num_procs;
  gchar **procs;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name == NULL)
    success = FALSE;

  blurb = (gchar *) args[1].value.pdb_pointer;
  if (blurb == NULL)
    success = FALSE;

  help = (gchar *) args[2].value.pdb_pointer;
  if (help == NULL)
    success = FALSE;

  author = (gchar *) args[3].value.pdb_pointer;
  if (author == NULL)
    success = FALSE;

  copyright = (gchar *) args[4].value.pdb_pointer;
  if (copyright == NULL)
    success = FALSE;

  date = (gchar *) args[5].value.pdb_pointer;
  if (date == NULL)
    success = FALSE;

  proc_type = (gchar *) args[6].value.pdb_pointer;
  if (proc_type == NULL)
    success = FALSE;

  if (success)
    {
      success = procedural_db_query (gimp,
                                     name, blurb, help, author, copyright, date, proc_type,
                                     &num_procs, &procs);
    }

  return_args = procedural_db_return_args (&procedural_db_query_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = num_procs;
      return_args[2].value.pdb_pointer = procs;
    }

  return return_args;
}

static ProcArg procedural_db_query_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The regex for procedure name"
  },
  {
    GIMP_PDB_STRING,
    "blurb",
    "The regex for procedure blurb"
  },
  {
    GIMP_PDB_STRING,
    "help",
    "The regex for procedure help"
  },
  {
    GIMP_PDB_STRING,
    "author",
    "The regex for procedure author"
  },
  {
    GIMP_PDB_STRING,
    "copyright",
    "The regex for procedure copyright"
  },
  {
    GIMP_PDB_STRING,
    "date",
    "The regex for procedure date"
  },
  {
    GIMP_PDB_STRING,
    "proc-type",
    "The regex for procedure type: { 'Internal GIMP procedure', 'GIMP Plug-In', 'GIMP Extension', 'Temporary Procedure' }"
  }
};

static ProcArg procedural_db_query_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num-matches",
    "The number of matching procedures"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "procedure-names",
    "The list of procedure names"
  }
};

static ProcRecord procedural_db_query_proc =
{
  "gimp-procedural-db-query",
  "gimp-procedural-db-query",
  "Queries the procedural database for its contents using regular expression matching.",
  "This procedure queries the contents of the procedural database. It is supplied with seven arguments matching procedures on { name, blurb, help, author, copyright, date, procedure type}. This is accomplished using regular expression matching. For instance, to find all procedures with \"jpeg\" listed in the blurb, all seven arguments can be supplied as \".*\", except for the second, which can be supplied as \".*jpeg.*\". There are two return arguments for this procedure. The first is the number of procedures matching the query. The second is a concatenated list of procedure names corresponding to those matching the query. If no matching entries are found, then the returned string is NULL and the number of entries is 0.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  7,
  procedural_db_query_inargs,
  2,
  procedural_db_query_outargs,
  { { procedural_db_query_invoker } }
};

static Argument *
procedural_db_proc_info_invoker (Gimp         *gimp,
                                 GimpContext  *context,
                                 GimpProgress *progress,
                                 Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  gchar *blurb;
  gchar *help;
  gchar *author;
  gchar *copyright;
  gchar *date;
  gint32 proc_type = 0;
  gint32 num_args;
  gint32 num_values;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL || !g_utf8_validate (proc_name, -1, NULL))
    success = FALSE;

  if (success)
    {
      GimpPDBProcType  ptype;
      gchar           *canonical;

      canonical = gimp_canonicalize_identifier (proc_name);

      success = procedural_db_proc_info (gimp, canonical,
                                         &blurb, &help, &author, &copyright, &date, &ptype,
                                         &num_args, &num_values);
      proc_type = ptype;

      g_free (canonical);
    }

  return_args = procedural_db_return_args (&procedural_db_proc_info_proc, success);

  if (success)
    {
      return_args[1].value.pdb_pointer = blurb;
      return_args[2].value.pdb_pointer = help;
      return_args[3].value.pdb_pointer = author;
      return_args[4].value.pdb_pointer = copyright;
      return_args[5].value.pdb_pointer = date;
      return_args[6].value.pdb_int = proc_type;
      return_args[7].value.pdb_int = num_args;
      return_args[8].value.pdb_int = num_values;
    }

  return return_args;
}

static ProcArg procedural_db_proc_info_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "procedure",
    "The procedure name"
  }
};

static ProcArg procedural_db_proc_info_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "blurb",
    "A short blurb"
  },
  {
    GIMP_PDB_STRING,
    "help",
    "Detailed procedure help"
  },
  {
    GIMP_PDB_STRING,
    "author",
    "Author(s) of the procedure"
  },
  {
    GIMP_PDB_STRING,
    "copyright",
    "The copyright"
  },
  {
    GIMP_PDB_STRING,
    "date",
    "Copyright date"
  },
  {
    GIMP_PDB_INT32,
    "proc-type",
    "The procedure type: { GIMP_INTERNAL (0), GIMP_PLUGIN (1), GIMP_EXTENSION (2), GIMP_TEMPORARY (3) }"
  },
  {
    GIMP_PDB_INT32,
    "num-args",
    "The number of input arguments"
  },
  {
    GIMP_PDB_INT32,
    "num-values",
    "The number of return values"
  }
};

static ProcRecord procedural_db_proc_info_proc =
{
  "gimp-procedural-db-proc-info",
  "gimp-procedural-db-proc-info",
  "Queries the procedural database for information on the specified procedure.",
  "This procedure returns information on the specified procedure. A short blurb, detailed help, author(s), copyright information, procedure type, number of input, and number of return values are returned. For specific information on each input argument and return value, use the 'gimp_procedural_db_proc_arg' and 'gimp_procedural_db_proc_val' procedures.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  NULL,
  GIMP_INTERNAL,
  1,
  procedural_db_proc_info_inargs,
  8,
  procedural_db_proc_info_outargs,
  { { procedural_db_proc_info_invoker } }
};

static Argument *
procedural_db_proc_arg_invoker (Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  gint32 arg_num;
  ProcRecord *proc;
  ProcArg *arg = NULL;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL || !g_utf8_validate (proc_name, -1, NULL))
    success = FALSE;

  arg_num = args[1].value.pdb_int;

  if (success)
    {
      gchar *canonical;

      canonical = gimp_canonicalize_identifier (proc_name);

      proc = procedural_db_lookup (gimp, canonical);

      if (! proc)
        {
          const gchar *compat_name;

          compat_name = g_hash_table_lookup (gimp->procedural_compat_ht, canonical);

          if (compat_name)
            proc = procedural_db_lookup (gimp, compat_name);
        }

      g_free (canonical);

      if (proc && (arg_num >= 0 && arg_num < proc->num_args))
        arg = &proc->args[arg_num];
      else
        success = FALSE;
    }

  return_args = procedural_db_return_args (&procedural_db_proc_arg_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = arg->arg_type;
      return_args[2].value.pdb_pointer = g_strdup (arg->name);
      return_args[3].value.pdb_pointer = g_strdup (arg->description);
    }

  return return_args;
}

static ProcArg procedural_db_proc_arg_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "procedure",
    "The procedure name"
  },
  {
    GIMP_PDB_INT32,
    "arg-num",
    "The argument number"
  }
};

static ProcArg procedural_db_proc_arg_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "arg-type",
    "The type of argument { GIMP_PDB_INT32 (0), GIMP_PDB_INT16 (1), GIMP_PDB_INT8 (2), GIMP_PDB_FLOAT (3), GIMP_PDB_STRING (4), GIMP_PDB_INT32ARRAY (5), GIMP_PDB_INT16ARRAY (6), GIMP_PDB_INT8ARRAY (7), GIMP_PDB_FLOATARRAY (8), GIMP_PDB_STRINGARRAY (9), GIMP_PDB_COLOR (10), GIMP_PDB_REGION (11), GIMP_PDB_DISPLAY (12), GIMP_PDB_IMAGE (13), GIMP_PDB_LAYER (14), GIMP_PDB_CHANNEL (15), GIMP_PDB_DRAWABLE (16), GIMP_PDB_SELECTION (17), GIMP_PDB_BOUNDARY (18), GIMP_PDB_VECTORS (19), GIMP_PDB_PARASITE (20), GIMP_PDB_STATUS (21), GIMP_PDB_PATH (GIMP_PDB_VECTORS) }"
  },
  {
    GIMP_PDB_STRING,
    "arg-name",
    "The name of the argument"
  },
  {
    GIMP_PDB_STRING,
    "arg-desc",
    "A description of the argument"
  }
};

static ProcRecord procedural_db_proc_arg_proc =
{
  "gimp-procedural-db-proc-arg",
  "gimp-procedural-db-proc-arg",
  "Queries the procedural database for information on the specified procedure's argument.",
  "This procedure returns information on the specified procedure's argument. The argument type, name, and a description are retrieved.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  NULL,
  GIMP_INTERNAL,
  2,
  procedural_db_proc_arg_inargs,
  3,
  procedural_db_proc_arg_outargs,
  { { procedural_db_proc_arg_invoker } }
};

static Argument *
procedural_db_proc_val_invoker (Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  gint32 val_num;
  ProcRecord *proc;
  ProcArg *val = NULL;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL || !g_utf8_validate (proc_name, -1, NULL))
    success = FALSE;

  val_num = args[1].value.pdb_int;

  if (success)
    {
      gchar *canonical;

      canonical = gimp_canonicalize_identifier (proc_name);

      proc = procedural_db_lookup (gimp, canonical);

      if (! proc)
        {
          const gchar *compat_name;

          compat_name = g_hash_table_lookup (gimp->procedural_compat_ht, canonical);

          if (compat_name)
            proc = procedural_db_lookup (gimp, compat_name);
        }

      g_free (canonical);

      if (proc && (val_num >= 0 && val_num < proc->num_values))
        val = &proc->values[val_num];
      else
        success = FALSE;
    }

  return_args = procedural_db_return_args (&procedural_db_proc_val_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = val->arg_type;
      return_args[2].value.pdb_pointer = g_strdup (val->name);
      return_args[3].value.pdb_pointer = g_strdup (val->description);
    }

  return return_args;
}

static ProcArg procedural_db_proc_val_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "procedure",
    "The procedure name"
  },
  {
    GIMP_PDB_INT32,
    "val-num",
    "The return value number"
  }
};

static ProcArg procedural_db_proc_val_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "val-type",
    "The type of return value { GIMP_PDB_INT32 (0), GIMP_PDB_INT16 (1), GIMP_PDB_INT8 (2), GIMP_PDB_FLOAT (3), GIMP_PDB_STRING (4), GIMP_PDB_INT32ARRAY (5), GIMP_PDB_INT16ARRAY (6), GIMP_PDB_INT8ARRAY (7), GIMP_PDB_FLOATARRAY (8), GIMP_PDB_STRINGARRAY (9), GIMP_PDB_COLOR (10), GIMP_PDB_REGION (11), GIMP_PDB_DISPLAY (12), GIMP_PDB_IMAGE (13), GIMP_PDB_LAYER (14), GIMP_PDB_CHANNEL (15), GIMP_PDB_DRAWABLE (16), GIMP_PDB_SELECTION (17), GIMP_PDB_BOUNDARY (18), GIMP_PDB_VECTORS (19), GIMP_PDB_PARASITE (20), GIMP_PDB_STATUS (21), GIMP_PDB_PATH (GIMP_PDB_VECTORS) }"
  },
  {
    GIMP_PDB_STRING,
    "val-name",
    "The name of the return value"
  },
  {
    GIMP_PDB_STRING,
    "val-desc",
    "A description of the return value"
  }
};

static ProcRecord procedural_db_proc_val_proc =
{
  "gimp-procedural-db-proc-val",
  "gimp-procedural-db-proc-val",
  "Queries the procedural database for information on the specified procedure's return value.",
  "This procedure returns information on the specified procedure's return value. The return value type, name, and a description are retrieved.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  NULL,
  GIMP_INTERNAL,
  2,
  procedural_db_proc_val_inargs,
  3,
  procedural_db_proc_val_outargs,
  { { procedural_db_proc_val_invoker } }
};

static Argument *
procedural_db_get_data_invoker (Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *identifier;
  gint32 bytes;
  guint8 *data_copy = NULL;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL || !g_utf8_validate (identifier, -1, NULL))
    success = FALSE;

  if (success)
    {
      const guint8 *data;
      gchar        *canonical;

      canonical = gimp_canonicalize_identifier (identifier);

      data = plug_in_data_get (gimp, canonical, &bytes);
      success = (data != NULL);

      g_free (canonical);

      if (success)
        data_copy = g_memdup (data, bytes);
    }

  return_args = procedural_db_return_args (&procedural_db_get_data_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = bytes;
      return_args[2].value.pdb_pointer = data_copy;
    }

  return return_args;
}

static ProcArg procedural_db_get_data_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "identifier",
    "The identifier associated with data"
  }
};

static ProcArg procedural_db_get_data_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "bytes",
    "The number of bytes in the data"
  },
  {
    GIMP_PDB_INT8ARRAY,
    "data",
    "A byte array containing data"
  }
};

static ProcRecord procedural_db_get_data_proc =
{
  "gimp-procedural-db-get-data",
  "gimp-procedural-db-get-data",
  "Returns data associated with the specified identifier.",
  "This procedure returns any data which may have been associated with the specified identifier. The data is a variable length array of bytes. If no data has been associated with the identifier, an error is returned.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  NULL,
  GIMP_INTERNAL,
  1,
  procedural_db_get_data_inargs,
  2,
  procedural_db_get_data_outargs,
  { { procedural_db_get_data_invoker } }
};

static Argument *
procedural_db_get_data_size_invoker (Gimp         *gimp,
                                     GimpContext  *context,
                                     GimpProgress *progress,
                                     Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *identifier;
  gint32 bytes;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL || !g_utf8_validate (identifier, -1, NULL))
    success = FALSE;

  if (success)
    {
      const guint8 *data;
      gchar        *canonical;

      canonical = gimp_canonicalize_identifier (identifier);

      data = plug_in_data_get (gimp, canonical, &bytes);
      success = (data != NULL);

      g_free (canonical);
    }

  return_args = procedural_db_return_args (&procedural_db_get_data_size_proc, success);

  if (success)
    return_args[1].value.pdb_int = bytes;

  return return_args;
}

static ProcArg procedural_db_get_data_size_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "identifier",
    "The identifier associated with data"
  }
};

static ProcArg procedural_db_get_data_size_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "bytes",
    "The number of bytes in the data"
  }
};

static ProcRecord procedural_db_get_data_size_proc =
{
  "gimp-procedural-db-get-data-size",
  "gimp-procedural-db-get-data-size",
  "Returns size of data associated with the specified identifier.",
  "This procedure returns the size of any data which may have been associated with the specified identifier. If no data has been associated with the identifier, an error is returned.",
  "Nick Lamb",
  "Nick Lamb",
  "1998",
  NULL,
  GIMP_INTERNAL,
  1,
  procedural_db_get_data_size_inargs,
  1,
  procedural_db_get_data_size_outargs,
  { { procedural_db_get_data_size_invoker } }
};

static Argument *
procedural_db_set_data_invoker (Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  gchar *identifier;
  gint32 bytes;
  guint8 *data;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL || !g_utf8_validate (identifier, -1, NULL))
    success = FALSE;

  bytes = args[1].value.pdb_int;
  if (bytes <= 0)
    success = FALSE;

  data = (guint8 *) args[2].value.pdb_pointer;

  if (success)
    {
      gchar *canonical;

      canonical = gimp_canonicalize_identifier (identifier);

      plug_in_data_set (gimp, canonical, bytes, data);

      g_free (canonical);
    }

  return procedural_db_return_args (&procedural_db_set_data_proc, success);
}

static ProcArg procedural_db_set_data_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "identifier",
    "The identifier associated with data"
  },
  {
    GIMP_PDB_INT32,
    "bytes",
    "The number of bytes in the data"
  },
  {
    GIMP_PDB_INT8ARRAY,
    "data",
    "A byte array containing data"
  }
};

static ProcRecord procedural_db_set_data_proc =
{
  "gimp-procedural-db-set-data",
  "gimp-procedural-db-set-data",
  "Associates the specified identifier with the supplied data.",
  "This procedure associates the supplied data with the provided identifier. The data may be subsequently retrieved by a call to 'procedural-db-get-data'.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  NULL,
  GIMP_INTERNAL,
  3,
  procedural_db_set_data_inargs,
  0,
  NULL,
  { { procedural_db_set_data_invoker } }
};
