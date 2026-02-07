// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_DEBUG_H
#define POVOS_KERNEL_DEBUG_H

//
// Kernel debug
// ============
//
// Various debugging functions to inspect the state of the kernel
//

#include <drivers/input/keyboard.h>

// Implemented in assembly
void debug_dump_regs_uart(void);

void debug_dump_keyboard_event_uart(keyboard_event_t event);

#endif // POVOS_KERNEL_DEBUG_H
