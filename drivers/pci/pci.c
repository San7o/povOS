// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/pci/pci.h>   // implements
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

// Get more names from: https://pci-ids.ucw.cz/
static pci_device_vendor_t pci_known_names[] = {
  { 0x8086, 0x0,    "Intel Corporation", "82G33/G31/P35/P31 Express DRAM Controller" },
  { 0x8086, 0x10d3, "Intel Corporation", "82574L Gigabit Network Connection" },
  { 0x8086, 0x2918, "Intel Corporation", "82801IB (ICH9) LPC Interface Controller" },
  { 0x8086, 0x2922, "Intel Corporation", "82801IR/IO/IH6 port SATA Controller" },
  { 0x8086, 0x2930, "Intel Corporation", "82801I (ICH9 Family) SMBus Controller" },
  { 0x8086, 0x29C0, "Intel Corporation", "82G33/G31/P35/P31 Express DRAM Controller" },
  { 0x8086, 0x1237, "Intel Corporation", "440FX - 82441FX PMC" },
  { 0x8086, 0x7000, "Intel Corporation", "82371SB PIIX3 ISA" },
  { 0x8086, 0x7010, "Intel Corporation", "82371SB PIIX3 IDE" },
  { 0x8086, 0x7113, "Intel Corporation", "82371AB/EB/MB PIIX4 ACPI" },
  { 0x1234, 0x1111, "Emulated", "Virtual Video Controller" },
  { 0x1234, 0x11E8, "Emulated", "EDU device" },
};

char* pci_get_vendor_name(u16_t vendor)
{
  for (unsigned int i = 0; i < sizeof pci_known_names / sizeof pci_known_names[0]; ++i)
    if (pci_known_names[i].vendor_id == vendor)
      return pci_known_names[i].vendor_name;
  
  return "Unknown";
}

char* pci_get_device_name(u16_t vendor, u16_t device)
{
  for (unsigned int i = 0; i < sizeof pci_known_names / sizeof pci_known_names[0]; ++i)
    if (pci_known_names[i].vendor_id == vendor
        && pci_known_names[i].device_id == device)
      return pci_known_names[i].device_name;
  
  return "Unknown";
}

pci_device_vendor_t pci_get_device_vendor(u8_t bus, u8_t slot, u8_t func)
{
  pci_device_vendor_t vd = {0};

  // Register 0, offset 0 contains device id and vendor id
  if ((vd.vendor_id = pci_readw(bus, slot, func, 0)) != PCI_DEVICE_VENDOR_NONE)
  {
    vd.device_id = pci_readw(bus, slot, func, 2);

    vd.vendor_name = pci_get_vendor_name(vd.vendor_id);
    vd.device_name = pci_get_device_name(vd.vendor_id, vd.device_id);
  }

  return vd;
}
