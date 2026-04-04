  [bits 64]

  global regs_save
  global cpu_do_context_switch

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
  ; RDI currently points to our cpu_regs_t struct

  ; Switch Page Tables (Address Space)
  mov rax, [rdi + 144] ; Offset for cr3
  mov cr3, rax

  ; Prepare the stack

  mov rsp, [rdi + 48]  ; Load the new RSP from the struct
    
  ; Push the new RIP onto the NEW stack so 'ret' can find it
  mov rax, [rdi + 128] ; Offset for rip
  push rax

  ; Load General Purpose Registers
  mov rax, [rdi + 0]
  mov rbx, [rdi + 8]
  mov rcx, [rdi + 16]
  mov rdx, [rdi + 24]
  mov rsi, [rdi + 32]
  ; Skip RDI for a moment
  mov rbp, [rdi + 56]
  mov r8,  [rdi + 64]
  mov r9,  [rdi + 72]
  mov r10, [rdi + 80]
  mov r11, [rdi + 88]
  mov r12, [rdi + 96]
  mov r13, [rdi + 104]
  mov r14, [rdi + 112]
  mov r15, [rdi + 120]

  ; Load RFLAGS
  ; We can't mov to rflags directly. We push it and use popfq.
  push qword [rdi + 136] ; Offset for rflags
  popfq

  ; Finally, load RDI
  mov rdi, [rdi + 40]

  ; Our new stack has the new RIP at the top. 
  ; 'ret' will pop that RIP into the register and jump there.
  ret
