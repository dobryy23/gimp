/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpprogress.c
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include "gimp.h"


typedef struct _GimpProgressData GimpProgressData;

struct _GimpProgressData
{
  gchar                     *progress_callback;
#if 0
  guint                      idle_id;
#endif
  GimpProgressStartCallback  start_callback;
  GimpProgressEndCallback    end_callback;
  GimpProgressTextCallback   text_callback;
  GimpProgressValueCallback  value_callback;
  gpointer                   data;
};


/*  local function prototypes  */

static void      gimp_temp_progress_run      (const gchar       *name,
                                              gint               nparams,
                                              const GimpParam   *param,
                                              gint              *nreturn_vals,
                                              GimpParam        **return_vals);
#if 0
static gboolean  gimp_temp_progress_run_idle (GimpProgressData  *progress_data);
#endif


/*  private variables  */

static GHashTable *gimp_progress_ht = NULL;


/*  public functions  */

const gchar *
gimp_progress_install (GimpProgressStartCallback start_callback,
                       GimpProgressEndCallback   end_callback,
                       GimpProgressTextCallback  text_callback,
                       GimpProgressValueCallback value_callback,
                       gpointer                  user_data)
{
  static const GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,  "command", "" },
    { GIMP_PDB_STRING, "text",    "" },
    { GIMP_PDB_FLOAT,  "value",   "" }
  };

  gchar *progress_callback;

  g_return_val_if_fail (start_callback != NULL, NULL);
  g_return_val_if_fail (end_callback != NULL, NULL);
  g_return_val_if_fail (text_callback != NULL, NULL);
  g_return_val_if_fail (value_callback != NULL, NULL);

  progress_callback = gimp_procedural_db_temp_name ();

  gimp_install_temp_proc (progress_callback,
			  "Temporary progress callback procedure",
			  "",
			  "Michael Natterer  <mitch@gimp.org>",
			  "Michael Natterer",
			  "2004",
			  NULL,
			  "RGB*, GRAY*, INDEXED*",
			  GIMP_TEMPORARY,
			  G_N_ELEMENTS (args), 0,
			  args, NULL,
			  gimp_temp_progress_run);

  if (_gimp_progress_install (progress_callback))
    {
      GimpProgressData *progress_data;

      gimp_extension_enable (); /* Allow callbacks to be watched */

      /* Now add to hash table so we can find it again */
      if (! gimp_progress_ht)
        gimp_progress_ht = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                  g_free, g_free);

      progress_data = g_new0 (GimpProgressData, 1);

      progress_data->progress_callback = progress_callback;
      progress_data->start_callback    = start_callback;
      progress_data->end_callback      = end_callback;
      progress_data->text_callback     = text_callback;
      progress_data->value_callback    = value_callback;
      progress_data->data              = user_data;

      g_hash_table_insert (gimp_progress_ht, progress_callback, progress_data);

      return progress_callback;
    }

  gimp_uninstall_temp_proc (progress_callback);
  g_free (progress_callback);

  return NULL;
}

void
gimp_progress_uninstall (const gchar *progress_callback)
{
  GimpProgressData *progress_data;

  g_return_if_fail (progress_callback != NULL);
  g_return_if_fail (gimp_progress_ht != NULL);

  progress_data = g_hash_table_lookup (gimp_progress_ht, progress_callback);

  if (! progress_data)
    {
      g_warning ("Can't find internal progress data");
      return;
    }

#if 0
  if (progress_data->idle_id)
    g_source_remove (progress_data->idle_id);
#endif

  _gimp_progress_uninstall (progress_callback);
  gimp_uninstall_temp_proc (progress_callback);

  g_hash_table_remove (gimp_progress_ht, progress_callback);
}


/*  private functions  */

static void
gimp_temp_progress_run (const gchar      *name,
                        gint              nparams,
                        const GimpParam  *param,
                        gint             *nreturn_vals,
                        GimpParam       **return_vals)
{
  static GimpParam  values[1];
  GimpProgressData *progress_data;

  progress_data = g_hash_table_lookup (gimp_progress_ht, name);

  if (! progress_data)
    {
      g_warning ("Can't find internal progress data");
    }
  else
    {
#if 0
      if (! progress_data->idle_id)
        progress_data->idle_id =
          g_idle_add ((GSourceFunc) gimp_temp_progress_run_idle,
                      progress_data);
#endif

      g_print ("%s: command = %d, text = %s, value = %f\n",
               G_STRFUNC,
               param[0].data.d_int32,
               param[1].data.d_string,
               param[2].data.d_float);

      switch (param[0].data.d_int32)
        {
        case 0:
          progress_data->start_callback (param[1].data.d_string,
                                         param[2].data.d_float != 0.0,
                                         progress_data->data);
          break;

        case 1:
          progress_data->end_callback (progress_data->data);
          break;

        case 2:
          progress_data->text_callback (param[1].data.d_string,
                                        progress_data->data);
          break;

        case 3:
          progress_data->value_callback (param[2].data.d_float,
                                         progress_data->data);
          break;

        default:
          g_warning ("Unknown command passed to progress callback");
          break;
        }
    }

  g_print ("%s: callback finished\n", G_STRFUNC);

  while (g_main_context_pending (NULL))
    g_main_context_iteration (NULL, TRUE);

  *nreturn_vals = 1;
  *return_vals  = values;

  values[0].type          = GIMP_PDB_STATUS;
  values[0].data.d_status = GIMP_PDB_SUCCESS;
}

#if 0
static gboolean
gimp_temp_progress_run_idle (GimpProgressData *progress_data)
{
  progress_data->idle_id = 0;

  if (progress_data->callback)
    progress_data->callback (progress_data->progress_name,
                             progress_data->closing,
                             progress_data->data);

  if (progress_data->closing)
    {
      gchar *progress_callback = progress_data->progress_callback;

      progress_data->progress_callback = NULL;
      gimp_progress_select_destroy (progress_callback);
    }

  return FALSE;
}
#endif

