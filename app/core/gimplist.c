/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-1997 Spencer Kimball and Peter Mattis
 *
 * gimplist.c
 * Copyright (C) 2001 Michael Natterer <mitch@gimp.org>
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

#include <string.h> /* strcmp */

#include <gtk/gtk.h>

#include "core-types.h"

#include "gimplist.h"


static void         gimp_list_class_init         (GimpListClass       *klass);
static void         gimp_list_init               (GimpList            *list);

static void         gimp_list_dispose            (GObject             *object);

static void         gimp_list_add                (GimpContainer       *container,
						  GimpObject          *object);
static void         gimp_list_remove             (GimpContainer       *container,
						  GimpObject          *object);
static void         gimp_list_reorder            (GimpContainer       *container,
						  GimpObject          *object,
						  gint                 new_index);
static gboolean     gimp_list_have               (const GimpContainer *container,
						  const GimpObject    *object);
static void         gimp_list_foreach            (const GimpContainer *container,
						  GFunc                func,
						  gpointer             user_data);
static GimpObject * gimp_list_get_child_by_name  (const GimpContainer *container,
						  const gchar         *name);
static GimpObject * gimp_list_get_child_by_index (const GimpContainer *container,
						  gint                 index);
static gint         gimp_list_get_child_index    (const GimpContainer *container,
						  const GimpObject    *object);


static GimpContainerClass *parent_class = NULL;


GType
gimp_list_get_type (void)
{
  static GType list_type = 0;

  if (! list_type)
    {
      static const GTypeInfo list_info =
      {
        sizeof (GimpListClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_list_class_init,
	NULL,		/* class_finalize */
	NULL,		/* class_data     */
	sizeof (GimpList),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_list_init,
      };

      list_type = g_type_register_static (GIMP_TYPE_CONTAINER,
					  "GimpList", 
					  &list_info, 0);
    }

  return list_type;
}

static void
gimp_list_class_init (GimpListClass *klass)
{
  GObjectClass       *object_class;
  GimpContainerClass *container_class;

  object_class    = G_OBJECT_CLASS (klass);
  container_class = GIMP_CONTAINER_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->dispose               = gimp_list_dispose;

  container_class->add                = gimp_list_add;
  container_class->remove             = gimp_list_remove;
  container_class->reorder            = gimp_list_reorder;
  container_class->have               = gimp_list_have;
  container_class->foreach            = gimp_list_foreach;
  container_class->get_child_by_name  = gimp_list_get_child_by_name;
  container_class->get_child_by_index = gimp_list_get_child_by_index;
  container_class->get_child_index    = gimp_list_get_child_index;
}

static void
gimp_list_init (GimpList *list)
{
  list->list = NULL;
}

static void
gimp_list_dispose (GObject *object)
{
  GimpList *list;

  list = GIMP_LIST (object);

  while (list->list)
    {
      gimp_container_remove (GIMP_CONTAINER (list),
			     GIMP_OBJECT (list->list->data));
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_list_add (GimpContainer *container,
	       GimpObject    *object)
{
  GimpList *list;

  list = GIMP_LIST (container);

  list->list = g_list_prepend (list->list, object);
}

static void
gimp_list_remove (GimpContainer *container,
		  GimpObject    *object)
{
  GimpList *list;

  list = GIMP_LIST (container);

  list->list = g_list_remove (list->list, object);
}

static void
gimp_list_reorder (GimpContainer *container,
		   GimpObject    *object,
		   gint           new_index)
{
  GimpList *list;

  list = GIMP_LIST (container);

  list->list = g_list_remove (list->list, object);

  if (new_index == -1 || new_index == container->num_children - 1)
    list->list = g_list_append (list->list, object);
  else
    list->list = g_list_insert (list->list, object, new_index);
}

static gboolean
gimp_list_have (const GimpContainer *container,
		const GimpObject    *object)
{
  GimpList *list;

  list = GIMP_LIST (container);

  return g_list_find (list->list, object) ? TRUE : FALSE;
}

static void
gimp_list_foreach (const GimpContainer *container,
		   GFunc                func,
		   gpointer             user_data)
{
  GimpList *list;

  list = GIMP_LIST (container);

  g_list_foreach (list->list, func, user_data);
}

GimpContainer *
gimp_list_new (GType                children_type,
	       GimpContainerPolicy  policy)
{
  GimpList *list;

  g_return_val_if_fail (g_type_is_a (children_type, GIMP_TYPE_OBJECT), NULL);
  g_return_val_if_fail (policy == GIMP_CONTAINER_POLICY_STRONG ||
                        policy == GIMP_CONTAINER_POLICY_WEAK, NULL);

  list = g_object_new (GIMP_TYPE_LIST, NULL);

  GIMP_CONTAINER (list)->children_type = children_type;
  GIMP_CONTAINER (list)->policy        = policy;

  return GIMP_CONTAINER (list);
}

static GimpObject *
gimp_list_get_child_by_name (const GimpContainer *container,
			     const gchar         *name)
{
  GimpList   *list;
  GimpObject *object;
  GList      *glist;

  list = GIMP_LIST (container);

  for (glist = list->list; glist; glist = g_list_next (glist))
    {
      object = (GimpObject *) glist->data;

      if (! strcmp (object->name, name))
	return object;
    }

  return NULL;
}

static GimpObject *
gimp_list_get_child_by_index (const GimpContainer *container,
			      gint                 index)
{
  GimpList *list;
  GList    *glist;

  list = GIMP_LIST (container);

  glist = g_list_nth (list->list, index);

  if (glist)
    return (GimpObject *) glist->data;

  return NULL;
}

static gint
gimp_list_get_child_index (const GimpContainer *container,
			   const GimpObject    *object)
{
  GimpList *list;

  list = GIMP_LIST (container);

  return g_list_index (list->list, (gpointer) object);
}
