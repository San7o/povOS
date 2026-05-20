// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_SCHED_H
#define POVOS_KERNEL_SCHED_H

#include <kernel/task.h>
#include <libk/stdbool.h>

#define SCHED_MAX_TASKS 1024
#define SCHED_FREQ      1    // schedules per seconds
#define SCHED_INIT_TASK_NAME "kernel\0"

typedef u64_t task_id_t;

struct task_entry {
  struct task task;
  bool   sleeping;
  bool   present;
};

struct scheduler {
  struct task_entry  tasks[SCHED_MAX_TASKS];
  bool  initialized;
};

extern struct scheduler  scheduler;
extern task_id_t  current_task;

void  sched_init(struct vmmgr *vmmgr);
void  sched_loop(void);

// Get the next task to be executed
task_id_t sched_get_next_task(void);
// Performs the context switch to some task
void      sched_switch_to(task_id_t task_id);
// Performs the context switch to the next task
void      sched_switch_next(void);

task_id_t sched_start_task(struct task task);
void      sched_stop_task(task_id_t task);

#endif // POVOS_KERNEL_SCHED_H
