// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_SCHED_H
#define POVOS_KERNEL_SCHED_H

#include <kernel/task.h>
#include <libk/stdbool.h>

#define SCHED_MAX_TASKS

typedef u64_t task_id_t;

typedef struct task_entry {
  task_t task;
  bool   sleeping;
} task_entry_t;

typedef struct sheduler {
  task_entry_t tasks[SCHED_MAX_TASKS];
} scheduler_t;

extern scheduler_t  scheduler;
extern task_id_t    current_task;

void sched_switch_to(task_id_t task_id);

task_id_t sched_start_task(task_t task);
void sched_stop_task(task_id_t task);

void sched_loop(void);

#endif // POVOS_KERNEL_SCHED_H
