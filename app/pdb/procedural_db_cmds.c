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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib-object.h>

#include "libgimpbase/gimpbasetypes.h"

#include "pdb-types.h"
#include "procedural_db.h"

#include "core/gimp.h"

#ifdef HAVE_GLIBC_REGEX
#include <regex.h>
#else
#include "regexrepl/regex.h"
#endif


#include "libgimp/gimpintl.h"
#include "libgimpbase/gimpbasetypes.h"

/*  Query structure  */
typedef struct _PDBQuery PDBQuery;

struct _PDBQuery
{
  regex_t name_regex;
  regex_t blurb_regex;
  regex_t help_regex;
  regex_t author_regex;
  regex_t copyright_regex;
  regex_t date_regex;
  regex_t proc_type_regex;

  gchar **list_of_procs;
  int     num_procs;
};

typedef struct _PDBData PDBData;

struct _PDBData
{
  gchar *identifier;
  gint   bytes;
  gchar *data;
};

static FILE  *procedural_db_out = NULL;
static GList *data_list         = NULL;

static gchar *proc_type_str[] = 
{
  N_("Internal GIMP procedure"),
  N_("GIMP Plug-In"),
  N_("GIMP Extension"),
  N_("Temporary Procedure")
};

static const gchar * const type_str[] =
{
  "GIMP_PDB_INT32",
  "GIMP_PDB_INT16",
  "GIMP_PDB_INT8",
  "GIMP_PDB_FLOAT",
  "GIMP_PDB_STRING",
  "GIMP_PDB_INT32ARRAY",
  "GIMP_PDB_INT16ARRAY",
  "GIMP_PDB_INT8ARRAY",
  "GIMP_PDB_FLOATARRAY",
  "GIMP_PDB_STRINGARRAY",
  "GIMP_PDB_COLOR",
  "GIMP_PDB_REGION",
  "GIMP_PDB_DISPLAY",
  "GIMP_PDB_IMAGE",
  "GIMP_PDB_LAYER",
  "GIMP_PDB_CHANNEL",
  "GIMP_PDB_DRAWABLE",
  "GIMP_PDB_SELECTION",
  "GIMP_PDB_BOUNDARY",
  "GIMP_PDB_PATH",
  "GIMP_PDB_PARASITE",
  "GIMP_PDB_STATUS",
  "GIMP_PDB_END"
};

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
  procedural_db_register (gimp, &procedural_db_dump_proc);
  procedural_db_register (gimp, &procedural_db_query_proc);
  procedural_db_register (gimp, &procedural_db_proc_info_proc);
  procedural_db_register (gimp, &procedural_db_proc_arg_proc);
  procedural_db_register (gimp, &procedural_db_proc_val_proc);
  procedural_db_register (gimp, &procedural_db_get_data_proc);
  procedural_db_register (gimp, &procedural_db_get_data_size_proc);
  procedural_db_register (gimp, &procedural_db_set_data_proc);
}

static int
match_strings (regex_t *preg,
	       gchar   *a)
{
  if (!a)
    return 0;

  return regexec (preg, a, 0, NULL, 0);
}

static void
procedural_db_query_entry (gpointer key,
                           gpointer value,
                           gpointer user_data)
{
  GList *list;
  ProcRecord *proc;
  PDBQuery *pdb_query;
  int new_length;

  list = (GList *) value;
  proc = (ProcRecord *) list->data;
  pdb_query = (PDBQuery *) user_data;

  if (!match_strings (&pdb_query->name_regex, proc->name) &&
      !match_strings (&pdb_query->blurb_regex, proc->blurb) &&
      !match_strings (&pdb_query->help_regex, proc->help) &&
      !match_strings (&pdb_query->author_regex, proc->author) &&
      !match_strings (&pdb_query->copyright_regex, proc->copyright) &&
      !match_strings (&pdb_query->date_regex, proc->date) &&
      !match_strings (&pdb_query->proc_type_regex,
		      proc_type_str[(int) proc->proc_type]))
    {
      new_length = proc->name ? (strlen (proc->name) + 1) : 0;

      if (new_length)
        {
          pdb_query->num_procs++;
          pdb_query->list_of_procs = g_realloc (pdb_query->list_of_procs,
                                               (sizeof (gchar **) * pdb_query->num_procs));
          pdb_query->list_of_procs[pdb_query->num_procs - 1] = g_strdup (proc->name);
        }
    }
}

