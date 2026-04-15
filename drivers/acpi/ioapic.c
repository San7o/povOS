// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/acpi/ioapic.h>   // implements

void ioapic_write_reg(u64_t apic_base, u8_t offset, u32_t val) 
{
  *(volatile u32_t*)(IOAPIC_IOREGSEL_ADDR(apic_base)) = offset;
  *(volatile u32_t*)(IOAPIC_IOWIN_ADDR(apic_base)) = val; 
}
 
u32_t ioapic_read_reg(u64_t apic_base, u8_t offset)
{
  *(volatile u32_t*)(IOAPIC_IOREGSEL_ADDR(apic_base)) = offset;
  return *(volatile u32_t*)(IOAPIC_IOWIN_ADDR(apic_base));
}
