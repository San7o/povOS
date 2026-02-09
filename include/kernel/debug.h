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
#include <kernel/input/input.h>
#include <kernel/mm/bios_mmap.h>

// Implemented in assembly
void debug_dump_regs_uart(void);

void debug_dump_keyboard_event_uart(keyboard_event_t event);
void debug_dump_input_event_uart(input_event_t event);
void debug_print_vga(void);
void debug_write_uart(void);
void debug_dump_input_loop(input_t *input);
void debug_print_memory_map_uart(u32_t *num_entries,
                                 bios_mmap_entry_t *memory_map);

#endif // POVOS_KERNEL_DEBUG_H
