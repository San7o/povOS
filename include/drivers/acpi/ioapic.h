// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_IOAPIC_H
#define POVOS_DRIVERS_IOAPIC_H

//
// I/O APIC
// ========
//
// Advanced Programmable Interrupt Controller, replaces the old PIC.
// The MADT table provides a list of IOAPIC records of different
// types.
//

#include <drivers/acpi/acpi.h>

#define IOAPIC_ACPI_SIGNATURE "APIC"

// Common header for each record type
typedef struct ioapic_record_header {
  // Specifies which specific record this entry represents
  // You should cast this header to the corresponding entry to access
  // the fields
  u8_t entry_type;
  // The length of this record
  u8_t length;
} _packed ioapic_record_header_t;

//
// Local APIC record - type 0
//
// The advent of multi-threaded processors yielded multiple logical
// processors executing on common processor hardware. ACPI defines
// logical processors in an identical manner as physical processors.
//
// When using the APIC interrupt model, each processor in the system
// is required to have a Processor Local APIC record in the MADT, and
// a processor device object in the DSDT.
typedef struct ioapic_record_local_apic {
  ioapic_record_header_t header;
  // (depricated) The OS associates this Local APIC Structure with a
  // processor object in the namespace when the _UID child object of
  // the processor’s device object (or the ProcessorId listed in the
  // Processor declaration operator) evaluates to a numeric value that
  // matches the numeric value in this field.
  u8_t  processir_uid;
  // The processor's local APIC ID
  u8_t  apic_id;
  // Flags
  #define IOAPIC_LOCAL_APIC_ENABLED_FLAG          0x0
  #define IOAPIC_LOCAL_APIC_ONLINE_CAPABLE_FLAG   0x1
  u32_t flags;
} _packed ioapic_record_local_apic_t;

//
// I/O APIC Structure - type 1
//
typedef struct ioapic_record_ioapic {
  ioapic_record_header_t header;
  u8_t ioapic_id;
  u8_t reserved;
  // The 32-bit physical address to access this I/O APIC. Each I/O
  // APIC resides at a unique address.
  u32_t ioapic_addr;
  // Global system interrup base
  //
  // The global system interrupt number where this I/O APIC’s
  // interrupt inputs start. The number of interrupt inputs is
  // determined by the I/O APIC’s Max Redir Entry register.
  u32_t gsystem_int_base;
} _packed ioapic_record_ioapic_t;

//
// Interrupt source override structure - type 2
//
// Interrupt Source Overrides are necessary to describe variances
// between the IA-PC standard dual 8259 interrupt definition and the
// platform’s implementation.
//
// It is assumed that the ISA interrupts will be identity-mapped into
// the first I/O APIC sources. Most existing APIC designs, however,
// will contain at least one exception to this assumption. The
// Interrupt Source Override Structure is provided in order to
// describe these exceptions. It is not necessary to provide an
// Interrupt Source Override for every ISA interrupt. Only those that
// are not identity-mapped onto the APIC interrupt inputs need be
// described.
//
// For example, if your machine has the ISA Programmable Interrupt
// Timer (PIT) connected to ISA IRQ 0, but in APIC mode, it is
// connected to I/O APIC interrupt input 2, then you would need an
// Interrupt Source Override where the source entry is ‘0’ and the
// Global System Interrupt is ‘2.’
typedef struct ioapic_record_int_src_override {
  ioapic_record_header_t header;
  // 0 constant, meaning ISA
  u8_t bus;
  // Bus-relative interrupt source
  u8_t source;
  // Global system interrupt
  //
  // The Global System Interrupt that this bus-relative interrupt
  // source will signal.
  u32_t gsystem_int;
  // flags
  //
  // Polarity of the APIC I/O input signals:
  //    00 Conforms to the specifications of the bus (for example,
  //       EISA is active-low for level-triggered interrupts).
  //    01 Active high
  //    10 Reserved
  //    11 Active low
  #define IOAPIC_INT_POLARITY_FLAG      0x3
  // Trigger mode of the APIC I/O Input signals:
  //    00 Conforms to specifications of the bus (For example, ISA is edge-triggered)
  //    01 Edge-triggered
  //    10 Reserved
  //    11 Level-triggered
  #define IOAPIC_INT_TRIGGER_MODE_FLAG  (0x3 << 2)
  u16_t flags;
} _packed ioapic_record_int_src_override_t;

