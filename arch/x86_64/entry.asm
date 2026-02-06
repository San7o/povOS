  ;; -----------------------------------------------------------------
  ;;
  ;; OS entrypoint
  ;; =============
  ;;
  ;; The bootloader just slapped you in x86_64 long (protected) mode
  ;; with a flat memory layout. You are ring 0, you can do
  ;; anything. You don't have access to the fancy BIOS routines
  ;; anymore, so you will have to get creative and implement your own
  ;; drivers, but you have a full 64-bit address space and the power
  ;; of CISC to play with.
  ;;
  ;; Check out drivers/uart.asm for an example driver.
  ;; 

  [bits 64]
  
  global kernel_entry
  
  extern kernel_main
  extern vga_get_memory_map
  extern vga_clear
  extern vga_print
  extern vga_print_hex
  extern vga_is_alpha_disabled
  extern vga_memory_map_error_str
  extern vga_alphanumeric_error_str
  extern uart_init_port
  extern uart_write_hex
  extern uart_write_str
  extern uart_putc
  extern pic_remap
  extern idt_load
  extern debug_dump_regs_uart
  
  section .startup
  
  ;; Entry point
kernel_entry:
  
  ;; 
  ;; Sanity checks
  ;;

  call vga_get_memory_map       ; Check VGA memory map value is 11
  cmp rax, 0b11
  jne .vga_memory_map_error

  call vga_is_alpha_disabled    ; Check VGA mode is alphanumeric
  cmp rax, 0
  jne .vga_alphanumeric_error

  mov rdi, 0x3F8            ; Initialize UART port COM1
  call uart_init_port
  and rax, 0x1
  cmp rax, 1
  jne .uart_init_error

  ;; Checks succesfull
  
  ;; Setup IDT
  call pic_remap                ; Change IRQ number for PIC
  call idt_load                 ; Load the interrupt descriptor table
  sti                           ; Enable interrupts
  
  ;; Clean the screen
  mov rdi, 0x1F
  call vga_clear
  
  ;; Success message
  mov rdx, 0x1F                  ; style
  mov rsi, greet_str             ; string
  mov rdi, 0                     ; position
  call vga_print

  ;; Uart message
  mov rdi, 0x3F8
  mov rsi, greet_str
  call uart_write_str

  ;; Uart hex number
  mov rsi, 0x6969
  call uart_write_hex
  mov rsi, `\n`
  call uart_putc
  
  ;; Vga hex number
  mov rdi, 19                    ; offset
  mov rsi, 0x6969
  mov rdx, 0x1F
  call vga_print_hex

  xor rax, rax

  call debug_dump_regs_uart
  
  ;; Divide by zero exception test
  ;;   mov rax, 10
  ;;   xor rdx, rdx
  ;;   mov rcx, 0
  ;;   idiv rcx

  ;; Triggers breakpoint exception
  ;;   int3

  call kernel_main
  cmp rax, 0
  jne .kernel_error
  
  .exit:
  ret

  .kernel_error:
  ;; Clean the screen
  mov rdi, 0x4F                 ; style
  call vga_clear
  
  mov rdx, 0xF                 ; style
  mov rsi, kernel_main_error_str ; string
  mov rdi, 0                     ; position
  call vga_print
  
  jmp .exit
 
  .vga_memory_map_error:
  
  ;; Clean the screen
  mov rdi, 0x4F                 ; style
  call vga_clear
  
  mov rdx, 0xF                 ; style
  mov rsi, vga_memory_map_error_str ; string
  mov rdi, 0                     ; position
  call vga_print
  jmp .exit
  
  .vga_alphanumeric_error:
  
  ;; Clean the screen
  mov rdi, 0x4F
  call vga_clear

  mov rdx, 0xF                 ; style
  mov rsi, vga_alphanumeric_error_str ; string
  mov rdi, 0                     ; position
  call vga_print
  jmp .exit

  .uart_init_error:

  ;; Clean the screen
  mov rdi, 0x4F
  call vga_clear

  mov rdx, 0xF                 ; style
  mov rsi, vga_uart_init_error_str ; string
  mov rdi, 0                     ; position
  call vga_print
  jmp .exit
   
  ;; Strings
  
greet_str:                   db `Hello, from povOS!`, 0
vga_uart_init_error_str:     db `Error initializing UART`, 0
kernel_main_error_str:       db `Kernel exited with error`, 0
vga_memory_map_error_str:    db `Memory map range not supported`, 0
vga_alphanumeric_error_str:  db `VGA alphanumeric mode is disabled`, 0
