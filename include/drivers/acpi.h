// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_ACPI_H
#define POVOS_DRIVERS_ACPI_H

//
// ACPI
// ====
//
// ACPI (Advanced Configuration and Power Interface) is a standard
// that allows us to discover and configure computer hardware
// components, manage power usage, and perform tasks like putting
// unused hardware to sleep. It provides a series of tables and
// structure that we can use to interact with the hardware.
//
// The best resource is of course the specification itself:
//
//      https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/
//

#include <kernel/mm/bios_mmap.h>
#include <libk/stddef.h>

#define ACPI_VERSION_1 0
#define ACPI_VERSION_2 2

//
// Root System Description Pointer
//
// This is the "entry point" to all ACPI informationm it contains
// a pointer to the RSDT.
typedef struct acpi_rsdp {
  #define ACPI_RSDP_SIGNATURE "RSD PTR "
  u8_t  signature[8];
  // This is the checksum of the fields defined in the ACPI 1.0
  // specification. Summing up the first 20 bytes of this table
  // (including the checksum field) must result with the lower byte
  // being zero.
  u8_t  checksum;
  // An OEM-supplied string that identifies the OEM
  u8_t  oemid[6];
  // The revision of this structure. See the ACPI_VESION_ macros
  u8_t  revision;
  // 32 bit physical address of the RSDT (depricated in version 2.0)
  u32_t rsdt_address;

  // ACPI 2.0
  
  // The length of the table, in bytes, including the header, starting
  // from offset 0
  u32_t length;
  // 64 bit physical address of the XSDT
  u64_t xsdt_address;
  // This is a checksum of the entire table, including both checksum
  // fields. The sum of all bytes & 0xF must be 0.
  u8_t  extended_checksum;
  u8_t  reserved[3];
} __attribute__((packed)) acpi_rsdp_t;

//
// System Description Table Header
//
// Common header for several tables
typedef struct acpi_sdt_header {
  #define ACPI_SDT_SIGNATURE_SIZE 4
  u8_t  signature[ACPI_SDT_SIGNATURE_SIZE];
  // Length, in bytes, of the entire table. The length implies the
  // number of entry fields (n) at the end of the table (after the
  // header).
  u32_t length;
  u8_t  revision;
  // Entire table must sum to 0
  u8_t  checksum;
  u8_t  oem_id[6];
  // The table ID is the manufacture model ID. This field must match
  // the OEM Table ID in the FADT.
  u64_t oem_table_id;
  u32_t oam_revision;
  // Vendor ID of utility that created the table. For tables
  // containing Definition Blocks, this is the ID for the ASL
  // Compiler.
  u32_t creator_id;
  u32_t creator_revision;
} __attribute__((packed)) acpi_sdt_header_t;


//
// System Description Table (used in ACPI 1.0)
//
typedef struct acpi_rsdt {
  #define ACPI_RSDT_SIGNATURE "RSDT"
  acpi_sdt_header_t header;
  // An array of 32-bit physical addresses that point to other system
  // description table headers. Its size can be calculated from the
  // length field at the stop of this structure.
  u32_t entries[];
} __attribute__((packed)) acpi_rsdt_t;

//
// Extended System Description Table (used in ACPI 2.0)
//
// Same as RSDT header, but entries addresses are 64 bit wide.
typedef struct acpi_xsdt {
  #define ACPI_XSDT_SIGNATURE "XSDT"
  acpi_sdt_header_t header;
  // Addresses are 64 bit here.
  u64_t entries[];
} __attribute__((packed)) acpi_xsdt_t;

#define ACPI_ADDRSPACE_SYSTEM_MEMORY             0x00
#define ACPI_ADDRSPACE_SYSTEM_IO                 0x01
#define ACPI_ADDRSPACE_PCI_CONF                  0x02
#define ACPI_ADDRSPACE_EMBEDDED_CONTROLLER       0x03
#define ACPI_ADDRSPACE_SMBUS                     0x04
#define ACPI_ADDRSPACE_SYSTEM_CMOS               0x05
#define ACPI_ADDRSPACE_CPI_BAR_TARGET            0x06
#define ACPI_ADDRSPACE_IPMI                      0x07
#define ACPI_ADDRSPACE_GENERAL_PURPOSE_IO        0x08
#define ACPI_ADDRSPACE_GENERIC_SERIAL_BUS        0x09
// Platform communication channel
#define ACPI_ADDRSPACE_PCC                       0x0A
// 0x0B to 0x7E are Reserved
#define ACPI_ADDRSPACE_FUNCTIONAL_FIXED_HARDWARE 0x7F
// 0xC0 to 0xFF are OEM Defined

