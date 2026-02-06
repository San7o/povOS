// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/uart.h>   // implements
#include <bits/port.h>

bool uart_init_port(port_t port)
{
  // Disable all interrupts
  port_outb(port + UART_REGISTER_INTERRUPT_ENABLE_INDEX, 0);
  // Enable DLAB (set baud rate divisor)
  port_outb(port + UART_REGISTER_LINE_CONTROL_INDEX, 1 << 7);
  // Set divisor to 3, 38499 baud
  port_outb(port + UART_REGISTER_DIVISOR_LOW_INDEX, 0x03);
  port_outb(port + UART_REGISTER_DIVISOR_HIGH_INDEX, 0);
  // 8 bits, no parity, one stop bit
  port_outb(port + UART_REGISTER_LINE_CONTROL_INDEX, 0x03);
  // Enable FIFO, clear them, with 14-byte threshold
  port_outb(port + UART_REGISTER_FIFO_CONTROL_INDEX, 0xC7);
  // IRQs enable, RTS, RSR set
  port_outb(port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x0B);
  
  // Test

  u8_t test_byte = 0xAE;
  // Set in loopback mode
  port_outb(port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x1E);
  // Send byte 0xAE
  port_outb(port + UART_REGISTER_TRANSMIT_BUF_INDEX, test_byte);
  // Check if serial returns same byte
  u8_t received_byte =
    port_inb(port + UART_REGISTER_RECEIVE_BUF_INDEX);
  if (received_byte != test_byte)
    return false;
  
  // If serial is not faulty, set it in normal operation mode
  port_outb(port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x0F);
  return true;
}

bool uart_is_transmit_ready(port_t port)
{
  u16_t line_status =
    port_inw(port + UART_REGISTER_LINE_STATUS_INDEX);
  
  if ((line_status & (1 << 5)) == 0)
    return false;
  
  return true;
}

void uart_putc(port_t port, u8_t c)
{
  // Wait for transmission to be available
  while(!uart_is_transmit_ready(port)) {}

  port_outb(port + UART_REGISTER_TRANSMIT_BUF_INDEX, c);
  
  return;
}

void uart_write_str(port_t port, const char *str)
{
  while (*str != '\0')
  {
    uart_putc(port, *str);
    ++str;
  }
  return;
}

void uart_write_hex(port_t port, u64_t num)
{
  uart_putc(port, '0');
  uart_putc(port, 'x');
  
  for (unsigned long i = 0; i < sizeof(num) * 2; ++i)
  {
    u8_t hex = (num >> (sizeof(num) * 8 - 4 * i - 4)) & 0xF;
    if (hex > 9)
    {
      uart_putc(port, 'A' + hex - 10);
    }
    else
    {
      uart_putc(port, '0' + hex);
    }
  }
}
