// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/isa/pit.h>   // implements
#include <kernel/utils.h>
#include <bits/port.h>

u16_t pit_read_count(void)
{
  disable_interrupts();

  // Select channel 0
  port_outb(PIT_MODE_REGISTER, 0);

  u16_t count = 0;
  count  = port_inb(PIT_CHANNEL_0_DATA_PORT);       // low byte
  count |= port_inb(PIT_CHANNEL_0_DATA_PORT) << 8;  // high byte
  
  enable_interrupts();

  return count;
}

void pit_set_count(u16_t count)
{
  port_outb(PIT_MODE_REGISTER, 0x34);   // 0b00110100
  port_outb(PIT_CHANNEL_0_DATA_PORT, count & 0xFF);
  port_outb(PIT_CHANNEL_0_DATA_PORT, (count & 0xFF00) >> 8);
}
