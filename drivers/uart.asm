  ;; -----------------------------------------------------------------
  ;;
  ;; UART driver
  ;; ===========
  ;;
  ;; You access UART registers via I/O ports in an indexed style, with
  ;; a base port + an offset that specifies the register. Each
  ;; register can allow read, write, or both.

  
  [bits 64]
  
  ;; 
  ;; I/O Ports
  ;; ----------
  ;; 
  %define UART_COM1 0x3F8
  %define UART_COM2 0x2F8
  ;; 
  ;; The following addresses are not fully reliable as they depend
  ;; on how the ports are connected to the machine and how the BIOS
  ;; is configured. COM1 and COM2 are mostly reliable.
  ;; 
  %define UART_COM3 0x3E8
  %define UART_COM4 0x2E8
  %define UART_COM5 0x5F8
  %define UART_COM6 0x4F9
  %define UART_COM7 0x5E8
  %define UART_COM8 0x4E8
  
  ;; 
  ;; Register index (offset from port)
  ;; ---------------------------------
  ;; 
  %define UART_REGISTER_RECEIVE_BUF_INDEX        0x0       ; r
  %define UART_REGISTER_TRANSMIT_BUF_INDEX       0x0       ; w
  %define UART_REGISTER_INTERRUPT_ENABLE_INDEX   0x1       ; r/w
  ;; With DLAB set to 1, this is the least significant byte of the
  ;; divisor value for setting the baud rate.
  %define UART_REGISTER_DIVISOR_LOW_INDEX        0x0       ; r/w, DLAB=1
  ;; With DLAB set to 1, this is the most significant byte of the
  ;; divisor value.
  %define UART_REGISTER_DIVISOR_HIGH_INDEX       0x1       ; r/w, DLAB=1
  %define UART_REGISTER_INTERRUPT_ID_INDEX       0x2       ; r
  %define UART_REGISTER_FIFO_CONTROL_INDEX       0x2       ; w
  ;; Line Control Register. The most significant bit of this register
  ;; is the DLAB.
  %define UART_REGISTER_LINE_CONTROL_INDEX       0x3       ; r/w
  %define UART_REGISTER_MODEM_CONTROL_INDEX      0x4       ; r/w
  %define UART_REGISTER_LINE_STATUS_INDEX        0x5       ; r
  %define UART_REGISTER_MODEM_STATUS_INDEX       0x6       ; r
  %define UART_REGISTER_SCRATCH_INDEX            0x7       ; r/w

  ;;
  ;; Functions
  ;; 
  
  ;; -----------------------------------------------------------------
  ;; Initialize port
  ;;
  ;; Args:
  ;;   r8w: port to initialize, should be one of UART_COM*
  ;;
  ;; Return:
  ;;   rax: 1 on success, or 0 on failure
  ;; 
