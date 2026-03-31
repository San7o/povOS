// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>
#include <libk/stdbool.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <kernel/mm/pmmgr.h>
#include <kernel/mm/vmmgr.h>
#include <kernel/mm/bios_mmap.h>
#include <kernel/mm/heap.h>
#include <kernel/idt.h>
#include <kernel/debug.h>
#include <kernel/utils.h>
#include <kernel/input/input.h>
#include <kernel/textbuffer.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <drivers/pic.h>
#include <drivers/pit.h>
#include <drivers/hpet.h>
#include <drivers/uart.h>
#include <drivers/acpi.h>
#include <drivers/video/vga.h>
#include <drivers/input/keyboard.h>

#include "banner.h"

// C entrypoint
int kernel_main(void)
{
  //
  // Sanity checks
  //

  if (vga_get_memory_map() != 0b11)
  {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "Memory map range not supported", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  if (!vga_is_text_mode())
  {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "VGA alphanumeric mode is disabled", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  if (!uart_init_port(&uart_port1))
  {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "Error initializing uart", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  
  // Checks successfull

  //
  // Setup memory management
  //

  debug_print_memory_map_uart();

  pmmgr_init();
  debug_print_pmmgr_bitfield();

  vmmgr_t vmmgr;
  vmmgr_setup(&vmmgr);
  vmmgr_activate(&vmmgr);
  //debug_print_pmmgr_bitfield();

  size_t heap_size = 4 * 1024 * 1024;
  heap_init(&vmmgr, heap_size);
  void* some_mem = kmalloc(1024);
  (void) some_mem;

  debug_print_pmmgr_bitfield();
  
  acpi_rsdp_t* acpi_rsdp = acpi_locate_rsdp();
  if (!acpi_rsdp)
  {
    printk("[error] Could not find ACPI RSDP table\n");
  }
  else
  {
    printk("[info] Found ACPI RSDP at: %x\n", acpi_rsdp);
    
    hpet_acpi_sdt_t* hpet = acpi_locate_sdt(acpi_rsdp, HPET_ACPI_SIGNATURE);
    if (!hpet)
      printk("[error] Could not find HPET timer\n");
    else
      printk("[info] Found HPET timer\n");
  }

  //
  // Setup interrupts
  //

  pit_set_count(1193); // one tick per millisecond
  
  pic_remap();  // Chage overlapping IRQ numbers
  idt_set();    // Setup the IDT

  // Tests
  debug_print_vga();
  debug_write_uart();
  debug_dump_regs_uart();
  debug_enumerate_pci_devices();
  // breakpoint();

  //
  // Setup tty
  //
  
  textbuffer_entry_t buff[VGA_TEXT_BUFFER_SIZE] = {0};
  textbuffer_t textbuffer;
  textbuffer_init(&textbuffer, buff,
                  VGA_TEXT_BUFFER_WIDTH, VGA_TEXT_BUFFER_HEIGHT, 0, 0);

  tty_t tty;
  tty_init(&tty, &textbuffer, TEXTBUFFER_STYLE_BW, &vga_console);
  
  input_t input;
  input_init(&input, &input_keymap_us, &tty);
  
  keyboard_t keyboard;
  keyboard_init(&keyboard, KEYBOARD_TYPE_PS2_SET1, &input);
  keyboard_set_active(&keyboard);

  tty_write(&tty, povos_banner, strlen(povos_banner));
  tty_flush(&tty);

  //vga_set_graphics_mode();
  //debug_vga_draw_flag();
  
  // Read and print keyboard input
  debug_dump_input_loop(&input);

  while(1) {}
  
  return EXIT_SUCCESS;
}
