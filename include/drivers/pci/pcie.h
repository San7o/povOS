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
// This is the new PCI standard, it allows MMIO operations and memory
// addresses can be found via ACPI.
//
// Devices using PCI share a common bus (parallel compilation), while
// with PCI Express heach device has its own dedicated connection to
// the switch (a switch is defined as a logical assembly of multiple
// virtual PCI-to-PCI Bridge devices).
//
// The official reference is the PCI Express Base Specification
// document. It is not officially distributed for free, but you will
// be able to find it online. The most relevan part of us is chapter 7
// "Software Initialization and Configuration".
//
// Layers
// ------
//
// The specification divides the architecture in several stacked
// layers: transaction, data link and physical. Devices uses packets
// to communicate information between components, this architecture
// resembles the ISO/OSI for networking stack.
//
// - transaction layer: The upper Layer of the architecture is the
//   Transaction Layer. The Transaction Layer’s primary responsibility
//   is the assembly and disassembly of TLPs. TLPs are used to
//   communicate transactions, such as read and write, as well as
//   certain types of events. The Transaction Layer is also
//   responsible for managing credit-based flow control for TLPs.
//
// - data link layer: The middle Layer in the stack, the Data Link
//   Layer, serves as an intermediate stage between the Transaction
//   Layer and the Physical Layer. The primary responsibilities of the
//   Data Link Layer include Link management and data integrity,
//   including error detection and error correction.
//
// - physical layer: The Physical Layer includes all circuitry for
//   interface operation, including driver and input buffers,
//   parallel-to-serial and serial-to-parallel conversion, PLL(s), and
//   impedance matching circuitry. It also includes logical functions
//   related to interface initialization and maintenance.
//
// The PCI Express interrupt model supports two mechanisms:
//
// - INTx emulation
// - Message Signaled Interrupt (MSI/MSI-X)
//
//
// Configuration
// -------------
//
// Chapter 7. The PCI Express Configuration model supports two
// Configuration Space access mechanisms:
//
// - PCI-compatible Configuration Access Mechanism (CAM). It supports
//   100% binary compatibility with Conventional PCI or later aware
//   operating systems 
// - PCI Express Enhanced Configuration Access Mechanism (ECAM)
//
// Once you find an PCIe entry from ACPI, you need to use its base
// address to calculate the physical address of the configuration
// space header of the device you want to work with. Use the
// PCIE_PHYS_ADDR macro for this. From the header you can interact
// with the device via MMIO for example by sending commands.
//

#include <drivers/acpi/acpi.h>

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
  pcie_acpi_entry_t   entries[];
} _packed pcie_acpi_sdt_t;

typedef struct pcie_common_config_space_header {
  u16_t vendor_id;
  u16_t device_id;
  #define PCIE_CONFIG_SPACE_CMD_IO_ENABLE                    0
  #define PCIE_CONFIG_SPACE_CMD_MEMORY_ENABLE            (1<<0)
  #define PCIE_CONFIG_SPACE_CMD_BUS_MASTER_ENABLE        (1<<1)
  #define PCIE_CONFIG_SPACE_CMD_SPECIAL_CYCLE_ENABLE     (1<<2)
  #define PCIE_CONFIG_SPACE_CMD_MEM_WRITE                (1<<3)
  #define PCIE_CONFIG_SPACE_CMD_VGA_PALETTE_SNOOP        (1<<4)
  #define PCIE_CONFIG_SPACE_CMD_PARITY_ERROR_RESPONSE    (1<<5)
  #define PCIE_CONFIG_SPACE_CMD_IDSEL_STEPPING           (1<<6)
  #define PCIE_CONFIG_SPACE_CMD_SERR_ENABLE              (1<<7)
  #define PCIE_CONFIG_SPACE_CMD_FAST_TRANSACTIONS_ENABLE (1<<8)
  #define PCIE_CONFIG_SPACE_CMD_INT_DISABLE              (1<<9)
  u16_t command;
  u16_t status;
  u8_t  revision_id;
  u8_t  class_code[3];
  u8_t  cache_line_size;
  u8_t  latency_timer;
  #define PCIE_CONFIG_SPACE_HEADER_TYPE0       0x0
  #define PCIE_CONFIG_SPACE_HEADER_TYPE1       0x1
  #define PCIE_CONFIG_SPACE_HEADER_TYPE_MASK   0x7F
  u8_t  header_type;
  u8_t  bist;
  u64_t type_specific[9];
  u8_t  capabilities_ptr;
  u8_t  type_specific2[7];
  u8_t  interrupt_line;
  u8_t  interrupt_pin;
  u8_t  type_specific3[2];
} _packed pcie_common_config_space_header_t;

typedef struct pcie_type0_config_space_header {
  u16_t vendor_id;
  u16_t device_id;
  u16_t command;
  u16_t status;
  u8_t  revision_id;
  u8_t  class_code[3];
  u8_t  cache_line_size;
  u8_t  latency_timer;
  u8_t  header_type;
  u8_t  bist;
  u32_t base_address_regs[6];  // aka BARs
  u32_t cardbus_cis_ptr;
  u16_t subsystem_vendor_id;
  u16_t subsystem_id;
  u32_t expansion_rom_base_addr;
  u8_t  capabilities_ptr;
  u8_t  reserved[7];
  u8_t  interrupt_line;
  u8_t  interrupt_pin;
  u8_t  min_gnt;
  u8_t  max_lat;
} _packed pcie_type0_config_space_header_t;

typedef struct pcie_type1_config_space_header {
  u16_t vendor_id;
  u16_t device_id;
  u16_t command;
  u16_t status;
  u8_t  revision_id;
  u8_t  class_code[3];
  u8_t  cache_line_size;
  u8_t  primary_latency_timer;
  u8_t  header_type;
  u8_t  bist;
  u32_t base_addr_reg0;
  u32_t base_addr_reg1;
  u8_t  primary_bus_number;
  u8_t  secondary_bus_number;
  u8_t  subordinate_bus_number;
  u8_t  secondary_latency_timer;
  u8_t  io_base;
  u8_t  io_limit;
  u16_t secondary_status;
  u16_t memory_base;
  u16_t memory_limit;
  u16_t prefetchable_memory_base;
  u16_t prefetchable_memory_limit;
  u32_t prefetchable_base_upper32;
  u32_t prefetchable_limit_upper32;
  u16_t io_base_upper16;
  u16_t io_base_limit_upper16;
  u8_t  capabilities_ptr;
  u8_t  reserved[3];
  u32_t expansion_rom_base_addr;
  u8_t  interrupt_line;
  u8_t  interrupt_pin;
  u16_t bridge_control;
} _packed pcie_type1_config_space_header_t;

#define PCIE_PHYS_ADDR(base, bus, device, function) \
  (base + ((bus) << 20 | device << 15 | function << 12))

#endif // POVOS_DRIVERS_PCIE_H
