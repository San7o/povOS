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

#include <libk/stddef.h>
#include <kernel/mm/paging.h>

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
} vmmgr_obj_t;

typedef struct vmmgr {
  paging_tables_t   pt;
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

#endif // POVOS_KERNEL_MM_VMMGR_H
