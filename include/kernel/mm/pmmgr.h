// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_PMMGR_H
#define POVOS_KERNEL_MM_PMMGR_H

//
// Physical Memory Manager
// -----------------------
//
// Responsible for keeping track of which parts of the available
// hardware memory (usually ram) are free/in-use. It allocated fixed
// size blocks (PAGE_SIZE).
//

#include <kernel/mm/bios_mmap.h>
#include <libk/stddef.h>

typedef struct pmmgr {
  u64_t *bitfield;
  u64_t size;
} pmmgr_t;

int pmmgr_init(pmmgr_t *pmmgr, bios_mmap_entry_t *mmap,
               u32_t mmap_num_entries);

#endif // POVOS_KERNEL_MM_PMMGR_H
