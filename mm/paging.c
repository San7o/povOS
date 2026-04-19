// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <mm/paging.h>   // implements
#include <mm/pmmgr.h>
#include <mm/vmmgr.h>
#include <mm/layout.h>
#include <libk/string.h>

page_table_t *paging_pml4t_init(void)
{
  page_table_t* pml4t = (void*) pmmgr_alloc_page();
  memset(pml4t, 0, PAGE_SIZE);

  // Kernel high-half map
  //
  // Maps 1GB of physical 0x0 to virtual KERNEL_BASE_ADDR
  
  page_table_t *pd_kernel = (void*) pmmgr_alloc_page();
  memset(pd_kernel, 0, PAGE_SIZE);  // Zero the whole table
  
  for (int i = 0; i < 512; ++i)
  {
    pd_kernel->entries[i].address = (i * HUGE_PAGE_SIZE) >> 12; 
    pd_kernel->entries[i].present = 1;
    pd_kernel->entries[i].rw = 1;
    pd_kernel->entries[i].ps = 1;   // huge page
  }

  page_table_t *pdpt_kernel = (void*) pmmgr_alloc_page();
  memset(pdpt_kernel, 0, PAGE_SIZE);
  
  pdpt_kernel->entries[PDPT_INDEX(KERNEL_BASE_ADDR)].address = ((u64_t)pd_kernel) >> 12;
  pdpt_kernel->entries[PDPT_INDEX(KERNEL_BASE_ADDR)].present = 1;
  pdpt_kernel->entries[PDPT_INDEX(KERNEL_BASE_ADDR)].rw = 1;

  pml4t->entries[PML4T_INDEX(KERNEL_BASE_ADDR)].address = ((u64_t)pdpt_kernel) >> 12;
  pml4t->entries[PML4T_INDEX(KERNEL_BASE_ADDR)].present = 1;
  pml4t->entries[PML4T_INDEX(KERNEL_BASE_ADDR)].rw = 1;

  // HHDM Map
  //
  // Map 4GM physical 0x0 to MM_HHDM_OFFSET
  
  // Map 4 GB of physical ram to MM_HHDM_OFFSET
  page_table_t *pdpt_hhdm = (void*) pmmgr_alloc_page();
  memset(pdpt_hhdm, 0, PAGE_SIZE);
  
  pml4t->entries[PML4T_INDEX(MM_HHDM_OFFSET)].address = (u64_t)pdpt_hhdm >> 12;
  pml4t->entries[PML4T_INDEX(MM_HHDM_OFFSET)].present = 1;
  pml4t->entries[PML4T_INDEX(MM_HHDM_OFFSET)].rw = 1;
  for (u64_t gb = 0; gb < 4; gb++)
  {
    page_table_t* pd = (void*) pmmgr_alloc_page();
    memset(pd, 0, PAGE_SIZE);

    pdpt_hhdm->entries[gb].address = (u64_t)pd >> 12;
    pdpt_hhdm->entries[gb].present = 1;
    pdpt_hhdm->entries[gb].rw = 1;

    for (u64_t entry = 0; entry < 512; entry++)
    {
      u64_t phys_addr = (gb << 30) + (entry << 21);
      pd->entries[entry].address = phys_addr >> 12;
      pd->entries[entry].present = 1;
      pd->entries[entry].rw = 1;
      pd->entries[entry].ps = 1; // huge page
    }
  }

  // Setup recursive trick mapping
  
  pml4t->entries[RECURSIVE_SLOT].address = (phys_addr_t)pml4t >> 12;
  pml4t->entries[RECURSIVE_SLOT].present = 1;
  pml4t->entries[RECURSIVE_SLOT].rw = 1;
  
  return pml4t;
}

void paging_add_entry(void               *phys_addr,
                      void               *virt_addr,
                      page_entry_flags_t  flags)
{
  u64_t virt = (u64_t)virt_addr;

  u64_t pml4_idx = (virt >> 39) & 0x1FF;
  u64_t pdpt_idx = (virt >> 30) & 0x1FF;
  u64_t pd_idx   = (virt >> 21) & 0x1FF;
  u64_t pt_idx   = (virt >> 12) & 0x1FF;

  page_table_t* pml4t = (page_table_t*)GET_PML4T_VADDR();
  page_table_t* pdpt  = (page_table_t*)GET_PDPT_VADDR(virt);
  page_table_t* pd    = (page_table_t*)GET_PD_VADDR(virt);
  page_table_t* pt    = (page_table_t*)GET_PT_VADDR(virt);
  
  page_entry_t *pml4e = &pml4t->entries[pml4_idx];
  if (!pml4e->present)
  {
    phys_addr_t new_pdpt = pmmgr_alloc_page();
    pml4e->address = new_pdpt >> 12;
    pml4e->present = 1;
    pml4e->rw      = 1;
    
    memset(MM_PHYS_TO_VIRT(new_pdpt), 0, PAGE_SIZE);
  }

  page_entry_t *pdpte = &pdpt->entries[pdpt_idx];
  if (!pdpte->present)
  {
    phys_addr_t new_pd = pmmgr_alloc_page();
    pdpte->address = new_pd >> 12;
    pdpte->present = 1;
    pdpte->rw      = 1;

    memset(MM_PHYS_TO_VIRT(new_pd), 0, PAGE_SIZE);
  }

  page_entry_t *pde = &pd->entries[pd_idx];
  if (!pde->present)
  {
    phys_addr_t new_pt = pmmgr_alloc_page();
    pde->address = new_pt >> 12;
    pde->present = 1;
    pde->rw      = 1;

    memset(MM_PHYS_TO_VIRT(new_pt), 0, PAGE_SIZE);
  }

  page_entry_t *pte = &pt->entries[pt_idx];
  pte->address  = ((u64_t)phys_addr) >> 12;
  pte->present  = 1;
  pte->rw       = flags.rw;
  pte->user     = flags.user;
  pte->nx       = flags.nx;
  pte->pwt      = flags.pwt;
  pte->pcd      = flags.pcd;

  vmm_invalidate_tlb(virt);
}
