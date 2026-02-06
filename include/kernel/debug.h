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

// Implemented in assembly
void debug_dump_regs_uart(void);

#endif // POVOS_KERNEL_DEBUG_H
