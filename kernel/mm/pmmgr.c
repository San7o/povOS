// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/pmmgr.h>   // implements
#include <kernel/mm/paging.h>
#include <kernel/utils.h>

#include <libk/stdbool.h>

#include <drivers/uart.h>  // used for debug prints

typedef struct range {
  size_t start;
  size_t end;
} range_t;

// Memory will not be allocated in these intervals "[a; b)"
// Note: ranges must not overlap!
range_t reserved_ranges[] = {
  { 0x0, 0x200000 }, // First 2MB (bios, kernel and some imporant
                     // structures)
};

// Gives you the next available range inside [range_bounds]
//
// This algorithm is really tricky to get right
static range_t get_next_avail_range(range_t range_bounds,
                                    bios_mmap_entry_t *mmap,
                                    u32_t mmap_num_entries)
{
  range_t next_range = {
    .start = range_bounds.end,
    .end   = range_bounds.end,
  };

  // Find next_range.start, which is the first available range start
  // after range_bounds.start

  bool found = false;
  while (!found)
  {

    // Check if start is valid

    bool start_valid = false;
    for (u32_t i = 0; i < mmap_num_entries; ++i)
    {
      // We only check available memory regions
      if (mmap[i].type != BIOS_MMAP_TYPE_AVAILABLE)
        continue;
      
      u64_t base =
        (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
      u64_t length =
        (u64_t)mmap[i].length_low | ((u64_t) mmap[i].length_high << 32);
      u64_t end = base + length;

      if (next_range.start >= base && next_range.end < end
          && next_range.start < range_bounds.end
          && next_range.start >= range_bounds.start)
        start_valid = true;
    }

    if (!start_valid)
    {
      // Find a new start
      for (u32_t i = 0; i < mmap_num_entries; ++i)
      {
        // We only check available memory regions
        if (mmap[i].type != BIOS_MMAP_TYPE_AVAILABLE)
          continue;
        
        u64_t start =
          (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
      
        if (start < next_range.start && start >= range_bounds.start)
          next_range.start = start;
      }
    }

    // Check if start is reserved
  
    bool is_reserved = false;
    for (size_t i = 0; i < sizeof reserved_ranges / sizeof reserved_ranges[0]; ++i)
    {
      if (next_range.start >= reserved_ranges[i].start
          && next_range.start < reserved_ranges[i].end)
      {
        // next_range.start is inside a reserved range, so we cut the
        // available range_bounds to the current start
        range_bounds.start = next_range.start + 1;
        next_range.start   = reserved_ranges[i].end;

        is_reserved = true;
        
        // We break, assuming there are not overlaps in reserved_ranges,
        // so we don't need to check the other reserved ranges
        break;
      }
    }

    if (!is_reserved)
      found = true;
  }

  // We have found start, now find the closest range end after
  // next_range.start inside range_bounds

  for (u32_t i = 0; i < mmap_num_entries; ++i)
  {
    // We only check available memory regions
    if (mmap[i].type != BIOS_MMAP_TYPE_AVAILABLE)
      continue;
        
    u64_t base =
      (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
    u64_t length =
      (u64_t)mmap[i].length_low | ((u64_t) mmap[i].length_high << 32);
    u64_t end = base + length;

    if (end < range_bounds.end
        && end < next_range.end && end > next_range.start)
      next_range.end = end;
  }

  // Make sure that next_range.end is available
  for (size_t i = 0; i < sizeof reserved_ranges / sizeof reserved_ranges[0]; ++i)
  {
    if (next_range.end >= reserved_ranges[i].start
        && next_range.end < reserved_ranges[i].end)
    {
      // End is reserved, so we take reserved_ranges[i].start as
      // our end
      next_range.end = reserved_ranges[i].start;

      // We do not need to do any more checks since we are sure that
      // an available area exists between next_range.start and
      // next_range.end (because of how we choose next_range.start)
      break;
    }
  }
  
  return next_range;
}

// Here we want to manually allocate pmmgr->bitfield in the physical
// memory, then initialize it based on available pages.
int pmmgr_init(pmmgr_t *pmmgr, bios_mmap_entry_t *mmap,
               u32_t mmap_num_entries)
{
  if (!pmmgr || !mmap) return -1;

  // Get minimum and maximum available address
  size_t max_addr = 0;
  size_t min_addr = (size_t) (void*) -1;
  for (u32_t i = 0; i < mmap_num_entries; ++i)
  {
    if (mmap[i].type != BIOS_MMAP_TYPE_AVAILABLE)
      continue;

    u64_t base =
      (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
    u64_t length =
      (u64_t)mmap[i].length_low | ((u64_t) mmap[i].length_high << 32);

    min_addr = MIN(min_addr, base);
    max_addr = MAX(max_addr, base + length + 1); // max is not included
  }
  // Full range possible
  range_t bitfield_range = (range_t){ .start = min_addr, .end = max_addr};

  // Calculate size of bitfield
  u64_t available_region = bitfield_range.end - bitfield_range.start;
  u64_t available_pages  = available_region / PAGE_SIZE;
  u64_t bitfield_len_bytes = available_pages / 8;
  u64_t bitfield_pages = bitfield_len_bytes / PAGE_SIZE;
  if (bitfield_len_bytes % PAGE_SIZE != 0) bitfield_pages++;

  // Find a range in physical memory bit enough to store the bitfield
  
  range_t range_candidate;
  while (true)
  {
    range_candidate = get_next_avail_range(bitfield_range,
                                           mmap,
                                           mmap_num_entries);

    // Useful debug prints
    uart_write_str(UART_COM1, "[debug] [pmmgr] range_candidate.start = ");
    uart_write_hex(UART_COM1, range_candidate.start);
    uart_write_str(UART_COM1, ", range_candidate.end = ");
    uart_write_hex(UART_COM1, range_candidate.end);
    uart_write_str(UART_COM1, "\n");

    if (range_candidate.end - range_candidate.start >= bitfield_pages * PAGE_SIZE)
      // We have found a good candidate
      break;

    // Setuo for getting the next available range
    bitfield_range.start = range_candidate.start + 1;
  }

  pmmgr->bitfield = (u8_t*) range_candidate.start;
  pmmgr->size = bitfield_len_bytes;

  // Fill the bitfield

  for (u64_t i = 0; i < bitfield_pages * PAGE_SIZE; ++i)
  {
    pmmgr->bitfield[i] = 0;
    for (int bit = 0; bit < 8; ++bit)
    {
      bool available = true;
      range_t this_page = {
        .start = (i * 8 + bit) * PAGE_SIZE,
        .end   = (i * 8 + bit) * PAGE_SIZE + PAGE_SIZE,
      };

      // Check mmap
      for (u32_t i = 0; i < mmap_num_entries; ++i)
      {
        // We only check unavailable memory regions
        if (mmap[i].type == BIOS_MMAP_TYPE_AVAILABLE)
          continue;
            
        u64_t base =
          (u64_t)mmap[i].base_low | ((u64_t) mmap[i].base_high << 32);
        u64_t length =
          (u64_t)mmap[i].length_low | ((u64_t) mmap[i].length_high << 32);
        u64_t end = base + length;

        if (this_page.start < end && this_page.end >= base)
          available = false;
      }

      if (available)
      {
        // Check reserved
        for (size_t i = 0; i < sizeof reserved_ranges / sizeof reserved_ranges[0]; ++i)
        {
          if (this_page.start < reserved_ranges[i].end
              && this_page.end >= reserved_ranges[i].start)
            available = false;
        }
      }
      // Check bitfield
      if (available)
      {
        if (this_page.start < (u64_t)pmmgr->bitfield + bitfield_pages * PAGE_SIZE
            && this_page.end >= (u64_t)pmmgr->bitfield)
          available = false;
      }

      if (available)
        pmmgr->bitfield[i] |= (1 << bit);
    }
  }
  
  return 0;
}
