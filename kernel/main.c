// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>
#include <libk/stdbool.h>
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

  // Test breakpoint
  breakpoint();

  //
  // Setup tty
  //
  
  textbuffer_entry_t buff[VGA_BUFFER_SIZE] = {0};
  textbuffer_t textbuffer;
  textbuffer_init(&textbuffer, buff, VGA_WIDTH, VGA_HEIGHT, 0, 0);

  tty_t tty;
  tty_init(&tty, &textbuffer, &vga_console);
  
  input_t input;
  input_init(&input, &input_keymap_us, &tty);
  
  keyboard_t keyboard;
  keyboard_init(&keyboard, KEYBOARD_TYPE_PS2_SET1, &input);
  keyboard_set_active(&keyboard);

  // Read keyboard input
  while(1) {
    input_event_t event = input_events_get(&input);
    if (event.type == INPUT_EVENT_TYPE_NONE) continue;
    
    debug_dump_input_event_uart(event);
  }
  
  return EXIT_SUCCESS;
}
