// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/uart.h>   // implements
#include <bits/port.h>
#include <libk/stdlib.h>

#define UART_PORT_MAKE(_port) (uart_port_t) { .port = _port, .initialized = false}
uart_port_t uart_port1 = UART_PORT_MAKE(UART_COM1);
uart_port_t uart_port2 = UART_PORT_MAKE(UART_COM2);
uart_port_t uart_port3 = UART_PORT_MAKE(UART_COM3);
uart_port_t uart_port4 = UART_PORT_MAKE(UART_COM4);
uart_port_t uart_port5 = UART_PORT_MAKE(UART_COM5);
uart_port_t uart_port6 = UART_PORT_MAKE(UART_COM6);
uart_port_t uart_port7 = UART_PORT_MAKE(UART_COM7);
uart_port_t uart_port8 = UART_PORT_MAKE(UART_COM8);

bool uart_init_port(uart_port_t* uart_port)
{
  // Disable all interrupts
  port_outb(uart_port->port + UART_REGISTER_INTERRUPT_ENABLE_INDEX, 0);
  // Enable DLAB (set baud rate divisor)
  port_outb(uart_port->port + UART_REGISTER_LINE_CONTROL_INDEX, 1 << 7);
  // Set divisor to 3, 38499 baud
  port_outb(uart_port->port + UART_REGISTER_DIVISOR_LOW_INDEX, 0x03);
  port_outb(uart_port->port + UART_REGISTER_DIVISOR_HIGH_INDEX, 0);
  // 8 bits, no parity, one stop bit
  port_outb(uart_port->port + UART_REGISTER_LINE_CONTROL_INDEX, 0x03);
  // Enable FIFO, clear them, with 14-byte threshold
  port_outb(uart_port->port + UART_REGISTER_FIFO_CONTROL_INDEX, 0xC7);
  // IRQs enable, RTS, RSR set
  port_outb(uart_port->port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x0B);
  
  // Test

  u8_t test_byte = 0xAE;
  // Set in loopback mode
  port_outb(uart_port->port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x1E);
  // Send byte 0xAE
  port_outb(uart_port->port + UART_REGISTER_TRANSMIT_BUF_INDEX, test_byte);
  // Check if serial returns same byte
  u8_t received_byte =
    port_inb(uart_port->port + UART_REGISTER_RECEIVE_BUF_INDEX);
  if (received_byte != test_byte)
    return false;
  
  // If serial is not faulty, set it in normal operation mode
  port_outb(uart_port->port + UART_REGISTER_MODEM_CONTROL_INDEX, 0x0F);

  uart_port->initialized = true;
  return true;
}

bool uart_is_transmit_ready(uart_port_t uart_port)
{
  if (!uart_port.initialized)
    return false;
  
  u16_t line_status =
    port_inw(uart_port.port + UART_REGISTER_LINE_STATUS_INDEX);
  
  if ((line_status & (1 << 5)) == 0)
    return false;
  
  return true;
}

void uart_putc(uart_port_t uart_port, u8_t c)
{
  if (!uart_port.initialized)
    return;
  
  // Wait for transmission to be available
  while(!uart_is_transmit_ready(uart_port)) {}

  port_outb(uart_port.port + UART_REGISTER_TRANSMIT_BUF_INDEX, c);
  
  return;
}

void uart_write_str(uart_port_t uart_port, const char *str)
{
  if (!uart_port.initialized)
    return;
  
  while (*str != '\0')
  {
    uart_putc(uart_port, *str);
    ++str;
  }
  return;
}

void uart_write_hex(uart_port_t uart_port, u64_t num)
{
  if (!uart_port.initialized)
    return;
  
  uart_putc(uart_port, '0');
  uart_putc(uart_port, 'x');
  
  for (unsigned long i = 0; i < sizeof(num) * 2; ++i)
  {
    u8_t hex = (num >> (sizeof(num) * 8 - 4 * i - 4)) & 0xF;
    if (hex > 9)
    {
      uart_putc(uart_port, 'A' + hex - 10);
    }
    else
    {
      uart_putc(uart_port, '0' + hex);
    }
  }

  return;
}

int uart_printf(uart_port_t uart_port, const char* fmt, ...)
{
  if (!uart_port.initialized)
    return 0;
  
  va_list list;
  va_start(list, fmt);
  int ret = uart_vprintf(uart_port, fmt, list);
  va_end(list);

  return ret;
}

int uart_vprintf(uart_port_t uart_port, const char* fmt, va_list args)
{
  if (!uart_port.initialized)
    return 0;
  
  while (*fmt)
  {
    if (*fmt == '%')
    {
      fmt++;
      switch (*fmt)
      {
      case 'd':
      case 'i': {
        char buf[32];
        itoa(va_arg(args, int), buf, 10);
        uart_write_str(uart_port, buf);
        break;
      }
      case 'x': {
        char buf[32];
        itoa(va_arg(args, unsigned int), buf, 16);
        uart_write_str(uart_port, "0x");
        uart_write_str(uart_port, buf);
        break;
      }
      case 's': {
        char* s = va_arg(args, char*);
        if (!s) s = "(null)";
        uart_write_str(uart_port, s);        
        break;
      }
      case 'f': {
        double f = va_arg(args, double);
        char buf[64];
        // Handle negative
        if (f < 0)
        {
          uart_putc(uart_port, '-');
          f = -f;
        }
        // Integer part
        long i_part = (long)f;
        itoa(i_part, buf, 10);
        uart_write_str(uart_port, buf);
        uart_putc(uart_port, '.');
        // Fractional part (6 decimal places)
        long f_part = (long)((f - (double)i_part) * 1000000.0 + 0.5);
        itoa(f_part, buf, 10);
        // Add leading zeros to fraction if necessary
        int len = 0; while(buf[len]) len++;
        for(int z = 0; z < (6 - len); z++)
          uart_putc(uart_port, '0');
        uart_write_str(uart_port, buf);
        break;
      }
      case '%': {
        uart_putc(uart_port, '%');
        break;
      }
      default: {
        uart_putc(uart_port, '%');
        uart_putc(uart_port, *fmt);
        break;
      }
      }
    }
    else
    {
      uart_putc(uart_port, *fmt);
    }
    fmt++;
  }
  return 0;
}
