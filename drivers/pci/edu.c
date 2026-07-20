// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/pci/edu.h>   // implements
#include <libk/unistd.h>
#include <mm/paging.h>
#include <mm/pmmgr.h>

#define EDU_ALLOC      0
#define EDU_BUS        0
#define EDU_DEVICE     3
#define EDU_FUNCTION   0

bool edu_init(struct edu_device *edu, struct pcie_acpi_sdt *pcie_sdt)
{
  if (!edu || !pcie_sdt)
    return false;

  struct pcie_acpi_entry *pcie_entry = &pcie_sdt->entries[EDU_ALLOC];
  phys_addr_t phys_addr = PCIE_PHYS_ADDR(pcie_entry->base_addr,
                                         EDU_BUS, EDU_DEVICE, EDU_FUNCTION);
  // Identity map memory
  void* virt_addr = (void*)phys_addr;
  struct page_entry_flags page_flags = { .rw = 1, .pcd = 1 };
  paging_add_entry((void*)phys_addr, virt_addr, page_flags);

  struct pcie_common_config_space_header *pcie_hdr = virt_addr;
  u8_t layout = pcie_hdr->header_type & PCIE_CONFIG_SPACE_HEADER_TYPE_MASK;
  if (layout != PCIE_CONFIG_SPACE_HEADER_TYPE0)
    return false;

  edu->header = (struct pcie_type0_config_space_header*)virt_addr;

  u32_t bar0_raw = edu->header->base_address_regs[0];
  phys_addr_t edu_mmio_phys = bar0_raw & 0xFFFFFFF0;

  edu->mmio = (void*)edu_mmio_phys;
  paging_add_entry((void*)edu_mmio_phys, (void*)edu->mmio, page_flags);

  // Enable interrupts
  edu->header->command &= ~PCIE_CONFIG_SPACE_CMD_INT_DISABLE;

  return edu_check_liveness(edu);
}

bool edu_check_liveness(struct edu_device *edu)
{
  if (!edu)
    return false;

  volatile u32_t test_val = 0x123;

  edu->mmio[1] = test_val;
  sleep_ticks(200);
  return edu->mmio[1] == ~test_val;
}

u32_t edu_read_identification(struct edu_device *edu)
{
  if (!edu)
    return 0;
  return edu->mmio[0];
}

void edu_int_raise(struct edu_device *edu)
{
  if (!edu)
    return;
  edu->mmio[24] = 5;
}

void edu_int_ack(struct edu_device *edu)
{
  if (!edu)
    return;
  edu->mmio[25] = 5;
}
