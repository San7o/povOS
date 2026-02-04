// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/ps2.h>
#include <bits/port.h>

unsigned char ps2_read_data(void)
{
  return port_inb(PS2_DATA_REG);
}
