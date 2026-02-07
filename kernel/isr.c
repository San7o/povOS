// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/isr.h>   // implements
#include <kernel/debug.h>
#include <drivers/uart.h>
#include <drivers/ps2.h>
#include <drivers/pic.h>
#include <drivers/input/keyboard.h>

void isr_common_handler(u8_t  isr_number,
                        u64_t error_code)
{
  uart_write_str(UART_COM1, "isr ");
  uart_write_hex(UART_COM1, isr_number);
  uart_write_str(UART_COM1, " , error code: ");
  uart_write_hex(UART_COM1, error_code);
  uart_putc(UART_COM1, '\n');

  pic_ack();
  return;
}

// Keyboard interrupt
void isr_keyboard_handler(u8_t  isr_number,
                          u64_t error_code)
{
  (void) isr_number;
  (void) error_code;

  keyboard_t *keyboard = keyboard_get_active();
  if (!keyboard) goto exit;
  
  u8_t scancode = ps2_read_data();
  keyboard_event_t event =
    keyboard_event_from_scancode(keyboard, scancode);
  if (event.key == KEY_NONE) goto exit;

  // Update the keyboard
  keyboard_update(keyboard, event);
  
  // Debug print via UART
  // debug_dump_keyboard_event_uart(event);

 exit:
  pic_ack();
  return;
}
