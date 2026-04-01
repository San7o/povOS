
  global debug_dump_regs_uart

  extern uart_port1
  extern uart_write_str
  extern uart_write_hex
  extern uart_putc

  ;; -----------------------------------------------------------------
  ;; Dump all register values over UART
debug_dump_regs_uart:
  push rdi
  push rsi
  push rax

  ;; rax
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rax_str
  call uart_write_str
  mov rsi, rax
  mov rdi, [uart_port1]  
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc
  
  ;; rbx
  mov rdi, [uart_port1]  
  mov rsi, debug_dump_regs_uart_rbx_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, rbx
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc
  
  ;; rcx
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rcx_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, rcx
  call uart_write_hex
  mov rdi, [uart_port1]  
  mov rsi, `\n`
  call uart_putc
  
  ;; rdx
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rdx_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, rdx
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; rsi
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rsi_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, [rsp]
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc
  
  ;; rdi
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rdi_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, [rsp+8]
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; rsp
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rsp_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, rsp
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; rbp
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_rbp_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, rbp
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r8
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r8_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r8
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r9
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r9_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r9
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r10
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r10_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r10
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r11
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r11_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r11
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r12
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r12_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r12
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r13
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r13_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r13
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r14
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r14_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r14
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  ;; r15
  mov rdi, [uart_port1]
  mov rsi, debug_dump_regs_uart_r15_str
  call uart_write_str
  mov rdi, [uart_port1]
  mov rsi, r15
  call uart_write_hex
  mov rdi, [uart_port1]
  mov rsi, `\n`
  call uart_putc

  pop rax
  pop rsi
  pop rdi
  ret

debug_dump_regs_uart_rax_str:   db `[debug] [regs] rax: `, 0
debug_dump_regs_uart_rbx_str:   db `[debug] [regs] rbx: `, 0
debug_dump_regs_uart_rcx_str:   db `[debug] [regs] rcx: `, 0
debug_dump_regs_uart_rdx_str:   db `[debug] [regs] rdx: `, 0
debug_dump_regs_uart_rsi_str:   db `[debug] [regs] rsi: `, 0
debug_dump_regs_uart_rdi_str:   db `[debug] [regs] rdi: `, 0  
debug_dump_regs_uart_rsp_str:   db `[debug] [regs] rsp: `, 0
debug_dump_regs_uart_rbp_str:   db `[debug] [regs] rbp: `, 0
debug_dump_regs_uart_r8_str:    db `[debug] [regs] r8:  `, 0  
debug_dump_regs_uart_r9_str:    db `[debug] [regs] r9:  `, 0
debug_dump_regs_uart_r10_str:   db `[debug] [regs] r10: `, 0
debug_dump_regs_uart_r11_str:   db `[debug] [regs] r11: `, 0
debug_dump_regs_uart_r12_str:   db `[debug] [regs] r12: `, 0
debug_dump_regs_uart_r13_str:   db `[debug] [regs] r13: `, 0
debug_dump_regs_uart_r14_str:   db `[debug] [regs] r14: `, 0
debug_dump_regs_uart_r15_str:   db `[debug] [regs] r15: `, 0
