// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_SCHED_H
#define POVOS_KERNEL_SCHED_H

#include <kernel/task.h>

void sched_switch_to(cpu_regs_t regs, task_t *task);

#endif // POVOS_KERNEL_SCHED_H