static void
output_string (const char *string)
{
  fprintf (procedural_db_out, "\"");
  while (*string)
    {
      switch (*string)
	{
	case '\\' : fprintf (procedural_db_out, "\\\\"); break;
	case '\"' : fprintf (procedural_db_out, "\\\""); break;
	case '{'  : fprintf (procedural_db_out, "@{"); break;
	case '@'  : fprintf (procedural_db_out, "@@"); break;
	case '}'  : fprintf (procedural_db_out, "@}"); break;
	default:
	  fprintf (procedural_db_out, "%c", *string);
	}
      string++;
    }
  fprintf (procedural_db_out, "\"\n");
}

static void
procedural_db_print_entry (gpointer key,
                           gpointer value,
                           gpointer user_data)
{
  int i;
  ProcRecord *procedure;
  GList *list = (GList *) value;
  int num = 0;
  GString *buf = g_string_new ("");

  while (list)
    {
      num++;
      procedure = (ProcRecord*) list->data;
      list = list->next;

      fprintf (procedural_db_out, "\n(register-procedure ");

      if (list || num != 1)
        {
          g_string_printf (buf, "%s <%d>", procedure->name, num);
          output_string (buf->str);
        }
      else
        output_string (procedure->name);

      output_string (procedure->blurb);
      output_string (procedure->help);
      output_string (procedure->author);
      output_string (procedure->copyright);
      output_string (procedure->date);
      output_string (proc_type_str[(int) procedure->proc_type]);

      fprintf (procedural_db_out, "( ");
      for (i = 0; i < procedure->num_args; i++)
        {
          fprintf (procedural_db_out, "( ");

          output_string (procedure->args[i].name );
          output_string (type_str[procedure->args[i].arg_type]);
          output_string (procedure->args[i].description);

          fprintf (procedural_db_out, " ) ");
        }
      fprintf (procedural_db_out, " ) ");

      fprintf (procedural_db_out, "( ");
      for (i = 0; i < procedure->num_values; i++)
        {
          fprintf (procedural_db_out, "( ");
          output_string (procedure->values[i].name );
          output_string (type_str[procedure->values[i].arg_type]);
          output_string (procedure->values[i].description);

          fprintf (procedural_db_out, " ) ");
        }
      fprintf (procedural_db_out, " ) ");
      fprintf (procedural_db_out, " ) ");
    }

  g_string_free (buf, TRUE);
}

/* This really doesn't belong here, but it depends on our generated type_str
 * array.
 */
const char *
pdb_type_name (gint type)
{
  if (type >= 0 && type <= GIMP_PDB_END)
    return type_str[type];
  else
    return g_strdup_printf ("(PDB type %d unknown)", type);
   /* Yeah, we leak the memory.  But then you shouldn't try and
    * get the name of a PDB type that doesn't exist, should you.
    */
}

