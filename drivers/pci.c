// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/pci.h>   // implements
#include <bits/port.h>

void pci_writew(u32_t value)
{
  port_outdw(PCI_CONFIG_ADDRESS, value);
}

u16_t pci_readw(u8_t bus, u8_t slot, u8_t func, u8_t offset)
{
  u32_t lbus  = (u32_t) bus;
  u32_t lslot = (u32_t) slot;
  u32_t lfunc = (u32_t) func;
  u16_t tmp = 0;

  u32_t address = (u32_t)((lbus << 16) | (lslot << 11) |
                          (lfunc << 8) | (offset & 0xFC) | ((u32_t)0x80000000));

  pci_writew(address);

  tmp = (u16_t)((port_indw(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
  return tmp;
}

pci_device_vendor_t pci_get_device_vendor(u8_t bus, u8_t slot)
{
  pci_device_vendor_t vd = {0};

  // Register 0, offset 0 contains device id and vendor id
  if ((vd.vendor_id = pci_readw(bus, slot, 0, 0)) != PCI_DEVICE_VENDOR_NONE)
    vd.device_id = pci_readw(bus, slot, 0, 2);

  return vd;
}
