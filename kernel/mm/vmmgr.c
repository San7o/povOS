// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/vmmgr.h>   // implements
#include <libk/string.h>

void vmmgr_setup(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  memset(&vmmgr->pt, 0, sizeof(vmmgr->pt));
  paging_setup(&vmmgr->pt);
  return;
}

void vmmgr_activate(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  paging_load(&vmmgr->pt);
  return;
}
  
vaddr_t* vmm_alloc(vmmgr_t *vmmgr,
                   size_t length,
                   vmmgr_obj_flags_t flags)
{
  (void) flags;
  
  if (!vmmgr) return NULL;

  // Align the length to the nearest page
  length = ((length + PAGE_SIZE + 1) / PAGE_SIZE) * PAGE_SIZE;


  // TODO
  
  return NULL;
}
