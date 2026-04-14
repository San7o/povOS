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
typedef struct edu_device {
  pcie_type0_config_space_header_t* header;
  volatile u32_t* mmio;
} edu_device_t;

bool edu_init(edu_device_t *edu, pcie_acpi_sdt_t *pcie_sdt);
bool edu_check_liveness(edu_device_t *edu);

u32_t edu_read_identification(edu_device_t *edu);

void edu_int_raise(edu_device_t *edu);
void edu_int_ack(edu_device_t *edu);  // should be called in the interrpt

#endif // POVOS_DRIVERS_EDU_H
