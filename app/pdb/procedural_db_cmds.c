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
#include "gimppdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "core/gimp.h"
#include "core/gimpparamspecs-desc.h"
#include "gimp-pdb-compat.h"
#include "gimppdb-query.h"
#include "plug-in/gimppluginmanager-data.h"


static GValueArray *
procedural_db_temp_name_invoker (GimpProcedure     *procedure,
                                 Gimp              *gimp,
                                 GimpContext       *context,
                                 GimpProgress      *progress,
                                 const GValueArray *args)
{
  GValueArray *return_vals;
  gchar *temp_name = NULL;

  static gint proc_number = 0;

  temp_name = g_strdup_printf ("temp-procedure-number-%d", proc_number++);

  return_vals = gimp_procedure_get_return_values (procedure, TRUE);
  g_value_take_string (&return_vals->values[1], temp_name);

  return return_vals;
}

static GValueArray *
procedural_db_dump_invoker (GimpProcedure     *procedure,
                            Gimp              *gimp,
                            GimpContext       *context,
                            GimpProgress      *progress,
                            const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *filename;

  filename = g_value_get_string (&args->values[0]);

  if (success)
    {
      success = gimp_pdb_dump (gimp->pdb, filename);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GValueArray *
procedural_db_query_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *name;
  const gchar *blurb;
  const gchar *help;
  const gchar *author;
  const gchar *copyright;
  const gchar *date;
  const gchar *proc_type;
  gint32 num_matches = 0;
  gchar **procedure_names = NULL;

  name = g_value_get_string (&args->values[0]);
  blurb = g_value_get_string (&args->values[1]);
  help = g_value_get_string (&args->values[2]);
  author = g_value_get_string (&args->values[3]);
  copyright = g_value_get_string (&args->values[4]);
  date = g_value_get_string (&args->values[5]);
  proc_type = g_value_get_string (&args->values[6]);

  if (success)
    {
      success = gimp_pdb_query (gimp->pdb,
                                name, blurb, help, author,
                                copyright, date, proc_type,
                                &num_matches, &procedure_names);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], num_matches);
      gimp_value_take_stringarray (&return_vals->values[2], procedure_names, num_matches);
    }

  return return_vals;
}

static GValueArray *
procedural_db_proc_info_invoker (GimpProcedure     *procedure,
                                 Gimp              *gimp,
                                 GimpContext       *context,
                                 GimpProgress      *progress,
                                 const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *procedure_name;
  gchar *blurb = NULL;
  gchar *help = NULL;
  gchar *author = NULL;
  gchar *copyright = NULL;
  gchar *date = NULL;
  gint32 proc_type = 0;
  gint32 num_args = 0;
  gint32 num_values = 0;

  procedure_name = g_value_get_string (&args->values[0]);

  if (success)
    {
      GimpPDBProcType  ptype;
      gchar           *canonical;

      canonical = gimp_canonicalize_identifier (procedure_name);

      success = gimp_pdb_proc_info (gimp->pdb, canonical,
                                    &blurb, &help, &author,
                                    &copyright, &date, &ptype,
                                    &num_args, &num_values);
      proc_type = ptype;

      g_free (canonical);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals->values[1], blurb);
      g_value_take_string (&return_vals->values[2], help);
      g_value_take_string (&return_vals->values[3], author);
      g_value_take_string (&return_vals->values[4], copyright);
      g_value_take_string (&return_vals->values[5], date);
      g_value_set_enum (&return_vals->values[6], proc_type);
      g_value_set_int (&return_vals->values[7], num_args);
      g_value_set_int (&return_vals->values[8], num_values);
    }

  return return_vals;
}

