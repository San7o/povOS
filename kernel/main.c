// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>
#include <libk/stdbool.h>
#include <libk/string.h>
#include <kernel/mm/pmmgr.h>
#include <kernel/mm/vmmgr.h>
#include <kernel/mm/bios_mmap.h>
#include <kernel/idt.h>
#include <kernel/debug.h>
#include <kernel/utils.h>
#include <kernel/input/input.h>
#include <kernel/textbuffer.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <drivers/pic.h>
#include <drivers/uart.h>
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

  if (vga_is_alpha_disabled() == true)
  {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "VGA alphanumeric mode is disabled", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  if (uart_init_port(UART_COM1) != true)
  {
    vga_clear(VGA_STYLE_RED);
    vga_print(0, "Error initializing uart", VGA_STYLE_BW);
    return EXIT_FAILURE;
  }

  // Checks successfull

  //
  // Setup memory management
  //

  u32_t *mmap_num_entries = (void*) 0x5000;
  bios_mmap_entry_t *mmap = (void*) 0x5004;
  debug_print_memory_map_uart(mmap, *mmap_num_entries);

  pmmgr_t pmmgr;
  pmmgr_init(&pmmgr, mmap, *mmap_num_entries);
  
  vmmgr_t vmmgr;
  vmmgr_setup(&vmmgr);
  vmmgr_activate(&vmmgr);

  //
  // Setup interrupts
  //
  
  pic_remap();  // Chage overlapping IRQ numbers
  idt_set();    // Setup the IDT

  // Tests
  debug_print_vga();
  debug_write_uart();
  debug_dump_regs_uart();
  breakpoint();

  //
  // Setup tty
  //
  
  textbuffer_entry_t buff[VGA_BUFFER_SIZE] = {0};
  textbuffer_t textbuffer;
  textbuffer_init(&textbuffer, buff, VGA_WIDTH, VGA_HEIGHT, 0, 0);

  tty_t tty;
  tty_init(&tty, &textbuffer, TEXTBUFFER_STYLE_BW, &vga_console);
  
  input_t input;
  input_init(&input, &input_keymap_us, &tty);
  
  keyboard_t keyboard;
  keyboard_init(&keyboard, KEYBOARD_TYPE_PS2_SET1, &input);
  keyboard_set_active(&keyboard);

  tty_write(&tty, povos_banner, strlen(povos_banner));
  tty_flush(&tty);

  // Read and print keyboard input
  debug_dump_input_loop(&input);
  
  while(1) {}
  
  return EXIT_SUCCESS;
}
