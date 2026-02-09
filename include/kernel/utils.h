// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_UTILS_H
#define POVOS_KERNEL_UTILS_H

//
// Utils
// =====
//
// General utility functions that can be useful in several occasions.
//

#define MAX(x, y) ((x > y) ? (x) : (y))
#define MIN(x, y) ((x < y) ? (x) : (y))

// Wait a very small amount of time (1 to 4 microseconds, generally)
void io_wait(void);
// Set a breakpoint
void breakpoint(void);

void disable_interrupts(void);
void enable_interrupts(void);

#endif // POVOS_KERNEL_UTILS_H
