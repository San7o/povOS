// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sched.h>   // implements
#include <kernel/utils.h>
#include <kernel/klog.h>
#include <arch/cpu_regs.h>
#include <libk/string.h>
#include <libk/stdio.h>

struct scheduler  glob_scheduler = {0};
task_id_t  glob_current_task = 0;

void sched_switch_to(task_id_t task_id)
{  
  glob_current_task = task_id;

  if (!glob_scheduler.tasks[task_id].present) {
    // wait for next sched
    while(1) {
      NOP;
    }
  }

  ktrace("[sched] switching to task %l %s\n",
         task_id, glob_scheduler.tasks[task_id].task.name);
  
  cpu_do_context_switch(&glob_scheduler.tasks[task_id].task.regs);

  // Unreachable
}

void sched_init(struct vmmgr *vmmgr)
{
  memset(&glob_scheduler, 0, sizeof(struct scheduler));
  
  glob_scheduler.tasks[0] = (struct task_entry){
    .task = (struct task) {
      .vmmgr = vmmgr,
    },
    .sleeping = false,
    .present  = true,
  };
  strncpy((char*)&glob_scheduler.tasks[0].task.name, SCHED_INIT_TASK_NAME,
          TASK_NAME_LEN);

  // This immediately enables the interrupt to call the scheduler
  glob_scheduler.initialized = true;
}

void sched_switch_next(void)
{
  task_id_t task = sched_get_next_task();
  sched_switch_to(task);

  // Unreachable
}

task_id_t sched_get_next_task(void)
{
  // Round Robin
  
  // Get the next present task
  for (u64_t i = glob_current_task + 1; i < SCHED_MAX_TASKS; ++i) {
    if (glob_scheduler.tasks[i].present)
      return i;
  }

  return 0;
}

task_id_t sched_start_task(struct task task)
{
  // Find an available task slot
  for (u64_t i = 1; i < SCHED_MAX_TASKS; ++i) {
    if (glob_scheduler.tasks[i].present)
      continue;

    glob_scheduler.tasks[i].task    = task;
    glob_scheduler.tasks[i].present = true;
    
    return i;
  }
  
  return 0;
}

void sched_stop_task(task_id_t task)
{
  if (task >= SCHED_MAX_TASKS)
    return;
  glob_scheduler.tasks[task].present = false;
}

void sched_loop(void)
{
  hcf();
}
