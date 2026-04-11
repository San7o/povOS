// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MACROS_H
#define POVOS_KERNEL_MACROS_H

#define _packed __attribute__((packed))

#define NOP __asm__ volatile ("nop")

#define UNUSED(value) (void)(value)

#endif // POVOS_KERNEL_MACROS_H