static Argument *
procedural_db_dump_invoker (Gimp     *gimp,
                            Argument *args)
{
  gboolean success = TRUE;
  gchar *filename;

  filename = (gchar *) args[0].value.pdb_pointer;
  if (filename == NULL)
    success = FALSE;

  if (success)
    {
      if ((procedural_db_out = fopen (filename, "w")))
	{
	  g_hash_table_foreach (gimp->procedural_ht,
				procedural_db_print_entry, NULL);
	  fclose (procedural_db_out);
	}
      else
	success = FALSE;
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
  "gimp_procedural_db_dump",
  "Dumps the current contents of the procedural database",
  "This procedure dumps the contents of the procedural database to the specified file. The file will contain all of the information provided for each registered procedure. This file is in a format appropriate for use with the supplied \"pdb_self_doc.el\" Elisp script, which generates a texinfo document.",
  "Spencer Kimball & Josh MacDonald",
  "Spencer Kimball & Josh MacDonald & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  1,
  procedural_db_dump_inargs,
  0,
  NULL,
  { { procedural_db_dump_invoker } }
};

static Argument *
procedural_db_query_invoker (Gimp     *gimp,
                             Argument *args)
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
  PDBQuery pdb_query;

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
      regcomp (&pdb_query.name_regex, name, 0);
      regcomp (&pdb_query.blurb_regex, blurb, 0);
      regcomp (&pdb_query.help_regex, help, 0);
      regcomp (&pdb_query.author_regex, author, 0);
      regcomp (&pdb_query.copyright_regex, copyright, 0);
      regcomp (&pdb_query.date_regex, date, 0);
      regcomp (&pdb_query.proc_type_regex, proc_type, 0);
    
      pdb_query.list_of_procs = NULL;
      pdb_query.num_procs = 0;
    
      g_hash_table_foreach (gimp->procedural_ht,
			    procedural_db_query_entry, &pdb_query);
    
      free (pdb_query.name_regex.buffer);
      free (pdb_query.blurb_regex.buffer);
      free (pdb_query.help_regex.buffer);
      free (pdb_query.author_regex.buffer);
      free (pdb_query.copyright_regex.buffer);
      free (pdb_query.date_regex.buffer);
      free (pdb_query.proc_type_regex.buffer);
    }

  return_args = procedural_db_return_args (&procedural_db_query_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = pdb_query.num_procs;
      return_args[2].value.pdb_pointer = pdb_query.list_of_procs;
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
    "proc_type",
    "The regex for procedure type: { 'Internal GIMP procedure', 'GIMP Plug-in', 'GIMP Extension' }"
  }
};

static ProcArg procedural_db_query_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num_matches",
    "The number of matching procedures"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "procedure_names",
    "The list of procedure names"
  }
};

static ProcRecord procedural_db_query_proc =
{
  "gimp_procedural_db_query",
  "Queries the procedural database for its contents using regular expression matching.",
  "This procedure queries the contents of the procedural database. It is supplied with seven arguments matching procedures on { name, blurb, help, author, copyright, date, procedure type}. This is accomplished using regular expression matching. For instance, to find all procedures with \"jpeg\" listed in the blurb, all seven arguments can be supplied as \".*\", except for the second, which can be supplied as \".*jpeg.*\". There are two return arguments for this procedure. The first is the number of procedures matching the query. The second is a concatenated list of procedure names corresponding to those matching the query. If no matching entries are found, then the returned string is NULL and the number of entries is 0.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  GIMP_INTERNAL,
  7,
  procedural_db_query_inargs,
  2,
  procedural_db_query_outargs,
  { { procedural_db_query_invoker } }
};

static Argument *
procedural_db_proc_info_invoker (Gimp     *gimp,
                                 Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  ProcRecord *proc = NULL;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL)
    success = FALSE;

  if (success)
    success = (proc = procedural_db_lookup (gimp, proc_name)) != NULL;

  return_args = procedural_db_return_args (&procedural_db_proc_info_proc, success);

  if (success)
    {
      return_args[1].value.pdb_pointer = g_strdup (proc->blurb);
      return_args[2].value.pdb_pointer = g_strdup (proc->help);
      return_args[3].value.pdb_pointer = g_strdup (proc->author);
      return_args[4].value.pdb_pointer = g_strdup (proc->copyright);
      return_args[5].value.pdb_pointer = g_strdup (proc->date);
      return_args[6].value.pdb_int = proc->proc_type;
      return_args[7].value.pdb_int = proc->num_args;
      return_args[8].value.pdb_int = proc->num_values;
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
    "proc_type",
    "The procedure type: { GIMP_INTERNAL (0), GIMP_PLUGIN (1), GIMP_EXTENSION (2), GIMP_TEMPORARY (3) }"
  },
  {
    GIMP_PDB_INT32,
    "num_args",
    "The number of input arguments"
  },
  {
    GIMP_PDB_INT32,
    "num_values",
    "The number of return values"
  }
};

static ProcRecord procedural_db_proc_info_proc =
{
  "gimp_procedural_db_proc_info",
  "Queries the procedural database for information on the specified procedure.",
  "This procedure returns information on the specified procedure. A short blurb, detailed help, author(s), copyright information, procedure type, number of input, and number of return values are returned. For specific information on each input argument and return value, use the 'gimp_procedural_db_proc_arg' and 'gimp_procedural_db_proc_val' procedures.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  GIMP_INTERNAL,
  1,
  procedural_db_proc_info_inargs,
  8,
  procedural_db_proc_info_outargs,
  { { procedural_db_proc_info_invoker } }
};

