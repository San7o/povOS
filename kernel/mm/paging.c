// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/paging.h>   // implements
#include <kernel/mm/pmmgr.h>
#include <kernel/mm/vmmgr.h>
#include <libk/string.h>

page_table_t *paging_pml4t_init(void)
{
  // Identity map the first 4 megabytes
  page_table_t *pt0 = (void*)pmmgr_alloc_page();
  for (int i = 0; i < 512; ++i)
  {
    pt0->entries[i].address = (i * PAGE_SIZE) >> 12; 
    pt0->entries[i].present = 1;
    pt0->entries[i].rw = 1;
  }

  page_table_t *pt1 = (void*)pmmgr_alloc_page();
  for (int i = 0; i < 512; ++i)
  {
    pt1->entries[i].address = (0x200000 + i * PAGE_SIZE) >> 12; 
    pt1->entries[i].present = 1;
    pt1->entries[i].rw = 1;
  }

  page_table_t *pd = (void*)pmmgr_alloc_page();
  pd->entries[0].address = ((u64_t)pt0) >> 12;
  pd->entries[0].present = 1;
  pd->entries[0].rw = 1;
  pd->entries[1].address = ((u64_t)pt1) >> 12;
  pd->entries[1].present = 1;
  pd->entries[1].rw = 1;

  page_table_t *pdpt = (void*)pmmgr_alloc_page();
  pdpt->entries[0].address = ((u64_t)pd) >> 12;
  pdpt->entries[0].present = 1;
  pdpt->entries[0].rw = 1;
    
  page_table_t* pml4t = (void*)pmmgr_alloc_page();
  pml4t->entries[0].address = ((u64_t)pdpt) >> 12;
  pml4t->entries[0].present = 1;
  pml4t->entries[0].rw = 1;

  return pml4t;
}

void page_add_entry(page_table_t       *pml4t,
                    void               *phys_addr,
                    void               *virt_addr,
                    page_entry_flags_t  flags)
{
  u64_t virt = (u64_t)virt_addr;

  // Extract indices for each level from the virtual address
  u64_t pml4_idx = (virt >> 39) & 0x1FF;
  u64_t pdpt_idx = (virt >> 30) & 0x1FF;
  u64_t pd_idx   = (virt >> 21) & 0x1FF;
  u64_t pt_idx   = (virt >> 12) & 0x1FF;

  // Level 1: PML4 -> PDPT
  page_entry_t *pml4e = &pml4t->entries[pml4_idx];
  if (!pml4e->present)
  {
    page_table_t *pdpt = (void*)pmmgr_alloc_page();
    memset(pdpt, 0, PAGE_SIZE);
    pml4e->address = ((u64_t)pdpt) >> 12;
    pml4e->present = 1;
    pml4e->rw      = 1;
  }
  page_table_t *pdpt = (page_table_t*)(pml4e->address << 12);

  // Level 2: PDPT -> PD
  page_entry_t *pdpte = &pdpt->entries[pdpt_idx];
  if (!pdpte->present)
  {
    page_table_t *pd = (void*)pmmgr_alloc_page();
    memset(pd, 0, PAGE_SIZE);
    pdpte->address = ((u64_t)pd) >> 12;
    pdpte->present = 1;
    pdpte->rw      = 1;
  }
  page_table_t *pd = (page_table_t*)(pdpte->address << 12);

  // Level 3: PD -> PT
  page_entry_t *pde = &pd->entries[pd_idx];
  if (!pde->present)
  {
    page_table_t *pt = (void*)pmmgr_alloc_page();
    memset(pt, 0, PAGE_SIZE);
    pde->address = ((u64_t)pt) >> 12;
    pde->present = 1;
    pde->rw      = 1;
  }
  page_table_t *pt = (page_table_t*)(pde->address << 12);

  // Level 4: PT -> page
  page_entry_t *pte = &pt->entries[pt_idx];
  pte->address  = ((u64_t)phys_addr) >> 12;
  pte->present  = flags.present;
  pte->rw       = flags.rw;
  pte->user     = flags.user;
  pte->nx       = flags.nx;
  pte->pwt      = flags.pwt;
  pte->pcd      = flags.pcd;

  vmm_invalidate_tlb(virt);
}
