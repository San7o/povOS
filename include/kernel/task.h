// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_TASK_H
#define POVOS_KERNEL_TASK_H

#include <libk/stddef.h>

#define TASK_NAME_LEN 16

typedef struct task_regs task_regs_t;

typedef struct task_regs {
  void* cr3;
  void* esp;
  // ...
};

typedef struct task {
  u64_t id;
  char name[TASK_NAME_LEN];
  task_contrrol_block_t cb;
  vmmgt_t *vmmgr;
} task_t;

extern task_t *current_task;

void task_init(void);
void task_switch(task_t *task);
task_t *task_create(void* instruction_pointer);

#endif // POVOS_KERNEL_TASK_H
