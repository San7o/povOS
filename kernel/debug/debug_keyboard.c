// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/debug.h>   // implements
#include <drivers/uart.h>

void debug_dump_keyboard_event_uart(keyboard_event_t event)
{
  const char* key_str = keyboard_string_from_keycode(event.key);

  uart_write_str(UART_COM1, "[debug] keyboard ");
  uart_write_str(UART_COM1, (event.pressed) ? "pressed " : "released ");
  uart_write_str(UART_COM1, key_str);
  uart_putc(UART_COM1, '\n');
  
  return;
}
