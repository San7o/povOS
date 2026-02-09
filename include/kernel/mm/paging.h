// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_PAGING_H
#define POVOS_KERNEL_MM_PAGING_H

//
// Paging
// ------
//

#include <libk/stddef.h>

typedef enum paging_entry_flags {
  PAGING_ENTRY_FLAG_NONE   = 0,
  PAGING_ENTRY_FLAG_WRITE  = (1 << 0),
  PAGING_ENTRY_FLAG_EXEC   = (1 << 1),
  PAGING_ENTRY_FLAG_USER   = (1 << 2),
} paging_entry_flags_t;

// This is a single page entry
typedef struct __attribute__((packed)) {
  u64_t present    : 1;  // P, must be 0
  u64_t rw         : 1;  // R/W, if 0 writes may not be allowed
  u64_t user       : 1;  // U/S, if 0 user-mode accesses are not allowed
  u64_t pwt        : 1;  // Page-level write-through
  u64_t pcd        : 1;  // Page-level cache disable
  u64_t accessed   : 1;  // A, indicated wheter this entry has been used
  u64_t dirty      : 1;  // D (Ignored in PML4)
  u64_t ps         : 1;  // Page size (0 for 4KB, must be 0 in PML4)
  u64_t global     : 1;  // G (Ignored in PML4)
  u64_t ignored    : 3;  // Custom OS bits
  u64_t address    : 40; // Physical address (shifted right 12 bits)
  u64_t available  : 11; // More custom bits
  u64_t nx         : 1;  // XD, No Execute
} paging_entry_t;

#define PAGE_SIZE   4096

typedef struct __attribute__((aligned(PAGE_SIZE))) paging_table {
  paging_entry_t   entries[1 << 9];
} paging_table_t;

// 4-level -aging
typedef struct paging_tables {
  paging_table_t   pml4_table;
  paging_table_t   pdpt_table;
  paging_table_t   pd_table;
  paging_table_t   pt_table;
} paging_tables_t;

// Link up the paging tables but does not load them in cr3
void paging_setup(paging_tables_t *tables);
// Load the paging tables into cr3
void paging_load(paging_tables_t  *tables);

void paging_add_entry(paging_tables_t      *tables,
                      void                 *phys_addr,
                      void                 *virt_addr,
                      paging_entry_flags_t  flags);

#endif // POVOS_KERNEL_MM_PAGING_H
