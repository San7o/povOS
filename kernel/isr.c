// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/isr.h>   // implements
#include <drivers/uart.h>
#include <drivers/ps2.h>
#include <drivers/pic.h>

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

void isr33_handler(u8_t  isr_number,
                   u64_t error_code)
{
  (void) isr_number;
  (void) error_code;
  
  unsigned char scancode = ps2_read_data();
  
  uart_write_str(UART_COM1, "key: ");
  uart_write_hex(UART_COM1, scancode);
  uart_putc(UART_COM1, '\n');
  
  pic_ack();
  return;
}
