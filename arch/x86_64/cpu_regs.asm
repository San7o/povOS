  [bits 64]

  global regs_get_cr3
  global regs_get_rflags
  global regs_save
  global cpu_do_context_switch

  ;; -----------------------------------------------------------------
  ;; Returns the value of cr3 in rax
regs_get_cr3:
  mov rax, cr3
  ret
  
  ;; -----------------------------------------------------------------
  ;; Returns the value of rflags in rax
regs_get_rflags:
  pushfq
  pop rax
  ret
  
  ;; -----------------------------------------------------------------
  ;; Save all registers in [rdi], which must be a pointer to a
  ;; cpu_regs_t struct.
regs_save:
  mov [rdi], rax
  mov [rdi + 8], rbx
  mov [rdi + 16], rcx
  mov [rdi + 24], rdx
  mov [rdi + 32], rsi
  mov [rdi + 40], rdi
  mov [rdi + 56], rbp
  mov [rdi + 64], r8
  mov [rdi + 72], r9
  mov [rdi + 80], r10
  mov [rdi + 88], r11
  mov [rdi + 96], r12
  mov [rdi + 104], r13
  mov [rdi + 112], r14
  mov [rdi + 120], r15

  ;; rsp
  lea rax, [rsp + 8]            ; Point to where RSP was before `call`
                                ; pushed RIP
  mov [rdi+48], rax
  
  ;; rip
  mov rax, [rsp]
  mov [rdi + 128], rax

  ;; rflags
  pushfq
  pop rax
  mov [rdi + 136], rax

  mov rax, cr3
  mov [rdi + 144], rax

  ret

  ;; -----------------------------------------------------------------
  ;; Perform context switching to cpu_regs_t passed in [rdi] as a
  ;; pointer
cpu_do_context_switch:
  ; RDI = pointer to cpu_regs_t
    
  ; Switch Address Space
  mov rax, [rdi + 144] ; cr3
  mov cr3, rax

  ; Construct the IRETQ frame on the NEW stack
  mov rbx, [rdi + 48]  ; New RSP
  mov rsp, rbx         ; Switch stack pointer immediately
    
  ; Push the IRETQ frame (SS, RSP, RFLAGS, CS, RIP)
  push qword 0x10      ; New SS (Usually Data Selector, e.g., 0x10)
  push rbx             ; New RSP
  push qword [rdi + 136] ; RFLAGS
  push qword 0x08      ; New CS (Usually Code Selector, e.g., 0x08)
  push qword [rdi + 128] ; New RIP

  ; Restore GPRs (except RAX and RDI)
  mov rbx, [rdi + 8]
  mov rcx, [rdi + 16]
  mov rdx, [rdi + 24]
  mov rsi, [rdi + 32]
  mov rbp, [rdi + 56]
  mov r8,  [rdi + 64]
  mov r9,  [rdi + 72]
  mov r10, [rdi + 80]
  mov r11, [rdi + 88]
  mov r12, [rdi + 96]
  mov r13, [rdi + 104]
  mov r14, [rdi + 112]
  mov r15, [rdi + 120]

  ; Restore RDI and RAX last
  mov rax, [rdi + 0]
  mov rdi, [rdi + 40]

  iretq ; Resumes the task
