/*
 * dLeyna
 *
 * Copyright (c) 2019 Jens Georg <mail@jensge.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "gasync-task.h"
#include <libdleyna/core/task-processor.h>

struct dleyna_gasync_task_t_ {
	dleyna_task_atom_t base;
	dleyna_gasync_task_action action;
	GObject *target;
	GCancellable *cancellable;
	GDestroyNotify free_func;
	gboolean current;
	gpointer cb_user_data;
};

const char *dleyna_gasync_task_create_source(void)
{
	static unsigned int cpt = 1;
	static char source[27];

	g_snprintf(source, 27, "gasync-source-%d", cpt);
	cpt++;

	return source;
}

void dleyna_gasync_task_add(const dleyna_task_queue_key_t *queue_id,
		dleyna_gasync_task_action action,
		GObject *target,
		GCancellable *cancellable,
		GDestroyNotify free_func,
		gpointer cb_user_data)
{
	dleyna_gasync_task_t *task;

	task = g_new0(dleyna_gasync_task_t, 1);

	task->action = action;
	task->cancellable = cancellable;
	task->free_func = free_func;
	task->cb_user_data = cb_user_data;
	task->target = target;

	if (target != NULL) {
		g_object_add_weak_pointer (target, (gpointer *)(&task->target));
	}

	dleyna_task_queue_add_task(queue_id, &task->base);
}

void dleyna_gasync_task_process_cb(dleyna_task_atom_t *atom,
		gpointer user_data)
{
	dleyna_gasync_task_t *task = (dleyna_gasync_task_t *)atom;

	task->current = TRUE;
	task->action(task, task->target);
}

void dleyna_gasync_task_cancel_cb(dleyna_task_atom_t *atom,
		gpointer user_data)
{
	dleyna_gasync_task_t *task = (dleyna_gasync_task_t *)atom;

	if (task->cancellable) {
		g_cancellable_cancel (task->cancellable);
		task->cancellable = NULL;

		if (task->current)
			dleyna_task_queue_task_completed(task->base.queue_id);
	}
}

void dleyna_gasync_task_delete_cb(dleyna_task_atom_t *atom,
		gpointer user_data)
{
	dleyna_gasync_task_t *task = (dleyna_gasync_task_t *)atom;

	if (task->free_func != NULL)
		task->free_func(task->cb_user_data);

	if (task->target != NULL) {
		g_object_remove_weak_pointer(task->target, (gpointer *)&task->target);
	}

	g_free(task);
}

gpointer dleyna_gasync_task_get_user_data(dleyna_gasync_task_t *task)
{
	return task->cb_user_data;
}

GCancellable *dleyna_gasync_task_get_cancellable(dleyna_gasync_task_t *task)
{
	return task->cancellable;
}
