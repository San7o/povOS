// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/acpi.h>   // implements
#include <kernel/range.h>
#include <kernel/mm/pmmgr.h>
#include <kernel/mm/paging.h>
#include <kernel/mm/layout.h>
#include <libk/string.h>
#include <libk/stdio.h>
#include <libk/stddef.h>

// The ACPI RSDP is identified by a "RSD PTR " string (notice
// the space at the end) and can be confirmed via a checksum.
static size_t acpi_locate_rsdp_range(range_t range)
{
  const char *id = ACPI_RSDP_SIGNATURE;
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
      
      return (size_t)addr;
    }

  next:
    addr++;
  }
  
  return 0;
}

// We don't know where the pointer is, so we need to check possible
// memory locations for the identifier.
acpi_rsdp_t* acpi_locate_rsdp(void)
{
  u32_t *mmap_num_entries = BIOS_MMAP_NUM_ENTRIES_ADDR;
  bios_mmap_entry_t *mmap = BIOS_MMAP_ENTRIES_ADDR;
  if (!mmap_num_entries || !mmap) return NULL;
  
  range_t range;
  
  // First, check EBDA (Extended BIOS Data Area), 0x00080000 - 0x0009FFFF
  u16_t* ebda_seg = (void*)0x40E;
  size_t ebda_physical_addr = ((size_t)(*ebda_seg)) << 4;
  printk("[acpi] EBDA located at: %x\n", ebda_physical_addr);

  range = (range_t){
    .start = ebda_physical_addr,
    .end   = ebda_physical_addr + 1024,
  };
  size_t addr = acpi_locate_rsdp_range(range);
  if (addr != 0)
    return (acpi_rsdp_t*) addr;
  
  // Second, check BIOS read-only memory space between 0x000E0000 and
  // 0x000FFFFF
  range = (range_t){
    .start = 0x000E0000,
    .end   = 0x000FFFFF,
  };
  addr = acpi_locate_rsdp_range(range);
  if (addr != 0)
    return (acpi_rsdp_t*) addr;

  
  // For last, check the memory map
  // Note that this code assumes you can read the memory, meaning that
  // it is identity mapped, otherwise it will cause a CPU triple fault.
  for (u32_t i = 0; i < *mmap_num_entries; ++i)
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
      return (acpi_rsdp_t*) addr;
  }
  
  
  return NULL;
}

void* acpi_locate_sdt(acpi_rsdp_t* rsdp, const char signature[4])
{
  if (!rsdp) return NULL;

  page_entry_flags_t page_flags = {
    .present = 1,
    .rw = 1,
  };
  
  if (rsdp->revision == ACPI_VERSION_1)
  {
    // ACPI 1.0
    phys_addr_t rsdt_phys = (phys_addr_t)rsdp->rsdt_address;
    if (rsdt_phys == 0) return NULL;

    // Map a virtual address to rsdt_phys
    acpi_rsdt_t* rsdt_virt = MM_PHYS_TO_VIRT(rsdt_phys);
    paging_add_entry((void*)rsdt_phys, rsdt_virt, page_flags);
    
    if (strncmp((char*)rsdt_virt->header.signature,
                ACPI_RSDT_SIGNATURE,
                ACPI_SDT_SIGNATURE_SIZE)
        != 0) return NULL;

    // Iterate over the entires, looking for [signature]
    unsigned int entry_len =
      (rsdt_virt->header.length - sizeof(acpi_sdt_header_t)) / 4;
    for (unsigned int i = 0; i < entry_len; ++i)
    {
      phys_addr_t entry_phys = rsdt_virt->entries[i];
      if (entry_phys == 0) continue;

      // Map a virtual address to rsdt_phys
      acpi_sdt_header_t* entry_virt = MM_PHYS_TO_VIRT(entry_phys);
      paging_add_entry((void*)entry_phys, entry_virt, page_flags);
      
      if (strncmp((char*)entry_virt->signature, signature,
                  ACPI_SDT_SIGNATURE_SIZE) == 0)
        return (void*) entry_virt;
      
    }
    return NULL;
  }
  else
  {
    // ACPI 2.0
    phys_addr_t xsdt_phys = (phys_addr_t)rsdp->xsdt_address;
    if (xsdt_phys == 0) return NULL;

    // Map xsdt_virt to xsdt_phys
    acpi_xsdt_t* xsdt_virt = MM_PHYS_TO_VIRT(xsdt_phys);
    paging_add_entry((void*)xsdt_phys, xsdt_virt, page_flags);
    
    if (strncmp((char*)xsdt_virt->header.signature,
                ACPI_XSDT_SIGNATURE,
                ACPI_SDT_SIGNATURE_SIZE)
        != 0) return NULL;

    // Iterate over the entires, looking for [signature]
    unsigned int entry_len =
      (xsdt_virt->header.length - sizeof(acpi_sdt_header_t)) / 4;
    for (unsigned int i = 0; i < entry_len; ++i)
    {
      acpi_sdt_header_t *entry_phys = (void*)xsdt_virt->entries[i];
      if (entry_phys == 0) return NULL;

      // Map a virtual address to rsdt_phys
      acpi_sdt_header_t* entry_virt = MM_PHYS_TO_VIRT(entry_phys);
      paging_add_entry((void*)entry_phys, entry_virt, page_flags);
      
      if (strncmp((char*)entry_virt->signature, signature,
                 ACPI_SDT_SIGNATURE_SIZE) == 0)
        return (void*) entry_virt;
    }
    return NULL;
  }
  return NULL;
}