static Argument *
procedural_db_proc_arg_invoker (Gimp     *gimp,
                                Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  gint32 arg_num;
  ProcRecord *proc;
  ProcArg *arg = NULL;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL)
    success = FALSE;

  arg_num = args[1].value.pdb_int;

  if (success)
    {
      proc = procedural_db_lookup (gimp, proc_name);
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
    "arg_num",
    "The argument number"
  }
};

static ProcArg procedural_db_proc_arg_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "arg_type",
    "The type of argument { GIMP_PDB_INT32 (0), GIMP_PDB_INT16 (1), GIMP_PDB_INT8 (2), GIMP_PDB_FLOAT (3), GIMP_PDB_STRING (4), GIMP_PDB_INT32ARRAY (5), GIMP_PDB_INT16ARRAY (6), GIMP_PDB_INT8ARRAY (7), GIMP_PDB_FLOATARRAY (8), GIMP_PDB_STRINGARRAY (9), GIMP_PDB_COLOR (10), GIMP_PDB_REGION (11), GIMP_PDB_DISPLAY (12), GIMP_PDB_IMAGE (13), GIMP_PDB_LAYER (14), GIMP_PDB_CHANNEL (15), GIMP_PDB_DRAWABLE (16), GIMP_PDB_SELECTION (17), GIMP_PDB_BOUNDARY (18), GIMP_PDB_PATH (19), GIMP_PDB_PARASITE (20), GIMP_PDB_STATUS (21) }"
  },
  {
    GIMP_PDB_STRING,
    "arg_name",
    "The name of the argument"
  },
  {
    GIMP_PDB_STRING,
    "arg_desc",
    "A description of the argument"
  }
};

static ProcRecord procedural_db_proc_arg_proc =
{
  "gimp_procedural_db_proc_arg",
  "Queries the procedural database for information on the specified procedure's argument.",
  "This procedure returns information on the specified procedure's argument. The argument type, name, and a description are retrieved.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  GIMP_INTERNAL,
  2,
  procedural_db_proc_arg_inargs,
  3,
  procedural_db_proc_arg_outargs,
  { { procedural_db_proc_arg_invoker } }
};

static Argument *
procedural_db_proc_val_invoker (Gimp     *gimp,
                                Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *proc_name;
  gint32 val_num;
  ProcRecord *proc;
  ProcArg *val = NULL;

  proc_name = (gchar *) args[0].value.pdb_pointer;
  if (proc_name == NULL)
    success = FALSE;

  val_num = args[1].value.pdb_int;

  if (success)
    {
      proc = procedural_db_lookup (gimp, proc_name);
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
    "val_num",
    "The return value number"
  }
};

static ProcArg procedural_db_proc_val_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "val_type",
    "The type of return value { GIMP_PDB_INT32 (0), GIMP_PDB_INT16 (1), GIMP_PDB_INT8 (2), GIMP_PDB_FLOAT (3), GIMP_PDB_STRING (4), GIMP_PDB_INT32ARRAY (5), GIMP_PDB_INT16ARRAY (6), GIMP_PDB_INT8ARRAY (7), GIMP_PDB_FLOATARRAY (8), GIMP_PDB_STRINGARRAY (9), GIMP_PDB_COLOR (10), GIMP_PDB_REGION (11), GIMP_PDB_DISPLAY (12), GIMP_PDB_IMAGE (13), GIMP_PDB_LAYER (14), GIMP_PDB_CHANNEL (15), GIMP_PDB_DRAWABLE (16), GIMP_PDB_SELECTION (17), GIMP_PDB_BOUNDARY (18), GIMP_PDB_PATH (19), GIMP_PDB_PARASITE (20), GIMP_PDB_STATUS (21) }"
  },
  {
    GIMP_PDB_STRING,
    "val_name",
    "The name of the return value"
  },
  {
    GIMP_PDB_STRING,
    "val_desc",
    "A description of the return value"
  }
};

static ProcRecord procedural_db_proc_val_proc =
{
  "gimp_procedural_db_proc_val",
  "Queries the procedural database for information on the specified procedure's return value.",
  "This procedure returns information on the specified procedure's return value. The return value type, name, and a description are retrieved.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  GIMP_INTERNAL,
  2,
  procedural_db_proc_val_inargs,
  3,
  procedural_db_proc_val_outargs,
  { { procedural_db_proc_val_invoker } }
};

