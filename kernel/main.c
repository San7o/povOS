// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>
#include <libk/stdbool.h>
#include <kernel/idt.h>
#include <kernel/debug.h>
#include <drivers/pic.h>
#include <drivers/vga.h>
#include <drivers/uart.h>
#include <drivers/input/keyboard.h>

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

  //
  // Checks successfull
  //

  pic_remap();  // Chage overlapping IRQ numbers
  idt_set();    // Setup the IDT

  // Print tests
  
  vga_clear(VGA_STYLE_BLUE);
  vga_print(0, "Hello, from povOS!", VGA_STYLE_BLUE);
  vga_print_hex(18, 0x6969, VGA_STYLE_BW);
  
  uart_write_str(UART_COM1, "Hello, from povOS!");
  uart_write_hex(UART_COM1, 0x6969);
  uart_putc(UART_COM1, '\n');

  debug_dump_regs_uart();

  // Create a keyboard
  keyboard_t keyboard;
  keyboard_init(&keyboard, KEYBOARD_TYPE_PS2_SET1);
  keyboard_set_active(&keyboard);

  while(1) {}
  
  return EXIT_SUCCESS;
}
