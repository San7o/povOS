
  global debug_dump_regs_uart

  extern UART_COM1
  extern uart_write_str
  extern uart_write_hex
  extern uart_putc

UART_COM1:   equ 0x3F8
  
  ;; -----------------------------------------------------------------
  ;; Dump all register values over UART
debug_dump_regs_uart:
  push rdi
  push rsi
  
  mov rdi, UART_COM1
  mov rsi, debug_dump_regs_uart_intro_str
  call uart_write_str

  ;; rax
  mov rsi, debug_dump_regs_uart_rax_str
  call uart_write_str
  mov rsi, rax
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc
  
  ;; rbx
  mov rsi, debug_dump_regs_uart_rbx_str
  call uart_write_str
  mov rsi, rbx
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc
  
  ;; rcx
  mov rsi, debug_dump_regs_uart_rcx_str
  call uart_write_str
  mov rsi, rcx
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc
  
  ;; rdx
  mov rsi, debug_dump_regs_uart_rdx_str
  call uart_write_str
  mov rsi, rdx
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; rsi
  mov rsi, debug_dump_regs_uart_rsi_str
  call uart_write_str
  mov rsi, [rsp]
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc
  
  ;; rdi
  mov rsi, debug_dump_regs_uart_rdi_str
  call uart_write_str
  mov rsi, [rsp+8]
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; rsp
  mov rsi, debug_dump_regs_uart_rsp_str
  call uart_write_str
  mov rsi, rsp
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; rbp
  mov rsi, debug_dump_regs_uart_rbp_str
  call uart_write_str
  mov rsi, rbp
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r8
  mov rsi, debug_dump_regs_uart_r8_str
  call uart_write_str
  mov rsi, r8
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r9
  mov rsi, debug_dump_regs_uart_r9_str
  call uart_write_str
  mov rsi, r9
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r10
  mov rsi, debug_dump_regs_uart_r10_str
  call uart_write_str
  mov rsi, r10
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r11
  mov rsi, debug_dump_regs_uart_r11_str
  call uart_write_str
  mov rsi, r11
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r12
  mov rsi, debug_dump_regs_uart_r12_str
  call uart_write_str
  mov rsi, r12
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r13
  mov rsi, debug_dump_regs_uart_r13_str
  call uart_write_str
  mov rsi, r13
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r14
  mov rsi, debug_dump_regs_uart_r14_str
  call uart_write_str
  mov rsi, r14
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  ;; r15
  mov rsi, debug_dump_regs_uart_r15_str
  call uart_write_str
  mov rsi, r15
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc

  mov rsi, debug_dump_regs_uart_outro_str
  call uart_write_str

  pop rsi
  pop rdi
  ret

debug_dump_regs_uart_intro_str: db `[debug] --------------\n`, 0
debug_dump_regs_uart_outro_str: db `[end debug] ------------\n`, 0
debug_dump_regs_uart_rax_str:   db `rax `, 0
debug_dump_regs_uart_rbx_str:   db `rbx `, 0
debug_dump_regs_uart_rcx_str:   db `rcx `, 0
debug_dump_regs_uart_rdx_str:   db `rdx `, 0
debug_dump_regs_uart_rsi_str:   db `rsi `, 0
debug_dump_regs_uart_rdi_str:   db `rdi `, 0  
debug_dump_regs_uart_rsp_str:   db `rsp `, 0
debug_dump_regs_uart_rbp_str:   db `rbp `, 0
debug_dump_regs_uart_r8_str:    db `r8  `, 0  
debug_dump_regs_uart_r9_str:    db `r9  `, 0
debug_dump_regs_uart_r10_str:   db `r10 `, 0
debug_dump_regs_uart_r11_str:   db `r11 `, 0
debug_dump_regs_uart_r12_str:   db `r12 `, 0
debug_dump_regs_uart_r13_str:   db `r13 `, 0
debug_dump_regs_uart_r14_str:   db `r14 `, 0
debug_dump_regs_uart_r15_str:   db `r15 `, 0
