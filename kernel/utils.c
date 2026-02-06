// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/utils.h>   // implements
#include <bits/port.h>

// Do an IO operation in an unused port
void io_wait(void)
{
  port_outb(0x80, 0);
  return;
}
