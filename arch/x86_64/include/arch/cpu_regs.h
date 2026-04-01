// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_ARCH_X86_64_CPU_REGS_H
#define POVOS_ARCH_X86_64_CPU_REGS_H

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
  
} cpu_regs_t;

// Save all registers in [regs]
void regs_save(cpu_regs_t *regs);

#endif // POVOS_ARCH_X86_64_CPU_REGS_H
