// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_PIC_H
#define POVOS_DRIVERS_PIC_H

// 
// Programmable Interrupt Controller
// =================================
//
// The IBM PIC (aka 8259 PIC) is responsible to manage hardware
// interrupts and send them to the appropriate system interrupt.
//
// Note that on modern systems, especially multicore processors, APIC
// has replaced PIC. This header only provides an interface to manage
// PIC.
//
// The IBM PC has two PIC chips, each one providing 8 IRQs to the
// system in a cascading way: interrupts can flow from one chip to the
// other. This provides a total of 15 IRQs (and not 16 because one is
// used so signal the cascading behaviour).
#define PIC1    0x0020
#define PIC2    0x00A0

//
// IBM PC Interrupt Table
// ----------------------
//
// Here are the 15 interrupts:
//
//    IRQ0   8  timer (55ms intervals, 18.2 per second)
//    IRQ1   9  keyboard service required
//    IRQ2   A  slave 8259 or EGA/VGA vertical retrace
//    IRQ3   B  COM2 or COM4 service required, (COM3-COM8 on MCA PS/2)
//    IRQ4   C  COM1 or COM3 service required
//    IRQ5   D  fixed disk or data request from LPT2
//    IRQ6   E  floppy disk service required
//    IRQ7   F  data request from LPT1 (unreliable on IBM mono)
//    IRQ8  70  real time clock  (AT,XT286,PS50+)
//    IRQ9  71  software redirected to IRQ2  (AT,XT286,PS50+)
//    IRQ10 72  reserved  (AT,XT286,PS50+)
//    IRQ11 73  reserved  (AT,XT286,PS50+)
//    IRQ12 74  mouse interrupt  (PS50+)
//    IRQ13 75  numeric coprocessor error  (AT,XT286,PS50+)
//    IRQ14 76  fixed disk controller (AT,XT286,PS50+)
//    IRQ15 77  reserved  (AT,XT286,PS50+)
//

// Each PIC (master or slave) have a command and data port.
#define PIC_MASTER_CMD        PIC1
#define PIC_MASTER_DATA   (PIC1+1)
#define PIC_SLAVE_CMD         PIC2
#define PIC_SLAVE_DATA     (PIC2+1)

