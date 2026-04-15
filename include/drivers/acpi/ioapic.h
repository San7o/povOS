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
// Advanced Programmable Interrupt Controller, replaces the old
// PIC. With the I/O APIC, interrupts can be distributed to physical
// or logical (clusters of) processors and can be prioritized. Each
// I/O APIC typically handles 24 external interrupts.  The MADT ACPI
// table provides a list of IOAPIC records of different types.
//
// Datasheet: 82093AA I/O Advanced Programmable Interrupt Controller
//
// Overview
// --------
//
// While the standard ISA Compatible interrupt controller (located in
// the PIIX3) is intended for use in a uni-processor system, the I/O
// Advanced Programmable Interrupt Controller (IOAPIC) can be used in
// either a uni-processor or multi-processor system. The IOAPIC
// provides multi-processor interrupt management and incorporates both
// static and dynamic symmetric interrupt distribution across all
// processors. In systems with multiple I/O subsystems, each subsystem
// can have its own set of interrupts.
//
// In a uni-processor system, the IOAPIC's dedicated interrupt bus can
// reduce interrupt latency over the standard interrupt controller
// (i.e., the latency associated with the propagation of the interrupt
// acknowledge cycle across multiple busses using the standard
// interrupt controller approach). Interrupts can be controlled by the
// standard ISA Compatible interrupt controller in the PIIX3, the
// IOAPIC unit, or mixed mode where both the standard ISA Compatible
// Interrupt Controller and IOAPIC are used. The selection of which
// controller responds to an interrupt is determined by how the
// interrupt controllers are programmed. Note that it is the
// programmer's responsibility to make sure that the same interrupt
// input signal is not handled by both interrupt controllers.
//
// At the system level, APIC consists of two parts - one residing in
// the I/O subsystem (called the IOAPIC) and the other in the CPU
// (called the Local APIC). The local APIC and the IOAPIC communicate
// over a dedicated APIC bus. The IOAPIC bus interface consists of two
// bi-directional data signals (APICD[1:0]) and a clock input
// (APICCLK)
//
// The CPU's Local APIC Unit contains the necessary intelligence to
// determine whether or not its processor should accept interrupts
// broadcast on the APIC bus. The Local Unit also provides local
// pending of interrupts, nesting and masking of interrupts, and
// handles all interactions with its local processor (e.g., the
// INTR/INTA/EOI protocol). The Local Unit further provides
// inter-processor interrupts and a timer, to its local processor. The
// register level interface of a processor to its local APIC is
// identical for every processor.
//
// The IOAPIC Unit consists of a set of interrupt input signals, a
// 24-entry by 64-bit Interrupt Redirection Table, programmable
// registers, and a message unit for sending and receiving APIC
// messages over the APIC bus. I/O devices inject interrupts into the
// system by asserting one of the interrupt lines to the IOAPIC. The
// IOAPIC selects the corresponding entry in the Redirection Table and
// uses the information in that entry to format an interrupt request
// message. Each entry in the Redirection Table can be individually
// programmed to indicate edge/level sensitive interrupt signals, the
// interrupt vector and priority, the destination processor, and how
// the processor is selected (statically or dynamically). The
// information in the table is used to transmit a message to other
// APIC units (via the APIC bus).
//
// The IOAPIC contains a set of programmable registers. Two of the
// registers (I/O Register Select and I/O Window Registers) are
// located in the CPU's memory space and are used to indirectly access
// the other APIC registers as described in the datasheet at section
// 3.0, Register Description. The Version Register provides the
// implementation version of the IOAPIC. The IOAPIC ID Register is
// programmed with an ID value that serves as a physical name of the
// IOAPIC. This ID is loaded into the ARB ID Register when the IOAPIC
// ID Register is written and is used during bus arbitration
//

