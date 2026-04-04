// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sched.h>   // implements
#include <kernel/utils.h>

scheduler_t  scheduler;
task_id_t    current_task;

void sched_switch_to(task_id_t task_id)
{
  current_task = task_id;
  // TODO
}

task_id_t sched_start_task(task_t task)
{
  (void) task;
  // TODO
  return 0;
}

void sched_stop_task(task_id_t task)
{
  (void) task;
}

void sched_loop(void)
{
  // TODO: enable scheduler interrupt
  
  while(1)
  {
    halt_cpu();
  }
}
