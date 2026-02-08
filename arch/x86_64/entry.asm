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
  extern idt_set
  extern debug_dump_regs_uart
  
  section .startup
  
  ;; Entry point, the bootloader will jump here
kernel_entry:
  
  ;; Triggers breakpoint exception (for debugging)
  ;;   int3

  call kernel_main
  cmp rax, 0
  jne .kernel_error
  
  .exit:
  hlt
  jmp .exit

  .kernel_error:
  ;; The kernel already prints an error message, so do nothing
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
