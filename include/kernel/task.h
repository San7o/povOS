// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_TASK_H
#define POVOS_KERNEL_TASK_H

#include <libk/stddef.h>
#include <kernel/mm/vmmgr.h>
#include <arch/cpu_regs.h>

#define TASK_NAME_LEN 16

typedef struct task {
  u64_t      id;
  char       name[TASK_NAME_LEN];
  vmmgr_t   *vmmgr;
} task_t;

extern task_t *current_task;

void    task_init(void);
task_t *task_create(cpu_regs_t regs);

#endif // POVOS_KERNEL_TASK_H
