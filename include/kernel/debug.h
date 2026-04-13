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
#include <drivers/pcie.h>
#include <kernel/input/input.h>
#include <mm/pmmgr.h>
#include <arch/cpu_regs.h>

//
// CPU
//

void debug_dump_regs_uart(void);   // implemented in assembly
void debug_dump_regs_uart2(void);
void debug_dump_regs_uart3(cpu_regs_t regs);

//
// Memory
//

// Dump memory contents up to count (maximum value is PAGE_SIZE)
void debug_dump_mem_page(phys_addr_t mem, size_t count);
void debug_print_memory_map_uart(void);
void debug_print_pmmgr_bitfield(void);

//
// Devices
//

void debug_enumerate_pci_devices(void);
void debug_enumerate_pcie_devices(pcie_acpi_sdt_t *pcie_sdt);

//
// Misc
//

// Output

void debug_print_vga(void);
void debug_write_uart(void);
void debug_vga_draw_flag(void);

// Input

void debug_dump_input_loop(input_t *input, void* hpet_base_reg);
void debug_dump_keyboard_event_uart(keyboard_event_t event);
void debug_dump_input_event_uart(input_event_t event);

void debug_sleep(void);
void debug_print_true_rand(void);

// Test functions that print a message in a loop, they never return
// This can be used as test tasks to debug the scheduler
void debug_test_task_a_fn(void);
void debug_test_task_b_fn(void);

#endif // POVOS_KERNEL_DEBUG_H
