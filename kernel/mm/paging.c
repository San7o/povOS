// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/paging.h>   // implements

void paging_tables_init(paging_tables_t *tables)
{
  if (!tables) return;
  
  // Link PML4 to PDPT
  // We shift the physical address right by 12 because the struct 
  // bit-field for 'address' is 40 bits (covering bits 12-51).
  tables->pml4_table.entries[0].address = ((u64_t)&tables->pdpt_table) >> 12;
  tables->pml4_table.entries[0].present = 1;
  tables->pml4_table.entries[0].rw = 1;

  // Link PDPT to PD
  tables->pdpt_table.entries[0].address = ((u64_t)&tables->pd_table) >> 12;
  tables->pdpt_table.entries[0].present = 1;
  tables->pdpt_table.entries[0].rw = 1;

  // Link PD to PT
  tables->pd_table.entries[0].address = ((u64_t)&tables->pt_table) >> 12;
  tables->pd_table.entries[0].present = 1;
  tables->pd_table.entries[0].rw = 1;

  // Map the first 4KB page to physical address 0x0
  tables->pt_table.entries[0].address = 0 >> 12; 
  tables->pt_table.entries[0].present = 1;
  tables->pt_table.entries[0].rw = 1;
    
  return;
}

void paging_tables_load(paging_tables_t *tables)
{
  if (!tables) return;

  (void) tables;

  // TODO: figure out how to always get the physical address of
  //       the pointer to tables
  
  return;
}
