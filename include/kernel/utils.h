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

#include <libk/stddef.h>

#define MAX(x, y) ((x > y) ? (x) : (y))
#define MIN(x, y) ((x < y) ? (x) : (y))

// Generate a tru random number with hardware support
// If this is not supported, returns 0
u32_t true_rand(void);

// Wait a very small amount of time (1 to 4 microseconds, generally)
void io_wait(void);
// Set a breakpoint
void breakpoint(void);

void disable_interrupts(void);
void enable_interrupts(void);

// Halt the CPU until the next external interrupt occurs, effectively
// putting the processor in a low-power state. 
void cpu_halt(void);

// Halt and catch fire
//
// Halt the CPU forever. The name is an old meme for OS developers.
void hcf(void);

void reboot(void);

#endif // POVOS_KERNEL_UTILS_H
