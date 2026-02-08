// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/debug.h>   // implements
#include <drivers/uart.h>

void debug_dump_keyboard_event_uart(keyboard_event_t event)
{
  const char* key_str = keycode_from_string(event.key);

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
    const char* key_str = keycode_from_string(event.e.key.key);
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
