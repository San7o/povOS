// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_PMMGR_H
#define POVOS_MM_PMMGR_H

//
// Physical Memory Manager
// -----------------------
//
// Responsible for keeping track of which parts of the available
// hardware memory (usually ram) are free/in-use. It allocates fixed
// size blocks (PAGE_SIZE).
//

#include <mm/bios_mmap.h>
#include <libk/stddef.h>

typedef u64_t phys_addr_t;

typedef struct pmmgr {
  // Pointer to physical memory
  u8_t *bitfield;
  u64_t size;
} pmmgr_t;

// The global physical memory manager
// Needs to be initialized before using it
extern pmmgr_t pmmgr;

// Returns a negative number in case of failure
int pmmgr_init(void);

// Allocates a free page of physical memory, returns the phisical address
//
// Time: O(bitfield size)
//
// Returns 0 if no page was allocated
phys_addr_t pmmgr_alloc_page(void);
void pmmgr_free_page(phys_addr_t page);

#endif // POVOS_MM_PMMGR_H