//
// ACPI
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
// In an APIC implementation, there are one or more I/O APICs. Each
// I/O APIC has a series of interrupt inputs, referred to as INTIn,
// where the value of n is from 0 to the number of the last interrupt
// input on the I/O APIC. The I/O APIC structure declares which global
// system interrupts are uniquely associated with the I/O APIC
// interrupt inputs. There is one I/O APIC structure for each I/O APIC
// in the system.
typedef struct ioapic_record_ioapic {
  ioapic_record_header_t header;
  // The I/O APIC’s ID
  u8_t ioapic_id;
  u8_t reserved;
  // The 32-bit physical address to access this I/O APIC. Each I/O
  // APIC resides at a unique address
  u32_t ioapic_addr;
  // Global system interrupt base
  //
  // The global system interrupt number where this I/O APIC’s
  // interrupt inputs start. The number of interrupt inputs is
  // determined by the I/O APIC’s Max Redir Entry register
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
  // source will signal
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
  // The 32-bit physical address at which each processor can access
  // its local interrupt controller.
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

//
// Registers
// ---------
//
// The IOAPIC registers are accessed by an indirect addressing scheme
// using two registers (IOREGSEL and IOWIN) that are located in the
// CPU's memory space (memory address specified by the APICBASE
// Register located in the PIIX3). These two registers are
// re-locateable (via the APICBASE Register). In the IOAPIC only the
// IOREGSEL and IOWIN Registers are directly accesable in the memory
// address space.
//

// I/O Register Select (index), r/w
//
// This register selects the IOAPIC Register to be read/written. The
// data is then read from or written to the selected register through
// the IOWIN Register.
#define IOAPIC_IOREGSEL_ADDR(base)    base
// I/O Window (data), r/w
//
// This register is used to write to and read from the register
// selected by the IOREGSEL Register. Readability/writability is
// determined by the IOAPIC register that is currently selected
#define IOAPIC_IOWIN_ADDR(base)       (base + 0x10)

#define IOAPIC_IOAPICID_REG_INDEX          0x00   // r/w
#define IOAPIC_IOAPICVER_REG_INDEX         0x01   // r
#define IOAPIC_IOAPICARB_REG_INDEX         0x02   // r
//
// There are 24 I/O Redirection Table entry registers. Each register
// is a dedicated entry for each interrupt input signal. Unlike IRQ
// pins of the 8259A, the notion of interrupt priority is completely
// unrelated to the position of the physical interrupt input signal on
// the APIC. Instead, software determines the vector (and therefore
// the priority) for each corresponding interrupt input signal. For
// each interrupt signal, the operating system can also specify the
// signal polarity (low active or high active), whether the interrupt
// is signaled as edges or levels, as well as the destination and
// delivery mode of the interrupt. The information in the redirection
// table is used to translate the corresponding interrupt pin
// information into an inter-APIC message
//
// An ioredtbl is a 64 bit registers, we we need to perform two reads
// or writes
#define IOAPIC_IOREDTBL_REG_INDEX_LOW(n)   (0x10 + 2 * n)      // r/w
#define IOAPIC_IOREDTBL_REG_INDEX_HIGH(n)  (0x10 + 2 * n + 1)  // r/w

typedef union ioapic_ioapicid_reg {
  struct {
    u32_t reserved  : 24;
    u8_t  id        : 4;
    u8_t  reserved2 : 4;
  } values;
  u32_t raw;
} _packed ioapic_ioapicid_reg_t;

typedef union ioapic_apicver_reg {
  struct {
    u8_t apic_version;  // r
    u8_t reserved;
    // This field contains the entry number (0 being the lowest entry)
    // of the highest entry in the I/O Redirection Table. The value is
    // equal to the number of interrupt input pins for the IOAPIC
    // minus one. The range of values is 0 through 239. For this
    // IOAPIC, the value is 17h.
    u16_t max_redirection;
    u8_t  reserved2;
  } values;
  u32_t raw;
} _packed ioapic_ioapic_reg_t;

typedef union ioapic_ioapicarb_reg {
  struct {
    u16_t reserved;
    u8_t  reserved2;
    // This 4 bit field contains the IOAPIC Arbitration ID.
    u8_t  id        : 4;  // r/w
    u8_t  reserved3 : 4;
  } values;
  u32_t raw;
} _packed ioapic_ioapicarb_reg_t;

typedef union ioapic_ioredtbl_reg {
  struct {
    // The vector field is an 8 bit field containing the interrupt
    // vector for this interrupt. Vector values range from 0x10 to
    // 0xFE
    u8_t int_vector;  // r/w
    // The Delivery Mode is a 3 bit field that specifies how the APICs
    // listed in the destination field should act upon reception of
    // this signal. Note that certain Delivery Modes only operate as
    // intended when used in conjunction with a specific trigger
    // Mode. These restrictions are indicated in the following table
    // for each Delivery Mode.
    //
    // See the datasheet for additional information
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_FIXED           000
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_LOWEST_PRIORITY 001
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_SMI             001
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_NMI             100
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_INIT            101
    #define IOAPIC_IOREDTBL_DELIVERY_MODE_EXINT           111
    u8_t delivery_mode    : 3;  // r/w
    // This field determines the interpretation of the Destination
    // field. When DESTMOD=0 (physical mode), a destination APIC is
    // identified by its ID. Bits 56 through 59 of the Destination
    // field specify the 4 bit APIC ID. When DESTMOD=1 (logical mode),
    // destinations are identified by matching on the logical
    // destination under the control of the Destination Format
    // Register and Logical Destination Register in each Local APIC.
    #define IOAPIC_IOREDTBL_DESTINATION_MODE_PHYSICAL     0
    #define IOAPIC_IOREDTBL_DESTINATION_MODE_LOGICAL      1
    u8_t destination_mode : 1;   // r/w
    // The Delivery Status bit contains the current status of the
    // delivery of this interrupt. Delivery Status is read-only and
    // writes to this bit (as part of a 32 bit word) do not effect
    // this bit. 0=IDLE (there is currently no activity for this
    // interrupt). 1=Send Pending (the interrupt has been injected but
    // its delivery is temporarily held up due to the APIC bus being
    // busy or the inability of the receiving APIC unit to accept that
    // interrupt at that time).
    u8_t delivery_status  : 1;   // r
    // Interrupt Input Pin Polarity - This bit specifies the polarity
    // of the interrupt signal. 0=High active, 1=Low active.
    u8_t intpol           : 1;   // r/w
    // This bit is used for level triggered interrupts. Its meaning is
    // undefined for edge triggered interrupts. For level triggered
    // interrupts, this bit is set to 1 when local APIC(s) accept the
    // level interrupt sent by the IOAPIC. The Remote IRR bit is set
    // to 0 when an EOI message with a matching interrupt vector is
    // received from a local APIC
    u8_t remote_irr       : 1;   // r
    // The trigger mode field indicates the type of signal on the
    // interrupt pin that triggers an interrupt. 1=Level sensitive,
    // 0=Edge sensitive
    u8_t trigger_mode     : 1;   // r/w
    // When this bit is 1, the interrupt signal is
    // masked. Edge-sensitive interrupts signaled on a masked
    // interrupt pin are ignored (i.e., not delivered or held
    // pending). Level-asserts or negates occurring on a masked
    // level-sensitive pin are also ignored and have no side
    // effects. Changing the mask bit from unmasked to masked after
    // the interrupt is accepted by a local APIC has no effect on that
    // interrupt. This behavior is identical to the case where the
    // device withdraws the interrupt before that interrupt is posted
    // to the processor. It is software's responsibility to handle the
    // case where the mask bit is set after the interrupt message has
    // been accepted by a local APIC unit but before the interrupt is
    // dispensed to the processor. When this bit is 0, the interrupt
    // is not masked. An edge or level on an interrupt pin that is not
    // masked results in the delivery of the interrupt to the
    // destination.
    u8_t  int_mask         : 1;   // r/w
    u64_t reserved         : 39;
    // If the Destination Mode of this entry is Physical Mode (bit
    // 11=0), bits [59:56] contain an APIC ID. If Logical Mode is
    // selected (bit 11=1), the Destination Field potentially defines
    // a set of processors. Bits [63:56] of the Destination Field
    // specify the logical destination address.
    u8_t destination_field;       // r/w
  } values;
  u64_t raw;
} _packed ioapic_ioredtbl_reg_t;

//
// Functions
//

void  ioapic_write_reg(u64_t apic_base, u8_t offset, u32_t val);
u32_t ioapic_read_reg(u64_t apic_base, u8_t offset);

#endif // POVOS_DRIVERS_IOAPIC_H
