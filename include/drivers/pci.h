// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_PCI_H
#define POVOS_DRIVERS_PCI_H

#include <libk/stddef.h>

//
// PCI
// ===
//
// Peripheral Component Interconnect is a local computer bus for
// attaching hardware devices in a computer. PCI slots are the big
// long slots you can find in a regular motherboard. PCI is now
// replaced by PCIe (PCI Express) which uses memory mapped IO instead
// of ports, however the new standard is still compatible with the old
// PCI so we can still use it.
//
// PCI devices are little-endian (don't forget this or it will hunt
// you).
//
// There is no standard way to enumerate PCI devices, the best way is
// to check ACPI tables. Each PCI device has its own registers we can
// read from or write to, but usually all devices support at least
// some way to get the vendor + device id (register 0x0) and the
// status (register 0x1).
//

// PCI CONFIG ADDRESS
//
//  31         Enable bit
//  30 - 24    Reserved
//  23 - 16    Bus Number
//  15 - 11    Device Number
//  10 - 8     Function Number
//  7 - 0      Register Offset
//
#define PCI_CONFIG_ADDRESS    0xCF8
#define PCI_CONFIG_DATA       0xCFC

#define PCI_DEVICE_VENDOR_NONE 0xFFFF
typedef struct pci_device_vendor {
  u16_t vendor_id;
  u16_t device_id;
  
  char* vendor_name;
  char* device_name;
} pci_device_vendor_t;

void  pci_writew(u32_t value);
u16_t pci_readw(u8_t bus, u8_t slot, u8_t func, u8_t offset);

pci_device_vendor_t pci_get_device_vendor(u8_t bus, u8_t slot, u8_t func);

#endif // POVOS_DRIVERS_PCI_H
