  [bits 64]

  global regs_save
  
  ;; Save all registers in [rdi], which must be a pointer to a
  ;; cpu_regs_t struct.
regs_save:
  push rax
  
  mov [rdi], rax
  mov [rdi + 8], rbx
  mov [rdi + 16], rcx
  mov [rdi + 24], rdx
  mov [rdi + 32], rsi
  mov [rdi + 40], rdi
  mov [rdi + 48], rsp
  mov [rdi + 56], rbp
  mov [rdi + 64], r8
  mov [rdi + 72], r9
  mov [rdi + 80], r10
  mov [rdi + 88], r11
  mov [rdi + 96], r12
  mov [rdi + 104], r13
  mov [rdi + 112], r14
  mov [rdi + 120], r15

  lea rax, [rsp]
  mov [rdi + 128], rax

  pushfq
  pop rax
  mov [rdi + 136], rax

  mov rax, cr3
  mov [rdi + 144], rax

  pop rax
  ret
