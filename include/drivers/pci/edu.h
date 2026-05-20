// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_EDU_H
#define POVOS_DRIVERS_EDU_H

//
// EDU Device
// ==========
//
// The EDU device is a fictional device implemented by Qemu for
// educational purposes. It can me used as an exercise for
// implementing a driver in your operating system or in the emulator
// itself.
//
// It is a PCIe device with id 0x11E8 vendor 0x1234. It implements
// several functionalities in the MMIO area, DMA and IRQ.
//
//       https://www.qemu.org/docs/master/specs/edu.html
//
// Only the MMIO is currently implemented.
//

#include <drivers/pci/pcie.h>
#include <libk/stdbool.h>
#include <libk/stddef.h>

// Use edu_init to initialize this structure
struct edu_device {
  struct pcie_type0_config_space_header* header;
  volatile u32_t* mmio;
};

bool edu_init(struct edu_device *edu, struct pcie_acpi_sdt *pcie_sdt);
bool edu_check_liveness(struct edu_device *edu);

u32_t edu_read_identification(struct edu_device *edu);

void edu_int_raise(struct edu_device *edu);
void edu_int_ack(struct edu_device *edu);  // should be called in the interrpt

#endif // POVOS_DRIVERS_EDU_H
