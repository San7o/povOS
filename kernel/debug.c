// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/debug.h>   // implements
#include <drivers/uart.h>
#include <drivers/video/vga.h>

void debug_dump_keyboard_event_uart(keyboard_event_t event)
{
  const char* key_str = keycode_to_string(event.key);

  uart_write_str(UART_COM1, "[debug] [keyboard event] ");
  uart_write_str(UART_COM1, (event.pressed) ? "pressed  " : "released ");
  uart_write_str(UART_COM1, key_str);
  uart_putc(UART_COM1, '\n');
  
  return;
}

void debug_dump_input_event_uart(input_event_t event)
{
  uart_write_str(UART_COM1, "[debug] [input event] ");

  switch (event.type)
  {
  case INPUT_EVENT_TYPE_KEYBOARD: ;
    const char* key_str = keycode_to_string(event.e.key.key);
    uart_write_str(UART_COM1, (event.e.key.pressed) ? "pressed " : "released ");
    uart_write_str(UART_COM1, key_str);
    break;
  case INPUT_EVENT_TYPE_CHAR:
    uart_putc(UART_COM1, event.e.c);
    break;
  default:
    uart_write_str(UART_COM1, "unknown");
    break;
  }
  
  uart_putc(UART_COM1, '\n');
  return;
}

void debug_print_vga(void)
{
  vga_clear(VGA_STYLE_BLUE);
  vga_print(0, "Hello, from povOS!", VGA_STYLE_BLUE);
  vga_print_hex(18, 0x6969, VGA_STYLE_BW);
  return;
}

void debug_write_uart(void)
{
  uart_write_str(UART_COM1, "Hello, from povOS!");
  uart_write_hex(UART_COM1, 0x6969);
  uart_putc(UART_COM1, '\n');
  return;
}

void debug_dump_input_loop(input_t *input)
{
  while(1) {
    input_event_t event = input_events_get(input);
    if (event.type == INPUT_EVENT_TYPE_NONE) continue;
    
    debug_dump_input_event_uart(event);
  }
}
