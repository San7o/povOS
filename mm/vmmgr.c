// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <mm/vmmgr.h>   // implements
#include <mm/pmmgr.h>
#include <mm/paging.h>
#include <libk/string.h>

#define PAGE_ALIGN_UP(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

void vmmgr_setup(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  vmmgr->pml4t   = paging_pml4t_init();
  // We start at 0x400000 since the memory before this is already
  // identity mapped my default in the physical memory manager
  free_list_alloc_init(&vmmgr->vas_allocator,
                       (void*)0x400000, 0xFFFFFFFFFFFFFFFF);
  return;
}

void vmmgr_activate(vmmgr_t *vmmgr)
{
  if (!vmmgr) return;
  paging_load(vmmgr->pml4t);
  return;
}

virt_addr_t vmm_alloc(vmmgr_t *vmmgr,
                      size_t length,
                      vmmgr_flags_t flags)
{
  if (!vmmgr) return 0;

  // Align the length to the nearest page
  length = PAGE_ALIGN_UP(length);
  unsigned int pages = length / PAGE_SIZE;
  virt_addr_t  addr = (virt_addr_t)free_list_alloc_malloc(&vmmgr->vas_allocator,
                                                          length);
  page_entry_flags_t pflags = {0};
  if (flags & VMMGR_FLAG_WRITE)
    pflags.rw = 1;
  if (flags & VMMGR_FLAG_EXEC)
    pflags.nx = 0;
  if (flags & VMMGR_FLAG_USER)
    pflags.user = 1;
  
  for (unsigned int i = 0; i < pages; ++i)
  {
    phys_addr_t paddr = pmmgr_alloc_page();
    virt_addr_t vaddr = addr + i * PAGE_SIZE;
    paging_add_entry((void*) paddr,
                     (void*) vaddr,
                     pflags);
  }

  for (int i = 0; i < VMMGR_MAX_OBJECTS; ++i)
  {
    if (!vmmgr->objects[i].mapped)
    {
      vmmgr->objects[i] = (vmmgr_obj_t) {
        .base   = addr,
        .length = length,
        .mapped = true,
        .flags  = flags,
      };
      break;
    }
 }
  
  return addr;
}