// Generic Address Structure
typedef struct acpi_addr
{
  // The address space where the data structure or register
  // exists. See the ACPI_ADDRSPACE_ macros
  u8_t  address_space;
  // The size in bits of the given register. When addressing a data
  // structure, this field must be zero.
  u8_t  bit_width;
  // The bit offset of the given register at the given address. When
  // addressing a data structure, this field must be zero.
  u8_t  bit_offset;
  // Specifies access size. Unless otherwise defined by the Address
  // Space ID:
  //     0 Undefined (legacy reasons)
  //     1 Byte access
  //     2 Word access
  //     3 Dword access
  //     4 QWord access
  u8_t  access_size;
  // The 64-bit address of the data structure or register in the given
  // address space (relative to the processor)
  u64_t address;
} acpi_addr_t;

// Power management profiles
#define ACPI_PM_PROFILE_UNSPECIFIED        0
#define ACPI_PM_PROFILE_DESKTOP            1
#define ACPI_PM_PROFILE_MOBILE             2
#define ACPI_PM_PROFILE_WORKSTATION        3
#define ACPI_PM_PROFILE_ENTERPRISE_SERVER  4
#define ACPI_PM_PROFILE_SOHO_SERVER        5
#define ACPI_PM_PROFILE_APPLIANCE_PC       6
#define ACPI_PM_PROFILE_PERFORMANCE_SERVER 7
#define ACPI_PM_PROFILE_TABLET             8
#define ACPI_PM_PROFILE_MAX                9  // profiles >= 9 are reserved

//
// Fixed ACPI Description Table
//
// Defines various fixed hardware ACPI information vital to an
// ACPI-compatible OS, such as the base address of several hardware
// registers blocks.
typedef struct acpi_fadt
{
  // hader.signature is "FACP"
  acpi_sdt_header_t header;
  // Physical memory address of the FACS, where OSPM and Firmware
  //exchange control information
  u32_t firmware_ctrl;
  // Physical memory address of the DSDT
  u32_t dsdt;
  // field used in ACPI 1.0; no longer in use, for compatibility only
  u8_t  reserved;
  // This field is set by the OEM to convey the preferred power
  // management profile to OSPM. OSPM can use this field to set
  // default power management policy parameters during OS
  // installation. Se the ACPI_PM_PROFILE_ defines.
  u8_t  preferred_pm_profile;
  // System vector the SCI interrupt is wired to in 8259 mode. On
  // systems that do not contain the 8259, this field contains the
  // Global System interrupt number of the SCI interrupt.
  u16_t sci_interrupt;
  // System port address of the SMI Command Port
  u32_t smi_command_port;
  // The value to write to SMI_CMD to disable SMI ownership of the
  // ACPI hardware registers
  u8_t  acpi_enable;
  // The value to write to SMI_CMD to re-enable SMI ownership of the
  // ACPI hardware registers
  u8_t  acpi_disable;
  // The value to write to SMI_CMD to enter the S4BIOS state. The
  // S4BIOS state provides an alternate way to enter the S4 state
  // where the firmware saves and restores the memory context. A value
  // of zero in S4BIOS_F indicates S4BIOS_REQ is not supported
  u8_t  s4bios_req;
  // If non-zero, this field contains the value OSPM writes to the
  // SMI_CMD register to assume processor performance state control
  // responsibility
  u8_t  pstate_control;
  
  // Addresses for several blocks
  u32_t pm1a_event_block;
  u32_t pm1b_event_block;
  u32_t pm1a_control_block;
  u32_t pm1b_control_block;
  u32_t pm2_control_block;
  u32_t pm_timer_block;
  u32_t gpe0_block;
  u32_t gpe1_block;
  
  u8_t  pm1_event_length;
  u8_t  pm1_control_length;
  u8_t  pm2_control_length;
  u8_t  pm_timer_length;
  u8_t  gpe0_length;
  u8_t  gpe1_length;
  u8_t  gpe1_base;
  u8_t  cstate_control;
  u16_t worstc2_latency;
  u16_t worstc3_latency;
  u16_t flush_size;
  u16_t flush_stride;
  u8_t  duty_offset;
  u8_t  duty_width;
  u8_t  day_alarm;
  u8_t  month_alarm;
  u8_t  century;

  // reserved in acpi 1.0; used since acpi 2.0+
  u16_t boot_architecture_flags;

  u8_t  reserved2;
  u32_t flags;

  // 12 byte structure; see below for details
  acpi_addr_t reset_reg;

  u8_t  reset_value;
  u8_t  reserved3[3];
  
  // 64bit pointers - available on acpi 2.0+
  u64_t                x_firmware_control;
  u64_t                x_dsdt;

  acpi_addr_t x_pm1a_event_block;
  acpi_addr_t x_pm1b_event_block;
  acpi_addr_t x_pm1a_control_block;
  acpi_addr_t x_pm1b_control_block;
  acpi_addr_t x_pm2_control_block;
  acpi_addr_t x_pm_timer_block;
  acpi_addr_t x_gpe0_block;
  acpi_addr_t x_gpe1_block;
} acpi_fadt_t;

// Find the address of the RSDP
// Returns NULL (0) if not found
acpi_rsdp_t* acpi_locate_rsdp(void);
void* acpi_locate_sdt(acpi_rsdp_t* rsdp, const char signature[4]);

#endif // POVOS_DRIVERS_ACPI_H
