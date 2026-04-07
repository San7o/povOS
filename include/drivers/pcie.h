// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_PCIE_H
#define POVOS_DRIVERS_PCIE_H

//
// PCI Express
// ===========
//
// This is the new PCI standard, it allows MMIO operations. Memory
// addresses can be found via ACPI.
//

#include <drivers/acpi.h>

#define PCIE_ACPI_SIGNATURE "MCFG"

typedef struct pcie_acpi_entry {
  u64_t   base_addr;
  u16_t   pci_segment_group;
  u8_t    start_bus;
  u8_t    end_bus;
  u32_t   reserved;
} _packed pcie_acpi_entry_t;

typedef struct pcie_acpi_sdt {
  acpi_sdt_header_t   header;
  u64_t               reserved;
  pcie_acpi_entry_t  *entries;
} pcie_acpi_sdt_t;

#define PCIE_PHYS_ADDR(base, bus, device, function) \
  (base + ((bus) << 20 | device << 15 | function << 12))

// TODO

#endif // POVOS_DRIVERS_PCIE_H
