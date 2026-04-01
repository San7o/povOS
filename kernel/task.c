// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/task.h>   // implements

task_t *current_task = NULL;

// Initialize current_task
void task_init(void)
{
  // TODO
}

task_t *task_create(cpu_regs_t regs)
{
  (void) regs;
  // TODO

  task_t task = {
  };
  (void) task;
  
  return NULL;
}
