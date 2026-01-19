
  ;; -----------------------------------------------------------------
  ;; Dump all register values over UART
debug_dump_regs_uart:
  push r8
  push r9
  
  mov r8w, UART_COM1
  mov r9, debug_dump_regs_uart_intro_str
  call uart_write_string

  ;; rax
  mov r9, debug_dump_regs_uart_rax_str
  call uart_write_string
  mov r9, rax
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char
  
  ;; rbx
  mov r9, debug_dump_regs_uart_rbx_str
  call uart_write_string
  mov r9, rbx
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char
  
  ;; rcx
  mov r9, debug_dump_regs_uart_rcx_str
  call uart_write_string
  mov r9, rcx
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char
  
  ;; rdx
  mov r9, debug_dump_regs_uart_rdx_str
  call uart_write_string
  mov r9, rdx
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; rsi
  mov r9, debug_dump_regs_uart_rsi_str
  call uart_write_string
  mov r9, rsi
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char
  
  ;; rdi
  mov r9, debug_dump_regs_uart_rdi_str
  call uart_write_string
  mov r9, rdi
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; rsp
  mov r9, debug_dump_regs_uart_rsp_str
  call uart_write_string
  mov r9, rsp
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; rbp
  mov r9, debug_dump_regs_uart_rbp_str
  call uart_write_string
  mov r9, rbp
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r8
  mov r9, debug_dump_regs_uart_r8_str
  call uart_write_string
  mov r9, [rsp+8]
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r9
  mov r9, debug_dump_regs_uart_r9_str
  call uart_write_string
  pop r9
  call uart_write_hex
  push r9
  mov r9, `\n`
  call uart_write_char

  ;; r10
  mov r9, debug_dump_regs_uart_r10_str
  call uart_write_string
  mov r9, r10
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r11
  mov r9, debug_dump_regs_uart_r11_str
  call uart_write_string
  mov r9, r11
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r12
  mov r9, debug_dump_regs_uart_r12_str
  call uart_write_string
  mov r9, r12
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r13
  mov r9, debug_dump_regs_uart_r13_str
  call uart_write_string
  mov r9, r13
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r14
  mov r9, debug_dump_regs_uart_r14_str
  call uart_write_string
  mov r9, r14
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  ;; r15
  mov r9, debug_dump_regs_uart_r15_str
  call uart_write_string
  mov r9, r15
  call uart_write_hex
  mov r9, `\n`
  call uart_write_char

  mov r9, debug_dump_regs_uart_outro_str
  call uart_write_string

  pop r9
  pop r8
  ret

debug_dump_regs_uart_intro_str: db `-------------- debug -------------\n`, 0
debug_dump_regs_uart_outro_str: db `------------ end debug -----------\n`, 0
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
