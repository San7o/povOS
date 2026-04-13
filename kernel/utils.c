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
  port_outb(0x80, 0);
  port_outb(0x80, 0);
  return;
}

// keyboard interface IO port: data and control
// READ:   status port
// WRITE:  control register
#define KBRD_INTRFC 0x64

// keyboard interface bits
#define KBRD_BIT_KDATA 1 << 0 // keyboard data is in buffer (output buffer is empty) (bit 0)
#define KBRD_BIT_UDATA 1 << 1 // user data is in buffer (command buffer is empty) (bit 1)

#define KBRD_IO    0x60    // keyboard IO port
#define KBRD_RESET 0xFE    // reset CPU command

void reboot(void)
{
  u8_t temp;

  disable_interrupts();

  // Clear all keyboard buffers (output and command buffers)
  do
  {
    temp = port_inb(KBRD_INTRFC);       // empty user data
    if ((temp & KBRD_BIT_KDATA) != 0)
      port_inb(KBRD_IO);                // empty keyboard data
  } while ((temp & KBRD_BIT_UDATA) != 0);

  port_outb(KBRD_INTRFC, KBRD_RESET);   // pulse CPU reset line

  while(1) { halt_cpu(); }
}
