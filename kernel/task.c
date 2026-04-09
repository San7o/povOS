// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/task.h>   // implements
#include <libk/string.h>

task_t task_create(cpu_regs_t regs, vmmgr_t *vmmgr,
                   const char name[TASK_NAME_LEN])
{  
  task_t task = {
    .regs  = regs,
    .vmmgr = vmmgr,
  };
  strncpy(&task.name[0], name, TASK_NAME_LEN);
  
  return task;
}
