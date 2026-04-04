// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sched.h>   // implements

task_t current_task;

void sched_switch_to(task_t task)
{
  cpu_do_context_switch(&task.regs);
}