static GValueArray *
procedural_db_proc_arg_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *procedure_name;
  gint32 arg_num;
  gint32 arg_type = 0;
  gchar *arg_name = NULL;
  gchar *arg_desc = NULL;

  procedure_name = g_value_get_string (&args->values[0]);
  arg_num = g_value_get_int (&args->values[1]);

  if (success)
    {
      GimpProcedure *proc;
      gchar         *canonical;

      canonical = gimp_canonicalize_identifier (procedure_name);

      proc = gimp_pdb_lookup_procedure (gimp->pdb, canonical);

      if (! proc)
        {
          const gchar *compat_name;

          compat_name = gimp_pdb_lookup_compat_proc_name (gimp->pdb, canonical);

          if (compat_name)
            proc = gimp_pdb_lookup_procedure (gimp->pdb, compat_name);
        }

      g_free (canonical);

      if (proc && (arg_num >= 0 && arg_num < proc->num_args))
        {
          GParamSpec *pspec = proc->args[arg_num];

          arg_type = gimp_pdb_compat_arg_type_from_gtype (G_PARAM_SPEC_VALUE_TYPE (pspec));
          arg_name = g_strdup (g_param_spec_get_name (pspec));
          arg_desc = gimp_param_spec_get_desc (pspec);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_enum (&return_vals->values[1], arg_type);
      g_value_take_string (&return_vals->values[2], arg_name);
      g_value_take_string (&return_vals->values[3], arg_desc);
    }

  return return_vals;
}

static GValueArray *
procedural_db_proc_val_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *procedure_name;
  gint32 val_num;
  gint32 val_type = 0;
  gchar *val_name = NULL;
  gchar *val_desc = NULL;

  procedure_name = g_value_get_string (&args->values[0]);
  val_num = g_value_get_int (&args->values[1]);

  if (success)
    {
      GimpProcedure *proc;
      gchar         *canonical;

      canonical = gimp_canonicalize_identifier (procedure_name);

      proc = gimp_pdb_lookup_procedure (gimp->pdb, canonical);

      if (! proc)
        {
          const gchar *compat_name;

          compat_name = gimp_pdb_lookup_compat_proc_name (gimp->pdb, canonical);

          if (compat_name)
            proc = gimp_pdb_lookup_procedure (gimp->pdb, compat_name);
        }

      g_free (canonical);

      if (proc && (val_num >= 0 && val_num < proc->num_values))
        {
          GParamSpec *pspec = proc->values[val_num];

          val_type = gimp_pdb_compat_arg_type_from_gtype (G_PARAM_SPEC_VALUE_TYPE (pspec));
          val_name = g_strdup (g_param_spec_get_name (pspec));
          val_desc = gimp_param_spec_get_desc (pspec);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_enum (&return_vals->values[1], val_type);
      g_value_take_string (&return_vals->values[2], val_name);
      g_value_take_string (&return_vals->values[3], val_desc);
    }

  return return_vals;
}

static GValueArray *
procedural_db_get_data_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *identifier;
  gint32 bytes = 0;
  guint8 *data = NULL;

  identifier = g_value_get_string (&args->values[0]);

  if (success)
    {
      gchar        *canonical = gimp_canonicalize_identifier (identifier);
      const guint8 *orig_data;

      orig_data = gimp_plug_in_manager_get_data (gimp->plug_in_manager,
                                                 canonical, &bytes);

      g_free (canonical);

      if (orig_data)
        data = g_memdup (orig_data, bytes);
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], bytes);
      gimp_value_take_int8array (&return_vals->values[2], data, bytes);
    }

  return return_vals;
}

