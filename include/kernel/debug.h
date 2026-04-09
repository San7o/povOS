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
#include <drivers/video/vga.h>
#include <kernel/input/input.h>
#include <arch/cpu_regs.h>

// Implemented in assembly
void debug_dump_regs_uart(void);

void debug_dump_regs_uart3(cpu_regs_t regs);
void debug_dump_regs_uart2(void);
void debug_dump_keyboard_event_uart(keyboard_event_t event);
void debug_dump_input_event_uart(input_event_t event);
void debug_print_vga(void);
void debug_write_uart(void);
void debug_dump_input_loop(input_t *input, void* hpet_base_reg);
void debug_print_memory_map_uart(void);
void debug_print_pmmgr_bitfield(void);
void debug_vga_draw_flag(void);
void debug_enumerate_pci_devices(void);
void debug_sleep(void);
// Test functions that print a message in a loop
void debug_test_task_a_fn(void);
void debug_test_task_b_fn(void);

#endif // POVOS_KERNEL_DEBUG_H
