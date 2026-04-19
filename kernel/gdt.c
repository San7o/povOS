// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/gdt.h>   // implements
#include <mm/layout.h>

void gdt_reload(void)
{
  gdtr_t gdtr;
  __asm__ volatile("sgdt %0" : "=m"(gdtr));

  gdtr.base = (u64_t)MM_PHYS_TO_VIRT(gdtr.base);

  __asm__ volatile("lgdt %0" : : "m"(gdtr));
    
  gdt_flush();
}
