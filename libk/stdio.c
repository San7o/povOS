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
  int ret = uart_vprintf(uart_port1, fmt, list);
  va_end(list);

  return ret;
}