static Argument *
procedural_db_get_data_invoker (Gimp     *gimp,
                                Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *identifier;
  guint8 *data_copy = NULL;
  PDBData *data = NULL;
  GList *list;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL)
    success = FALSE;

  if (success)
    {
      success = FALSE;
    
      list = data_list;
      while (list)
	{
	  data = (PDBData *) list->data;
	  list = list->next;
    
	  if (!strcmp (data->identifier, identifier))
	    {
	      data_copy = g_new (guint8, data->bytes);
	      memcpy (data_copy, data->data, data->bytes);
    
	      success = TRUE;
	      break;
	    }
	}
    }

  return_args = procedural_db_return_args (&procedural_db_get_data_proc, success);

  if (success)
    {
      return_args[1].value.pdb_int = data->bytes;
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
  "gimp_procedural_db_get_data",
  "Returns data associated with the specified identifier.",
  "This procedure returns any data which may have been associated with the specified identifier. The data is a variable length array of bytes. If no data has been associated with the identifier, an error is returned.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  GIMP_INTERNAL,
  1,
  procedural_db_get_data_inargs,
  2,
  procedural_db_get_data_outargs,
  { { procedural_db_get_data_invoker } }
};

static Argument *
procedural_db_get_data_size_invoker (Gimp     *gimp,
                                     Argument *args)
{
  gboolean success = TRUE;
  Argument *return_args;
  gchar *identifier;
  PDBData *data = NULL;
  GList *list;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL)
    success = FALSE;

  if (success)
    {
      success = FALSE;
    
      list = data_list;
      while (list)
	{
	  data = (PDBData *) list->data;
	  list = list->next;
    
	  if (!strcmp (data->identifier, identifier))
	    {
	      success = TRUE;
	      break;
	    }
	}
    }

  return_args = procedural_db_return_args (&procedural_db_get_data_size_proc, success);

  if (success)
    return_args[1].value.pdb_int = data->bytes;

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
  "gimp_procedural_db_get_data_size",
  "Returns size of data associated with the specified identifier.",
  "This procedure returns the size of any data which may have been associated with the specified identifier. If no data has been associated with the identifier, an error is returned.",
  "Nick Lamb",
  "Nick Lamb",
  "1998",
  GIMP_INTERNAL,
  1,
  procedural_db_get_data_size_inargs,
  1,
  procedural_db_get_data_size_outargs,
  { { procedural_db_get_data_size_invoker } }
};

static Argument *
procedural_db_set_data_invoker (Gimp     *gimp,
                                Argument *args)
{
  gboolean success = TRUE;
  gchar *identifier;
  gint32 bytes;
  guint8 *data_src;
  PDBData *data = NULL;
  GList *list;

  identifier = (gchar *) args[0].value.pdb_pointer;
  if (identifier == NULL)
    success = FALSE;

  bytes = args[1].value.pdb_int;
  if (bytes <= 0)
    success = FALSE;

  data_src = (guint8 *) args[2].value.pdb_pointer;

  if (success)
    {
      list = data_list;
      while (list)
	{
	  if (!strcmp (((PDBData *) list->data)->identifier, identifier))
	    data = (PDBData *) list->data;
    
	  list = list->next;
	}
    
      /* If there isn't already data with the specified identifier, create one */
      if (data == NULL)
	{
	  data = (PDBData *) g_new (PDBData, 1);
	  data_list = g_list_append (data_list, data);
	}
      else
	g_free (data->data);
    
      data->identifier = g_strdup (identifier);
      data->bytes = bytes;
      data->data = g_new (char, data->bytes);
      memcpy (data->data, (char *) data_src, data->bytes);
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
  "gimp_procedural_db_set_data",
  "Associates the specified identifier with the supplied data.",
  "This procedure associates the supplied data with the provided identifier. The data may be subsequently retrieved by a call to 'procedural-db-get-data'.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1997",
  GIMP_INTERNAL,
  3,
  procedural_db_set_data_inargs,
  0,
  NULL,
  { { procedural_db_set_data_invoker } }
};
