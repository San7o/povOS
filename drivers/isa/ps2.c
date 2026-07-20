// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <bits/port.h>
#include <drivers/isa/ps2.h>   // implements

unsigned char ps2_read_data(void)
{
  return port_inb(PS2_DATA_REG);
}
