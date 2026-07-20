// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/task.h>   // implements
#include <libk/string.h>

struct task task_create(struct cpu_regs regs, struct vmmgr *vmmgr,
                        const char name[TASK_NAME_LEN])
{
  struct task task = {
    .regs  = regs,
    .vmmgr = vmmgr,
  };
  strncpy(&task.name[0], name, TASK_NAME_LEN);

  return task;
}