static GValueArray *
procedural_db_get_data_size_invoker (GimpProcedure     *procedure,
                                     Gimp              *gimp,
                                     GimpContext       *context,
                                     GimpProgress      *progress,
                                     const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *identifier;
  gint32 bytes = 0;

  identifier = g_value_get_string (&args->values[0]);

  if (success)
    {
      gchar *canonical = gimp_canonicalize_identifier (identifier);

      if (! gimp_plug_in_manager_get_data (gimp->plug_in_manager,
                                           canonical, &bytes))
        success = FALSE;

      g_free (canonical);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_int (&return_vals->values[1], bytes);

  return return_vals;
}

static GValueArray *
procedural_db_set_data_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *identifier;
  gint32 bytes;
  const guint8 *data;

  identifier = g_value_get_string (&args->values[0]);
  bytes = g_value_get_int (&args->values[1]);
  data = gimp_value_get_int8array (&args->values[2]);

  if (success)
    {
      gchar *canonical = gimp_canonicalize_identifier (identifier);

      gimp_plug_in_manager_set_data (gimp->plug_in_manager,
                                     canonical, bytes, data);

      g_free (canonical);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

void
register_procedural_db_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-procedural-db-temp-name
   */
  procedure = gimp_procedure_new (procedural_db_temp_name_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-temp-name");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-temp-name",
                                     "Generates a unique temporary PDB name.",
                                     "This procedure generates a temporary PDB entry name that is guaranteed to be unique. It is mainly used by the interactive popup dialogs to generate a PDB entry name.",
                                     "Andy Thomas",
                                     "Andy Thomas",
                                     "1998",
                                     NULL);
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("temp-name",
                                                           "temp name",
                                                           "A unique temporary name for a temporary PDB entry",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-dump
   */
  procedure = gimp_procedure_new (procedural_db_dump_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-dump");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-dump",
                                     "Dumps the current contents of the procedural database",
                                     "This procedure dumps the contents of the procedural database to the specified file. The file will contain all of the information provided for each registered procedure. This file is in a format appropriate for use with the supplied \"pdb_self_doc.el\" Elisp script, which generates a texinfo document.",
                                     "Spencer Kimball & Josh MacDonald",
                                     "Spencer Kimball & Josh MacDonald & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("filename",
                                                       "filename",
                                                       "The dump filename",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-query
   */
  procedure = gimp_procedure_new (procedural_db_query_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-query");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-query",
                                     "Queries the procedural database for its contents using regular expression matching.",
                                     "This procedure queries the contents of the procedural database. It is supplied with seven arguments matching procedures on { name, blurb, help, author, copyright, date, procedure type}. This is accomplished using regular expression matching. For instance, to find all procedures with \"jpeg\" listed in the blurb, all seven arguments can be supplied as \".*\", except for the second, which can be supplied as \".*jpeg.*\". There are two return arguments for this procedure. The first is the number of procedures matching the query. The second is a concatenated list of procedure names corresponding to those matching the query. If no matching entries are found, then the returned string is NULL and the number of entries is 0.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1995-1996",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The regex for procedure name",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("blurb",
                                                       "blurb",
                                                       "The regex for procedure blurb",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("help",
                                                       "help",
                                                       "The regex for procedure help",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("author",
                                                       "author",
                                                       "The regex for procedure author",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("copyright",
                                                       "copyright",
                                                       "The regex for procedure copyright",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("date",
                                                       "date",
                                                       "The regex for procedure date",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("proc-type",
                                                       "proc type",
                                                       "The regex for procedure type: { 'Internal GIMP procedure', 'GIMP Plug-In', 'GIMP Extension', 'Temporary Procedure' }",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-matches",
                                                          "num matches",
                                                          "The number of matching procedures",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("procedure-names",
                                                                 "procedure names",
                                                                 "The list of procedure names",
                                                                 GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-proc-info
   */
  procedure = gimp_procedure_new (procedural_db_proc_info_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-proc-info");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-proc-info",
                                     "Queries the procedural database for information on the specified procedure.",
                                     "This procedure returns information on the specified procedure. A short blurb, detailed help, author(s), copyright information, procedure type, number of input, and number of return values are returned. For specific information on each input argument and return value, use the 'gimp_procedural_db_proc_arg' and 'gimp_procedural_db_proc_val' procedures.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1997",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure name",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("blurb",
                                                           "blurb",
                                                           "A short blurb",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("help",
                                                           "help",
                                                           "Detailed procedure help",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("author",
                                                           "author",
                                                           "Author(s) of the procedure",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("copyright",
                                                           "copyright",
                                                           "The copyright",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("date",
                                                           "date",
                                                           "Copyright date",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_enum ("proc-type",
                                                      "proc type",
                                                      "The procedure type",
                                                      GIMP_TYPE_PDB_PROC_TYPE,
                                                      GIMP_INTERNAL,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-args",
                                                          "num args",
                                                          "The number of input arguments",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-values",
                                                          "num values",
                                                          "The number of return values",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-proc-arg
   */
  procedure = gimp_procedure_new (procedural_db_proc_arg_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-proc-arg");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-proc-arg",
                                     "Queries the procedural database for information on the specified procedure's argument.",
                                     "This procedure returns information on the specified procedure's argument. The argument type, name, and a description are retrieved.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1997",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure name",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("arg-num",
                                                      "arg num",
                                                      "The argument number",
                                                      G_MININT32, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_enum ("arg-type",
                                                         "arg type",
                                                         "The type of argument",
                                                         GIMP_TYPE_PDB_ARG_TYPE,
                                                         GIMP_PDB_INT32,
                                                         GIMP_PARAM_READWRITE));
  gimp_param_spec_enum_exclude_value (GIMP_PARAM_SPEC_ENUM (procedure->values[0]),
                                      GIMP_PDB_END);
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("arg-name",
                                                           "arg name",
                                                           "The name of the argument",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("arg-desc",
                                                           "arg desc",
                                                           "A description of the argument",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-proc-val
   */
  procedure = gimp_procedure_new (procedural_db_proc_val_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-proc-val");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-proc-val",
                                     "Queries the procedural database for information on the specified procedure's return value.",
                                     "This procedure returns information on the specified procedure's return value. The return value type, name, and a description are retrieved.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1997",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("procedure-name",
                                                       "procedure name",
                                                       "The procedure name",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("val-num",
                                                      "val num",
                                                      "The return value number",
                                                      G_MININT32, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_enum ("val-type",
                                                         "val type",
                                                         "The type of return value",
                                                         GIMP_TYPE_PDB_ARG_TYPE,
                                                         GIMP_PDB_INT32,
                                                         GIMP_PARAM_READWRITE));
  gimp_param_spec_enum_exclude_value (GIMP_PARAM_SPEC_ENUM (procedure->values[0]),
                                      GIMP_PDB_END);
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("val-name",
                                                           "val name",
                                                           "The name of the return value",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("val-desc",
                                                           "val desc",
                                                           "A description of the return value",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-get-data
   */
  procedure = gimp_procedure_new (procedural_db_get_data_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-get-data");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-get-data",
                                     "Returns data associated with the specified identifier.",
                                     "This procedure returns any data which may have been associated with the specified identifier. The data is a variable length array of bytes. If no data has been associated with the identifier, an error is returned.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1997",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("identifier",
                                                       "identifier",
                                                       "The identifier associated with data",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("bytes",
                                                          "bytes",
                                                          "The number of bytes in the data",
                                                          1, G_MAXINT32, 1,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int8_array ("data",
                                                               "data",
                                                               "A byte array containing data",
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-get-data-size
   */
  procedure = gimp_procedure_new (procedural_db_get_data_size_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-get-data-size");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-get-data-size",
                                     "Returns size of data associated with the specified identifier.",
                                     "This procedure returns the size of any data which may have been associated with the specified identifier. If no data has been associated with the identifier, an error is returned.",
                                     "Nick Lamb",
                                     "Nick Lamb",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("identifier",
                                                       "identifier",
                                                       "The identifier associated with data",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("bytes",
                                                          "bytes",
                                                          "The number of bytes in the data",
                                                          1, G_MAXINT32, 1,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-procedural-db-set-data
   */
  procedure = gimp_procedure_new (procedural_db_set_data_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure), "gimp-procedural-db-set-data");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-procedural-db-set-data",
                                     "Associates the specified identifier with the supplied data.",
                                     "This procedure associates the supplied data with the provided identifier. The data may be subsequently retrieved by a call to 'procedural-db-get-data'.",
                                     "Spencer Kimball & Peter Mattis",
                                     "Spencer Kimball & Peter Mattis",
                                     "1997",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("identifier",
                                                       "identifier",
                                                       "The identifier associated with data",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("bytes",
                                                      "bytes",
                                                      "The number of bytes in the data",
                                                      1, G_MAXINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int8_array ("data",
                                                           "data",
                                                           "A byte array containing data",
                                                           GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
