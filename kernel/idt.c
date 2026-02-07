// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/idt.h>   // implements
#include <kernel/utils.h>

// The IDT
idt_gate_t idt[IDT_ENTRIES] __attribute__((aligned(16))) = {0};

// This descriptor will be loaded with the `lidt` instruction to tell
// the position of the IDT to the CPU
idt_descriptor_t idt_descriptor __attribute__((aligned(16))) =
  (idt_descriptor_t) {
  .size   = sizeof(idt) - 1,
  .offset = (u64_t) &idt,
};

void idt_set_gate(u8_t  gate_number,
                  u64_t isr_address,
                  bool  is_trap)
{
  u8_t attributes = 0b10001110;
  if (is_trap) attributes |= 0x01;

  // Refer to the documentation in the header to understand these
  // values
  idt[gate_number] = (idt_gate_t) {
    .base_low      = (u16_t) (isr_address & 0xFFFF),
    .cs_selector   = 0x08,
    .ist           = 0,
    .attributes    = attributes,
    .base_middle   = (u16_t) ((isr_address >> 16) & 0xFFFF),
    .base_high     = (u32_t) ((isr_address >> 32) & 0xFFFFFFFF),
    .reserved      = 0
  };
  return;
}

void idt_set(void)
{
  disable_interrupts();

  // The various isr are implemented in assembly
  idt_set_gate(0,  (u64_t) isr0, true);
  idt_set_gate(1,  (u64_t) isr1, true);
  idt_set_gate(2,  (u64_t) isr2, false);
  idt_set_gate(3,  (u64_t) isr3, true);
  idt_set_gate(4,  (u64_t) isr4, true);
  idt_set_gate(5,  (u64_t) isr5, true);
  idt_set_gate(6,  (u64_t) isr6, true);
  idt_set_gate(7,  (u64_t) isr7, true);
  idt_set_gate(8,  (u64_t) isr8, true);
  idt_set_gate(9,  (u64_t) isr9, true);
  idt_set_gate(10, (u64_t) isr10, true);
  idt_set_gate(11, (u64_t) isr11, true);
  idt_set_gate(12, (u64_t) isr12, true);
  idt_set_gate(13, (u64_t) isr13, true);
  idt_set_gate(14, (u64_t) isr14, true);
  idt_set_gate(15, (u64_t) isr15, true);
  idt_set_gate(16, (u64_t) isr16, true);
  idt_set_gate(17, (u64_t) isr17, true);
  idt_set_gate(18, (u64_t) isr18, true);
  idt_set_gate(19, (u64_t) isr19, true);
  idt_set_gate(20, (u64_t) isr20, true);
  idt_set_gate(21, (u64_t) isr21, true);
  idt_set_gate(22, (u64_t) isr22, true);
  idt_set_gate(23, (u64_t) isr23, true);
  idt_set_gate(24, (u64_t) isr24, true);
  idt_set_gate(25, (u64_t) isr25, true);
  idt_set_gate(26, (u64_t) isr26, true);
  idt_set_gate(27, (u64_t) isr27, true);
  idt_set_gate(28, (u64_t) isr28, true);
  idt_set_gate(29, (u64_t) isr29, true);
  idt_set_gate(30, (u64_t) isr30, true);
  idt_set_gate(31, (u64_t) isr31, true);
  idt_set_gate(32, (u64_t) isr32, false);
  idt_set_gate(33, (u64_t) isr33, false);

  idt_load((u64_t) &idt_descriptor);

  enable_interrupts();
  return;
}
