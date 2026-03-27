// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/vmmgr.h>   // implements
#include <libk/string.h>

#define PAGE_ALIGN_UP(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

void vmmgr_setup(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  vmmgr->pml4t   = paging_pml4t_init();
  vmmgr->objects = NULL;
  return;
}

void vmmgr_activate(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  paging_load(vmmgr->pml4t);
  return;
}
  
vaddr_t* vmm_alloc(vmmgr_t *vmmgr,
                   size_t length,
                   vmmgr_obj_flags_t flags)
{
  (void) flags;
  
  if (!vmmgr) return NULL;

  // Align the length to the nearest page
  length = PAGE_ALIGN_UP(length);

  // TODO
  
  return NULL;
}
