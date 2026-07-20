// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_TASK_H
#define POVOS_KERNEL_TASK_H

#include <arch/cpu_regs.h>
#include <libk/stddef.h>
#include <mm/vmmgr.h>

#define TASK_NAME_LEN 16

struct task {
  char  name[TASK_NAME_LEN];
  struct vmmgr  *vmmgr;
  struct cpu_regs  regs;
};

struct task task_create(struct cpu_regs regs, struct vmmgr *vmmgr,
                        const char name[TASK_NAME_LEN]);

#endif // POVOS_KERNEL_TASK_H