//
// Non-Maskable Interrupt Source - type 3
//
// This structure allows a platform designer to specify which I/O
// (S)APIC interrupt inputs should be enabled as non-maskable. Any
// source that is non-maskable will not be available for use by
// devices.
typedef struct ioapic_record_nmi_src {
  ioapic_record_header_t header;
  // Same as interrupt source flags (IOAPIC_INT_*)
  u16_t flags;
  // The Global System Interrupt that this NMI will signal
  u32_t gsystem_interrupt;
} _packed ioapic_record_nmi_src_t;

//
// Local APIC NMI - type 4
//
// This structure describes the Local APIC interrupt input (LINTn)
// that NMI is connected to for each of the processors in the system
// where such a connection exists. This information is needed by OSPM
// to enable the appropriate local APIC entry.
//
// Each Local APIC NMI connection requires a separate Local APIC NMI
// structure. For example, if the platform has 4 processors with ID
// 0-3 and NMI is connected LINT1 for processor 3 and 2, two Local
// APIC NMI entries would be needed in the MADT.
typedef struct ioapic_record_nmi {
  ioapic_record_header_t header;
  // (depricated) Value corresponding to the _UID listed in the
  // processor’s device object, or the Processor ID corresponding to
  // the ID listed in the processor object. A value of 0xFF signifies
  // that this applies to all processors in the machine.
  u8_t  acpi_processor_uid;
  // Same as interrupt source flags (IOAPIC_INT_*)
  u16_t flags;
  // Local APIC interrupt input LINTn to which NMI is connected.
  u8_t  local_apic_lint;
} _packed ioapic_record_nmi_t;

//
// Local APIC Address Override - type 5
//
// This optional structure supports 64-bit systems by providing an
// override of the physical address of the local APIC in the MADT’s
// table header, which is defined as a 32-bit field.
//
// If defined, OSPM must use the address specified in this structure
// for all local APICs (and local SAPICs), rather than the address
// contained in the MADT’s table header. Only one Local APIC Address
// Override Structure may be defined.
typedef struct ioapic_record_local_apic_addr_override {
  ioapic_record_header_t header;
  u16_t  reserved;
  // Physical address of Local APIC
  u64_t  local_apic_addr;
} _packed ioapic_record_local_apic_addr_override_t;

// Record type 6 to 8 (SAPIC)
// TODO

// Processor Local x2APIC - type 9
//
// The Processor X2APIC structure is very similar to the processor
// local APIC structure. When using the X2APIC interrupt model,
// logical processors are required to have a processor device object
// in the DSDT and must convey the processor’s APIC information to
// OSPM using the Processor Local X2APIC structure.
typedef struct ioapic_record_proc_local_x2apic {
  ioapic_record_header_t header;
  u16_t reserved;
  // The processor's local x2APIC ID
  u32_t x2apic_id;
  // Same as Local APIC flags
  u32_t flags;
  // OSPM associates the X2APIC Structure with a processor object
  // declared in the namespace using the Device statement, when the
  // _UID child object of the processor device evaluates to a numeric
  // value, by matching the numeric value with this field.
  u32_t acpi_processor_uid;
} _packed ioapic_record_proc_local_x2apic_t;

// Record type 0xA to 0x10 (GIC)
// TODO

typedef struct ioapic_acpi_sdt {
  acpi_sdt_header_t header;
  u32_t local_addr;
  
  // Flags
  //
  // indicates that the system also has a PC-AT-compatible dual-8259
  // setup
  #define IOAPIC_PCAT_COMPAT_FLAG 1
  u32_t flags;

  // List of records
  // Node: You must not access them linearly, eg records[0], records[1] ...
  // This is because sizeof(ioapic_record_header_t) is not the size
  // of the entire record, you need to compute it dynamically by
  // reading the lenght field in the record header.
  ioapic_record_header_t records[];
} _packed ioapic_acpi_sdt_t;

#endif // POVOS_DRIVERS_IOAPIC_H
