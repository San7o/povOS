// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_VMMGR_H
#define POVOS_MM_VMMGR_H

//
// Virtual Memory Manager
// ----------------------
//
// The virtual memory manager sits between the physical allocator and
// anything that needs virtual memory. It finds a free region in
// virtual address space, then allocates physical frames from the
// Physical Memory Manager and maps it via paging.
//

#include <libk/stddef.h>
#include <libk/stdbool.h>
#include <mm/paging.h>
#include <mm/free_list_allocator.h>

#define VMMGR_MAX_OBJECTS 1024

typedef u64_t virt_addr_t;

#define VMMGR_FLAG_NONE    0
#define VMMGR_FLAG_WRITE   (1 << 0)
#define VMMGR_FLAG_EXEC    (1 << 1)
#define VMMGR_FLAG_USER    (1 << 2)
typedef u64_t vmmgr_flags_t;

struct vmmgr_obj {
  virt_addr_t    base;
  size_t         length;
  bool           mapped;
  vmmgr_flags_t  flags;
};

struct vmmgr {
  // Virtual address space allocator
  struct free_list_alloc  vas_allocator;
  struct page_table  *pml4t;
  struct vmmgr_obj  objects[VMMGR_MAX_OBJECTS];
};

//
// Functions
//

void vmmgr_setup(struct vmmgr *vmmgr);
void vmmgr_activate(struct vmmgr *vmmgr);

// Allocates multiples of PAGE_SIZE of virtual and physical memory,
// sets up the translation between them
virt_addr_t vmm_alloc(struct vmmgr *vmmgr,
                      size_t length_bytes,
                      vmmgr_flags_t flags);

// Invalidate TLB for this virtual address
//
// Implemented in assembly
void vmm_invalidate_tlb(virt_addr_t addr);

#endif // POVOS_MM_VMMGR_H
