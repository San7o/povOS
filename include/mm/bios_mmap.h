// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_BIOS_MMAP_H
#define POVOS_MM_BIOS_MMAP_H

//
// Bios Memory Map
// ---------------
//
// This header contains the structure of an entry for the system
// address map returned by int 0x15, ax=0xE820
//

#include <kernel/macros.h>
#include <libk/stddef.h>

// Memory map physical address from the bootloader
#define BIOS_MMAP_NUM_ENTRIES_ADDR   ((void*)0x5000)
#define BIOS_MMAP_ENTRIES_ADDR       ((void*)0x5004)

// Address Range Descriptor Structure
typedef struct bios_mmap_entry {
  u32_t base_low;
  u32_t base_high;
  u32_t length_low;
  u32_t length_high;
  // Address type of this range
  #define BIOS_MMAP_TYPE_AVAILABLE          1
  #define BIOS_MMAP_TYPE_RESERVED           2
  #define BIOS_MMAP_TYPE_ACPI_RECLAIMABLE   3
  #define BIOS_MMAP_TYPE_MEMORY_NVS         4
  #define BIOS_MMAP_TYPE_MEMORY_BADRAM      5
  u32_t type;
  // Extended Attributes for Address Range Descriptor
  // Bit 0 must be 1, if bit 3 is set the address range
  // descriptor represents memory used for logging hardware
  // errors. The rest is reserved.
  u32_t acpi;
} _packed bios_mmap_entry_t;

#endif // POVOS_MM_BIOS_MMAP_H
