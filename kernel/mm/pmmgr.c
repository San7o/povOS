// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/pmmgr.h>   // implements
#include <kernel/mm/paging.h>
#include <kernel/utils.h>

typedef struct range {
  size_t start;
  size_t end;
} range_t;

// Memory will not be allocated in these intervals "[a; b)"
range_t reserved_ranges[] = {
  { 0x0, 0x1e8480 }, // First 2MB (bios, kernel and some imporant
                     // structures)
};

int pmmgr_init(pmmgr_t *pmmgr, bios_mmap_entry_t *mmap,
               u32_t num_entries)
{
  if (!pmmgr || !mmap) return -1;

  // Get minimum and maximum available address
  size_t max_addr = 0;
  size_t min_addr = (size_t) (void*) -1;
  for (u32_t i = 0; i < num_entries; ++i)
  {
    u64_t base =
      (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
    u64_t length =
      (u64_t)mmap[i].length_low | ((u64_t) mmap[i].base_high << 32);

    if (mmap[i].type != BIOS_MMAP_TYPE_AVAILABLE) continue;

    min_addr = MIN(min_addr, base);
    max_addr = MAX(max_addr, base + length - 1);
  }

  // Calculate size of bitfield in available region
  u64_t available_region = max_addr - min_addr;
  u64_t available_pages  = available_region / PAGE_SIZE;
  u64_t bitfield_len_bytes = available_pages / 8;

  // Look for memory for the bitfield
  u64_t bitfield_pages = bitfield_len_bytes * 8 / PAGE_SIZE;
  if (bitfield_pages % PAGE_SIZE != 0) bitfield_pages++;
  size_t bitfield_addr = min_addr;
  const size_t reserved_ranges_size =
    sizeof(reserved_ranges) / sizeof(reserved_ranges[0]);
  // TODO
  (void) bitfield_addr;
  (void) reserved_ranges_size;
  
  // Set the available bits based on the mmap
  // TODO

  // Patch special locations (bitfield, bios + kernel)
  // TODO

  return 0;
}
