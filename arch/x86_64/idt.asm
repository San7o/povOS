  [bits 64]

  section .text

  ;; Exported symbols
  global idt_load
  global isr0
  global isr1
  global isr2
  global isr3
  global isr4
  global isr5
  global isr6
  global isr7
  global isr8
  global isr9
  global isr10
  global isr11
  global isr12
  global isr13
  global isr14
  global isr15
  global isr16
  global isr17
  global isr18
  global isr19
  global isr20
  global isr21
  global isr22
  global isr23
  global isr24
  global isr25
  global isr26
  global isr27
  global isr28
  global isr29
  global isr30
  global isr31
  global isr32
  global isr33

  ;; Imported symbols
  extern isr_common_handler
  extern isr_keyboard_handler
  extern idt_descriptor
  extern idt

  ;; -----------------------------------------------------------------
  ;; Load the IDT from the address in rdi
idt_load:
  lidt [rdi]
  ret
  
  ;;
  ;; Interrupt service routines implementation
  ;; -----------------------------------------
  ;;
  ;; This macro disables interrupts, saves registers and calls a
  ;; function.
%macro ISR_HANDLER 1
  cli
  
  ;; save all registers
  push r15
  push r14
  push r13
  push r12
  push r11
  push r10
  push r9
  push r8
  push rdi
  push rsi
  push rbp
  push rbx
  push rdx
  push rcx
  push rax

  mov rdi, [rsp+120]            ; ISR number
  mov rsi, [rsp+128]            ; Error code

  call %1
  
  pop rax
  pop rcx
  pop rdx
  pop rbx
  pop rbp
  pop rsi
  pop rdi
  pop r8
  pop r9
  pop r10
  pop r11
  pop r12
  pop r13
  pop r14
  pop r15
  
  add rsp, 16     ; Cleans up the pushed error code and pushed ISR number

  sti
  iretq
%endmacro
  
  ;; Some isr push a 32 bit error code. Here we manually push a 0
  ;; value so that the isr handler can expect to have the save number
  ;; of elements in the stack to pop.
  
  ;;  0: Divide Error fault
isr0:
  push qword 0    ; push a dummy error code to keep a uniform stack frame
  push qword 0    ; isr number
  ISR_HANDLER isr_common_handler

  ;;  1: Debug exception fault / trap
isr1:
  push qword 0                   ; dummy value
  push qword 1                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 2: Non maskable external interrupt
isr2:
  push qword 0                   ; dummy value
  push qword 2                   ; isr number
  ISR_HANDLER isr_common_handler
  
  ;; 3: Breakpoint trap
isr3:
  push qword 0                   ; dummy value
  push qword 3                   ; isr number
  ISR_HANDLER isr_common_handler
  
  ;; 4: Overflow trap
isr4:
  push qword 0                   ; dummy value
  push qword 4                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 5: BOUND range exceeded fault
isr5:
  push qword 0                   ; dummy value
  push qword 5                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 6: Invalid Opcode fault
isr6:
  push qword 0                   ; dummy value
  push qword 6                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 7: Device not available (no main coprocessor) fault
isr7:
  push qword 0                   ; dummy value
  push qword 7                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 8: Double fault abort
isr8:
  push qword 8                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 9: Coprocessor Segment Overrun (reserved) fault
isr9:
  push qword 0                   ; dummy value
  push qword 9                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 10: Invalid TSS fault
isr10:
  push qword 10                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 11: Segment not present fault
isr11:
  push qword 11                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 12: Stack-segment fault
isr12:
  push qword 12                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 13: General Protection fault
isr13:
  push qword 13                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 14: Page fault
isr14:
  push qword 14                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 15: (intel reserved, do not use)
isr15:
  push 0                         ; dummy value
  push qword 15                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 16: x87 FPU Floating-point error (math fault)
isr16:
  push 0                         ; dummy value
  push qword 16                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 17: Alignment Check fault
isr17:
  push qword 17                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 18: Machine Check Abort
isr18:
  push 0                         ; dummy value
  push qword 18                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 19: SIMD Floating-point exception fault
isr19:
  push 0                         ; dummy value
  push qword 19                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 20: virtualization exception fault
isr20:
  push 0                         ; dummy value
  push qword 20                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 21: Control Protection Exception fault
isr21:
  push qword 21                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 22: Intel reserved. Do not use.
isr22:
  push qword 0
  push qword 22                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 23: Intel reserved. Do not use.
isr23:
  push qword 0
  push qword 23                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 24: Intel reserved. Do not use.
isr24:
  push qword 0
  push qword 24                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 25: Intel reserved. Do not use.
isr25:
  push qword 0
  push qword 25                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 26: Intel reserved. Do not use.
isr26:
  push qword 0
  push qword 26                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 27: Intel reserved. Do not use.
isr27:
  push qword 0
  push qword 27                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 28: Intel reserved. Do not use.
isr28:
  push qword 0
  push qword 28                   ; isr number
  ISR_HANDLER isr_common_handler
  
  ;; 29: Intel reserved. Do not use.
isr29:
  push qword 0
  push qword 29                   ; isr number
  ISR_HANDLER isr_common_handler
  
  ;; 30: Intel reserved. Do not use.
isr30:
  push qword 0
  push qword 30                   ; isr number
  ISR_HANDLER isr_common_handler

  ;; 31: Intel reserved. Do not use.
isr31:
  push qword 0
  push qword 31                   ; isr number
  ISR_HANDLER isr_common_handler
  
  ;; 32: Mouse Interrupt
isr32:
  push qword 0
  push qword 32                   ; isr number

  ;; Does nothing special for now
  ISR_HANDLER isr_common_handler
  
  ;; 33: Keyboard Interrupt
isr33:
  push qword 0
  push qword 33                   ; isr number
  ISR_HANDLER isr_keyboard_handler
  