uart_init_port:
  push rdx
  
  ;; Disable all interrupts
  mov dx, r8w
  add dx, UART_REGISTER_INTERRUPT_ENABLE_INDEX
  mov al, 0x00
  out dx, al
  
  ;; Enable DLAB (set baud rate divisor)
  mov dx, r8w
  add dx, UART_REGISTER_LINE_CONTROL_INDEX
  mov al, 1 << 7
  out dx, al
  
  ;; Set divisor to 3 (lo byte) 38400 baud
  mov dx, r8w
  add dx, UART_REGISTER_DIVISOR_LOW_INDEX
  mov al, 0x03
  out dx, al
  mov dx, r8w
  add dx, UART_REGISTER_DIVISOR_HIGH_INDEX
  mov al, 0x00
  out dx, al

  ;; 8 bits, no parity, one stop bit
  mov dx, r8w
  add dx, UART_REGISTER_LINE_CONTROL_INDEX
  mov al, 0x03
  out dx, al

  ;; Enable FIFO, clear them, with 14-byte threshold
  mov dx, r8w
  add dx, UART_REGISTER_FIFO_CONTROL_INDEX
  mov al, 0xC7
  out dx, al
  
  ;; IRQs enabled, RTS / RSR set
  mov dx, r8w
  add dx, UART_REGISTER_MODEM_CONTROL_INDEX
  mov al, 0x0B
  out dx, al
  
  ;; Test
  
  ;; Set in loopback mode
  mov dx, r8w
  add dx, UART_REGISTER_MODEM_CONTROL_INDEX
  mov al, 0x1E
  out dx, al
  
  ;; Send byte 0xAE
  mov dx, r8w
  add dx, UART_REGISTER_TRANSMIT_BUF_INDEX
  mov al, 0XAE
  out dx, al

  ;; Check if serial returns same byte
  mov dx, r8w
  add dx, UART_REGISTER_RECEIVE_BUF_INDEX
  in ax, dx

  cmp ax, 0xAE
  jne .uart_init_port_failed
  
  ;; If serial is not faulty set it in normal operation mode
  mov dx, r8w
  add dx, UART_REGISTER_MODEM_CONTROL_INDEX
  mov al, 0x0F
  out dx, al

  ;; Success
  mov rax, 0
  pop rdx
  ret

  .uart_init_port_failed:
  mov rax, 1
  pop rdx
  ret 

  ;; -----------------------------------------------------------------
  ;; Retrun 1 if transmisison buffer is ready to transmit (empty)
  ;;
  ;; Args:
  ;;   r8w: serial port, should be one of UART_COM*
  ;;
  ;; Returns:
  ;;   rax: 1 if buffer is ready, or 0 otherwise
  ;; 
uart_is_transmit_ready: 
  push rdx

  mov dx, r8w
  add dx, UART_REGISTER_LINE_STATUS_INDEX
  in ax, dx

  and ax, 1 << 5
  cmp ax, 0
  je .not_ready

  mov rax, 1
  pop rdx
  ret

  .not_ready:
  mov rax, 0
  pop rdx
  ret
    
  ;; -----------------------------------------------------------------
  ;; Write a char to serial port
  ;;
  ;; Args:
  ;;   r8w: serial port, should be one of UART_COM*
  ;;   r9b: byte to write
  ;; 
uart_write_char:
  push rax
  push rdx

  ;; Wait for transmission to be available
  .wait: 
  call uart_is_transmit_ready
  cmp rax, 1
  jne .wait

  mov al, r9b
  mov dx, r8w
  add dx, UART_REGISTER_TRANSMIT_BUF_INDEX
  out dx, al

  pop rdx
  pop rax
  ret
  
  ;; -----------------------------------------------------------------
  ;; Write a NULL terminated string to serial port
  ;;
  ;; Args:
  ;;   r8w: serial port, should be one of UART_COM*
  ;;   r9: address of NULL terminated string to write
  ;;
uart_write_string:
  push rax

  mov rax, r9
  
  .loop:
  cmp byte[rax], 0
  je .done

  mov r9b, byte[rax]
  call uart_write_char

  ;; Next
  inc rax
  jmp .loop
  
  .done:
  pop rax
  ret

  ;; -----------------------------------------------------------------
  ;; Write a number as hex using ASCII
  ;;
  ;; Args:
  ;;   r8w: serial port, should be one of UART_COM*
  ;;   r9: the number to write
uart_write_hex:
  push r9
  push rax

  ;; Print '0x'
  mov rax, r9                   ; backup
  mov r9, '0'
  call uart_write_char
  mov r9, 'x'
  call uart_write_char
  mov r9, rax                   ; restore

  .loop:
  
  mov rax, r9                   ; backup
  shr r9, 60                    ; keep only the high 4 bits, shifted
                                ; to the right
  
  cmp rax, 0                    ; see if number is 0 (before shift)
  je .end

  cmp r9, 10
  jge .alpha

    ;; Is digit
    add r9, '0'
    call uart_write_char
    jmp .next

    .alpha:
    ;; Is alpha
    sub r9, 10
    add r9, 'A'
    call uart_write_char
  
    .next:
    mov r9, rax                 ; restore
    shl r9, 4                   ; set new high bytes
    jmp .loop
  
  .end:

  pop rax
  pop r9
  ret
  
