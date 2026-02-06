// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/pic.h>   // implements
#include <bits/port.h>
#include <kernel/utils.h>

void pic_remap(void)
{
  // ICW1: Start initialization in cascade mode
  port_outb(PIC_MASTER_CMD, PIC_ICW1_INIT | PIC_ICW1_ICW4);
  io_wait();
  
  port_outb(PIC_SLAVE_CMD, PIC_ICW1_INIT | PIC_ICW1_ICW4);
  io_wait();

  // ICW2: Set vector offsets
  // Master PIC offsets to 0x20 (Int 32)
  port_outb(PIC_MASTER_DATA, 0x20);
  io_wait();

  // Slave PIC offsets to 0x28 (Int 40)
  port_outb(PIC_SLAVE_DATA, 0x28);
  io_wait();
  
  // ICW3: Tell master PIC there is a slave at IRQ 2
  port_outb(PIC_MASTER_DATA, 1 << 2);
  io_wait();
    
  // Tell slave PIC its cascade identity (2)
  port_outb(PIC_SLAVE_DATA, 2);
  io_wait();
  
  // ICW4: Set mode to 8086
  port_outb(PIC_MASTER_DATA, PIC_ICW4_8086);
  io_wait();
  port_outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
  io_wait();

  // Unmask Keyboard (IRQ 1)
  port_outb(PIC_MASTER_DATA, 0b11111101);
  io_wait();

  // Keep all slave interrupts masked for now
  port_outb(PIC_SLAVE_DATA, 0xFF);
  io_wait();
  
  return;
}

void pic_ack(void)
{
  port_outb(PIC_MASTER_CMD, PIC_OCW3_EOI);
  return;
}

void pic_disable(void)
{
  port_outb(PIC_MASTER_DATA, 0xFF);
  port_outb(PIC_SLAVE_DATA,  0xFF);
  return;
}
