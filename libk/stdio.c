// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdio.h>   // implements
#include <stdarg.h>
#include <drivers/isa/uart.h>

int printk(const char* fmt, ...)
{
  va_list list;
  va_start(list, fmt);
  vprintk(fmt, list);
  int ret = vprintk(fmt, list);
  va_end(list);

  return ret;
}

int vprintk(const char* fmt, va_list list)
{
  return uart_vprintf(uart_port1, fmt, list);
}
