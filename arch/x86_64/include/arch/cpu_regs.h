// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_ARCH_X86_64_CPU_REGS_H
#define POVOS_ARCH_X86_64_CPU_REGS_H

#include <kernel/macros.h>

//
// Struct containing CPU registers, used for context switching
//
// Some registers are commented out, this means that they exist but we
// don't currently care about (because they are not really used
// anywhere other than during boot maybe), so there is no point in
// storing them.
typedef struct cpu_regs {

  // General Purtpose Registers

  u64_t rax;
  u64_t rbx;
  u64_t rcx;
  u64_t rdx;
  u64_t rsi;
  u64_t rdi;
  u64_t rsp;   // stack pointer
  u64_t rbp;   // base pointer
  u64_t r8;
  u64_t r9;
  u64_t r10;
  u64_t r11;
  u64_t r12;
  u64_t r13;
  u64_t r14;
  u64_t r15;
  
  u64_t rip;   // instruction pointer

  // Segment regisers

  /*
  u16_t cs;   // code segment
  u16_t ds;   // data segment
  u16_t ss;   // stack segment
  u16_t ex;   // extra segment
  u16_t fs;   // general-purpose segment
  u16_t gs;   // general-purpose segment
  */

  // RFLAGS Register
  u64_t rflags;

  // Control Registers
  // u64_t cr0;
  // u64_t cr2;
  u64_t cr3;
  // u64_t cr4;
  // u64_t cr8;

  // Debug registers
  /*
  u64_t dr0;   // linear addresses of a breakpoint
  u64_t dr1;   // linear addresses of a breakpoint
  u64_t dr2;   // linear addresses of a breakpoint
  u64_t dr3;   // linear addresses of a breakpoint
  u64_t dr6;   // debug conditions occurred
  u64_t dr7;
  */

  // Test registers
  /*
  u64_t tr6;   // test command register
  u64_t tr7;   // test data register
  */
  
  // Protected Mode Registers

  /*
  u64_t gdt_ptr;
  u64_t ldt_segment_selector;
  u64_t tss_segment_selector;
  u64_t idtr;
  */

  // FPU Registers (legacy)...
  // MMX Registers...
  // SSE registers...
  
} _packed cpu_regs_t;

typedef union cpu_rflags_reg {
  struct _packed {
    u8_t cf     : 1;  // carry flag
    u8_t r1     : 1;  // reserved
    u8_t pf     : 1;  // parity flag
    u8_t r2     : 1;  // reserved
    u8_t af     : 1;  // auxilary carry flag
    u8_t r3     : 1;  // reserved
    u8_t zf     : 1;  // zero flag
    u8_t sf     : 1;  // sign flag
    u8_t tf     : 1;  // trap flag
    u8_t intf   : 1;  // interrupt enable flag
    u8_t df     : 1;  // direction flag
    u8_t of     : 1;  // overflow flag
    u8_t iopl   : 2;  // I/O privilege level
    u8_t nt     : 1;  // nested task
    u8_t r4     : 1;  // reserved
    u8_t rf     : 1;  // resume flag
    u8_t vm     : 1;  // virtual-8086 mode
    u8_t ac     : 1;  // alignment check / access control
    u8_t vif    : 1;  // virtual interrupt flag
    u8_t vip    : 1;  // virtual interrupt pending
    u8_t id     : 1;  // ID flag
    u64_t r5    : 43;  // reserved
  } values;
  u64_t raw;
} _packed cpu_rflags_reg_t;

typedef union cpu_cr0_reg {
  struct _packed {
    u8_t pe    : 1;  // protected mode enable
    u8_t mp    : 1;  // monitor co-processor
    u8_t em    : 1;  // emulation
    u8_t ts    : 1;  // task switched
    u8_t et    : 1;  // extension type
    u8_t ne    : 1;  // numeric error
    u16_t r1   : 10; // reserved
    u8_t wp    : 1;  // write protect
    u8_t r2    : 1;  // reserved
    u8_t am    : 1;  // alignment mask
    u16_t r3   : 10; // reserved
    u8_t nw    : 1;  // non-write through
    u8_t cd    : 1;  // cache disable
    u8_t pg    : 1;  // paging
    u32_t r4;        // reserved
  } values;
  u64_t raw;
} _packed cpu_cr9_reg_t;

// Note that CR3 must be page aligned
typedef union cpu_cr3_reg {
  struct _packed {
    u8_t r1   : 2;  // reserved
    u8_t pwt  : 1;  // Page-Level Write Through
    u8_t pcd  : 1;  // Page-Level Cache Disable
    u8_t r2   : 7;  // reserved
    u64_t pdb : 53; // Physical Base Address of the PML4 
  } values;         // when cr4.pcide = 0
  struct _packed {
    u16_t pcid : 11;
    u64_t pdb  : 53; // Physical Base Address of the PML4 
  } values_pcide;    // when cr4.pcide = 1
  u64_t raw;
} _packed cpu_cr3_reg_t;

//
// Functions
//

u64_t regs_get_cr3(void);
u64_t regs_get_rflags(void);

// Save all registers in [regs]
void regs_save(cpu_regs_t *regs);

// Perform context switching
void cpu_do_context_switch(cpu_regs_t *regs);

#endif // POVOS_ARCH_X86_64_CPU_REGS_H
