// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_PAGING_H
#define POVOS_MM_PAGING_H

//
// Paging
// ------
//
// 64 bit paging (aka Physical Address Extension) has 4 levels of
// tables, each one containing 512 entries.
//
//   PML4T -> 0x1000 (Page Map Level 4 Table)
//   PDPT  -> 0x2000 (Page Directory Pointer Table)
//   PDT   -> 0x3000 (Page Directory Table)
//   PT    -> 0x4000 (Page table) Contains 512 entries, each mapping a
//                    4KB page
//
// The virtual address bit breakdown is:
//
//  63       48 47    39 38    30 29    21 20    12 11      0
//  [  unused  ] [pml4] [ pdpt ] [  pd  ] [  pt  ] [ offset ]
//                 9bit   9bit     9bit     9bit      12bit
//

#include <kernel/macros.h>
#include <libk/stddef.h>

typedef u64_t virt_ptr_t;

typedef struct page_entry_flags {
  u64_t rw         : 1;
  u64_t user       : 1;
  u64_t pwt        : 1;
  u64_t nx         : 1;
  u64_t pcd        : 1;
} page_entry_flags_t;

// This is a single page entry (uses 8 bytes)
typedef struct page_entry {
  u64_t present    : 1;  // 1 present, 0 not present
  u64_t rw         : 1;  // R/W, if 0 writes may not be allowed
  u64_t user       : 1;  // U/S, if 0 user-mode accesses are not allowed
  u64_t pwt        : 1;  // Page-level write-through
  u64_t pcd        : 1;  // Page-level cache disable
  u64_t accessed   : 1;  // A, indicated wheter this entry has been used
  u64_t dirty      : 1;  // D (Ignored in PML4)
  u64_t ps         : 1;  // Page size (0 for 4KB, 1 for 2MB) Must be 0 in PML4
  u64_t global     : 1;  // G (Ignored in PML4)
  u64_t ignored    : 3;  // Custom OS bits
  u64_t address    : 40; // Physical address (shifted right 12 bits)
  u64_t available  : 11; // More custom bits
  u64_t nx         : 1;  // XD, No Execute
} _packed page_entry_t;

#define PAGE_SIZE   4096

#define PAGE_ALIGN_UP(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

// A single level, takes up 4KB size
typedef struct __attribute__((aligned(PAGE_SIZE))) page_table {
  page_entry_t   entries[1 << 9];
} page_table_t;

//
// Recursive Trick
// ---------------
//
// TODO: we have some long long explaination to do here...
//

// Our index of choice
#define RECURSIVE_SLOT 510UL
// The base prefix for the recursive window (Sign-extended for 64-bit)
#define RECURSIVE_BASE (0xFFFF000000000000ULL | (RECURSIVE_SLOT << 39))
// Get the address of the PML4 itself
// By using the slot 4 times, the CPU lands on the PML4 as a data page
#define GET_PML4T_VADDR() \
  ((u64_t*)(RECURSIVE_BASE | (RECURSIVE_SLOT << 30) | (RECURSIVE_SLOT << 21) | (RECURSIVE_SLOT << 12)))
// Get the address of a specific PDPT
// (Requires the PML4 index of the target address)
#define GET_PDPT_VADDR(vaddr) \
  ((u64_t*)(RECURSIVE_BASE | (RECURSIVE_SLOT << 30) | (RECURSIVE_SLOT << 21) | (((u64_t)(vaddr) >> 27) & 0x00000000001FF000ULL)))
// Get the address of a specific Page Directory (PD)
#define GET_PD_VADDR(vaddr) \
  ((u64_t*)(RECURSIVE_BASE | (RECURSIVE_SLOT << 30) | (((u64_t)(vaddr) >> 18) & 0x000000003FFFF000ULL)))
// Get the address of a specific Page Table (PT)
#define GET_PT_VADDR(vaddr) \
  ((u64_t*)(RECURSIVE_BASE | (((u64_t)(vaddr) >> 9) & 0x0000007FFFFFF000ULL)))

// Allocate a new page map level 4 table
//
// Initialized it by identity mapping the first 4MB or memory. Does
// not activate the page table, use `paging_load` for that.
//
// Retruns a pointer to the PML4 table, or NULL if unsuccessful 
page_table_t *paging_pml4t_init(void);

// Load the PML4 table into cr3
// Implemented in assembly
void paging_load(page_table_t *pml4t);

void paging_add_entry(void               *phys_addr,
                      void               *virt_addr,
                      page_entry_flags_t  flags);

#endif // POVOS_MM_PAGING_H
