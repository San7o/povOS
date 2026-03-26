// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/acpi.h>   // implements
#include <kernel/range.h>
#include <libk/string.h>
#include <libk/stdio.h>

// The ACPI RSDP is identified by a "RSD PTR " string (notice
// the space at the end) and can be confirmed via a checksum.
static size_t acpi_locate_rsdp_range(range_t range)
{
  const char *id = "RSD PTR ";
  unsigned int id_len = strlen(id);
  char* addr = (void*)range.start;

  while ((size_t) ((void*)addr) < range.end - sizeof(acpi_rsdp_t))
  {
    if (strncmp(id, addr, id_len) == 0)
    {
      // Check checksum ACPI 1.0
      unsigned int sum = 0;
      for (unsigned int i = 0; i < 20; ++i)
        sum += addr[i];
      if ((sum & 0xF) != 0)
        goto next;

      acpi_rsdp_t *rsdp = (void*)addr;
      if (rsdp->revision == ACPI_VERSION_2)
      {
        // Check checksum ACPI 2.0
        for (unsigned int i = 20; i < sizeof(acpi_rsdp_t); ++i)
          sum += addr[i];
        if ((sum & 0xF) != 0)
          goto next;
      }
      
      printk("[acpi] Found ACPI RSDP at %x\n", addr);
      return (size_t)addr;
    }

  next:
    addr++;
  }
  
  return 0;
}

// We don't know where the pointer is, so we need to check possible
// memory locations for the identifier.
size_t acpi_locate_rsdp(bios_mmap_entry_t *mmap, u32_t mmap_num_entries)
{
  range_t range;
  
  // First, check the memory map
  for (u32_t i = 0; i < mmap_num_entries; ++i)
  {
    if (mmap[i].type != BIOS_MMAP_TYPE_ACPI_RECLAIMABLE)
      continue;

    u64_t base =
      (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
    u64_t length =
      (u64_t)mmap[i].length_low | ((u64_t) mmap[i].length_high << 32);
    u64_t end = base + length;

    range = (range_t){
      .start = base,
      .end   = end,
    };

    size_t addr = acpi_locate_rsdp_range(range);
    if (addr != 0)
      return addr;
  }
  
  // Second, check EBDA (Extended BIOS Data Area), 0x00080000 - 0x0009FFFF
  u16_t* ebda_seg = (void*)0x40E;
  size_t ebda_physical_addr = ((size_t)(*ebda_seg)) << 4;
  printk("[acpi] EBDA located at: %x\n", ebda_physical_addr);

  range = (range_t){
    .start = ebda_physical_addr,
    .end   = ebda_physical_addr + 1024,
  };
  size_t addr = acpi_locate_rsdp_range(range);
  if (addr != 0)
    return addr;
  
  // Third, check BIOS read-only memory space between 0x000E0000 and
  // 0x000FFFFF
  range = (range_t){
    .start = 0x000E0000,
    .end   = 0x000FFFFF,
  };
  addr = acpi_locate_rsdp_range(range);
  if (addr != 0)
    return addr;
  
  return 0;
}
