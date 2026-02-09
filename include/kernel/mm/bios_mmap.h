// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o


#ifndef POVOS_KERNEL_MM_BIOS_MMAP_H
#define POVOS_KERNEL_MM_BIOS_MMAP_H

//
// Bios Memory Map
// ---------------
//
// This header contains the structure of an entry for the system
// address map returned by int 0x15, ax=0xE820
//

typedef struct __attribute__((packed)) bios_mmap_entry {
  u32_t base_low;
  u32_t base_high;
  u32_t length_low;
  u32_t length_high;
  u32_t type;
  u32_t acpi;
} bios_mmap_entry_t;

#endif // POVOS_KERNEL_MM_BIOS_MMAP_H
