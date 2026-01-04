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

  %include "drivers/uart.asm"
  
  section .text
  
  ;; Entry point
main:
  
  ;; 
  ;; Sanity checks
  ;;
  
  call vga_get_memory_map       ; Check VGA memory map value is 11
  cmp rax, 0b11
  jne .vga_memory_map_error
  
  call vga_is_alpha_disabled    ; Check VGA mode is alphanumeric
  cmp rax, 0
  jne .vga_alphanumeric_error

  mov r8w, UART_COM1            ; Initialize UART port COM1
  call uart_init_port
  cmp rax, 0
  jne .uart_init_error
  
  ;; Checks succesfull
  
  ;; Clean the screen
  mov r8b, vga_style_blue
  call vga_clear
  
  ;; Success message
  mov r10b, vga_style_blue      ; style
  mov r9, greet_string          ; string
  mov r8, 0                     ; position
  call vga_print

  ;; Uart message
  mov r8w, UART_COM1
  mov r9, greet_string
  call uart_write_string

  ;; Vga hex number
  mov r8, 19                    ; offset
  mov r9, 0x6969
  mov r10b, vga_style_blue
  call vga_print_hex

  
  ;; Divide by zero exception test
  ;;   mov rax, 10
  ;;   xor rdx, rdx
  ;;   mov rcx, 0
  ;;   idiv rcx

  ;; Triggers breakpoint exception
  ;;  int3
  
  .exit:
  ret
 
  .vga_memory_map_error:
  
  ;; Clean the screen
  mov r8b, vga_style_red        ; style
  call vga_clear
  
  mov r10b, vga_style_bw        ; style
  mov r9, vga_memory_map_error_string ; string
  mov r8, 0                     ; position
  call vga_print
  jmp .exit
  
  .vga_alphanumeric_error:
  
  ;; Clean the screen
  mov r8b, vga_style_red
  call vga_clear

  mov r10b, vga_style_bw        ; style
  mov r9, vga_alphanumeric_error_string ; string
  mov r8, 0                     ; position
  call vga_print
  jmp .exit

  .uart_init_error:

  ;; Clean the screen
  mov r8b, vga_style_red
  call vga_clear

  mov r10b, vga_style_bw        ; style
  mov r9, vga_uart_init_error_string ; string
  mov r8, 0                     ; position
  call vga_print
  jmp .exit

  
  section .data
  
greet_string: db `Hello, from povOS!`, 0
vga_uart_init_error_string:  db `Error initializing UART`, 0
