// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_VMMGR_H
#define POVOS_KERNEL_MM_VMMGR_H

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
#include <kernel/mm/paging.h>

typedef u64_t virt_addr_t;

typedef enum vmmgr_object_flags {
  VMMGR_OBJ_FLAG_NONE    = 0,
  VMMGR_OBJ_FLAG_WRITE   = (1 << 0),
  VMMGR_OBJ_FLAG_EXEC    = (1 << 1),
  VMMGR_OBJ_FLAG_USER    = (1 << 2),
} vmmgr_obj_flags_t;

typedef struct vmmgr_obj {
  void*    base;
  size_t   length;
  size_t   flags;
  bool     mapped;
} vmmgr_obj_t;

typedef struct vmmgr {
  page_table_t  *pml4t;
  vmmgr_obj_t   *objects;
} vmmgr_t;

typedef void* vaddr_t;

//
// Functions
//

void vmmgr_setup(vmmgr_t *vmmgr);
void vmmgr_activate(vmmgr_t *vmmgr);

vaddr_t* vmm_alloc(vmmgr_t *vmmgr,
                   size_t length,
                   vmmgr_obj_flags_t flags);

// Invalidate TLB for this virtual address
void vmmgr_invalidate_tlb(virt_addr_t addr);

#endif // POVOS_KERNEL_MM_VMMGR_H