//
// Commands
// --------
//
// Source: David Jurgens' HelpPC Quick Reference Utility
//
//
//  Initialization Command Word 1 at Port 20h and A0h
//
//  |7|6|5|4|3|2|1|0|  ICW1
//   | | | | | | | `---- 1=ICW4 is needed, 0=no ICW4 needed
//   | | | | | | `----- 1=single 8259, 0=cascading 8259's
//   | | | | | `------ 1=4 byte interrupt vectors, 0=8 byte int vectors
//   | | | | `------- 1=level triggered mode, 0=edge triggered mode
//   | | | `-------- must be 1 for ICW1 (port must also be 20h or A0h)
//   `------------- must be zero for PC systems
//
//  Initialization Command Word 2 at Port 21h and A1h
//
//  |7|6|5|4|3|2|1|0|  ICW2
//   | | | | | `-------- 000= on 80x86 systems
//   `----------------- A7-A3 of 80x86 interrupt vector
//
//  Initialization Command Word 3 at Port 21h and A1h
//
//  |7|6|5|4|3|2|1|0|  ICW3 for Master Device
//   | | | | | | | `---- 1=interrupt request 0 has slave, 0=no slave
//   | | | | | | `----- 1=interrupt request 1 has slave, 0=no slave
//   | | | | | `------ 1=interrupt request 2 has slave, 0=no slave
//   | | | | `------- 1=interrupt request 3 has slave, 0=no slave
//   | | | `-------- 1=interrupt request 4 has slave, 0=no slave
//   | | `--------- 1=interrupt request 5 has slave, 0=no slave
//   | `---------- 1=interrupt request 6 has slave, 0=no slave
//   `----------- 1=interrupt request 7 has slave, 0=no slave
// 
//  |7|6|5|4|3|2|1|0|  ICW3 for Slave Device
//   | | | | | `-------- master interrupt request slave is attached to
//   `----------------- must be zero
//
//  Initialization Command Word 4 at Port 21h and A1h
//
//  |7|6|5|4|3|2|1|0|  ICW4
//   | | | | | | | `---- 1 for 80x86 mode, 0 = MCS 80/85 mode
//   | | | | | | `----- 1 = auto EOI, 0=normal EOI
//   | | | | `-------- slave/master buffered mode (see below)
//   | | | `--------- 1 = special fully nested mode (SFNM), 0=sequential
//   `-------------- unused (set to zero)
//
//  Bits
//   32 Buffering Mode
//   00 not buffered
//   01 not buffered
//   10 buffered mode slave (PC mode)
//   11 buffered mode master (PC mode)
//
//  Operation Control Word 1 / Interrupt Mask Reg.  (Ports 21h & A1h)
//
//  |7|6|5|4|3|2|1|0|  OCW1 - IMR Interrupt Mask Register
//   | | | | | | | `---- 0 = service IRQ0, 1 = mask off
//   | | | | | | `----- 0 = service IRQ1, 1 = mask off
//   | | | | | `------ 0 = service IRQ2, 1 = mask off
//   | | | | `------- 0 = service IRQ3, 1 = mask off
//   | | | `-------- 0 = service IRQ4, 1 = mask off
//   | | `--------- 0 = service IRQ5, 1 = mask off
//   | `---------- 0 = service IRQ6, 1 = mask off
//   `----------- 0 = service IRQ7, 1 = mask off
//
//  Operation Control Word 2 / Interrupt Command Reg. (Ports 20h & A0h)
//
//  |7|6|5|4|3|2|1|0|  OCW2 - ICR Interrupt Command Register
//   | | | | | `-------- interrupt request level to act upon
//   | | | | `--------- must be 0 for OCW2
//   | | | `---------- must be 0 for OCW2
//   `--------------- EOI type (see table)
//
//  Bits
//  765  EOI - End Of Interrupt code (PC specific)
//  001  non-specific EOI command
//  010  NOP
//  011  specific EOI command
//  100  rotate in automatic EOI mode
//  101  rotate on non-specific EOI command
//  110  set priority command  (uses bits 2-0)
//  111  rotate on specific EOI command
//
//  Operation Control Word 3   (Ports 20h & A0h)
//
//  |7|6|5|4|3|2|1|0|  OCW3
//   | | | | | | | `--- 1=read IRR on next read, 0=read ISR on next read
//   | | | | | | `---- 1=act on value of bit 0, 0=no action if bit 0 set
//   | | | | | `----- 1=poll command issued, 0=no poll command issued
//   | | | | `------ must be 1 for OCW3
//   | | | `------- must be 0 for OCW3
//   | | `-------- 1=set special mask, 0=reset special mask
//   | `--------- 1=act on value of bit 5, 0=no action if bit 5 set
//   `---------- not used (zero)
//
//  Other Registers
//
//  IRR - Interrupt Request Register, maintains a bit vector indicating
//        which IRQ hardware events are awaiting service. Highest
//        level interrupt is reset when the CPU acknowledges interrupt.
//  ISR - In Service Register, tracks IRQ line currently being serviced.
//        Updated by EOI command.
//
#define PIC_ICW1_ICW4       0x01    // Indicates that ICW4 will be present
#define PIC_ICW1_SINGLE     0x02    // Single (cascade) mode
#define PIC_ICW1_INTERVAL4  0x04    // Call address interval 4 (8)
#define PIC_ICW1_LEVEL      0x08    // Level triggered (edge) mode
#define PIC_ICW1_INIT       0x10    // Initialization
#define PIC_ICW4_8086       0x01    // 8086/88 (MCS-80/85) mode
#define PIC_ICW4_AUTO       0x02    // Auto (normal) EOI
#define PIC_ICW4_BUF_SLAVE  0x08    // Buffered mode/slave
#define PIC_ICW4_BUF_MASTER 0x0C    // Buffered mode/master
#define PIC_ICW4_SFNM       0x10    // Special fully nested (not)
#define PIC_OCW3_EOI        0x20

// In protected mode, IRQs 0–15 map to intel's interrupts 8–23, but
// these are reserved as x86_64 CPU exceptions! We need to move them
// to Interrupts 32–47 instead.
void pic_remap(void);
// Send acknowledgement to PIC. You need to send this each time after
// handling the interrupt.
void pic_ack(void);
void pic_disable(void);

#endif // POVOS_DRIVERS_PIC_H
