  ;; -----------------------------------------------------------------
  ;;
  ;; OS entrypoint
  ;; =============
  ;;
  ;; The bootloader just loaded you in x86_64 long (protected) mode
  ;; with a flat memroy layout. You are ring 0, you can do
  ;; anything. You don't have access to the fancy BIOS routines
  ;; anymore, so you will have to to get creative and implement your
  ;; own drivers, but you have a full 64-bit address space to play
  ;; with.
  ;;
  ;; Check out vga.asm for an example driver.
  ;; 

  [bits 64]

  section .text
  
  ;; Entry point
main:
  
  ;; 
  ;; Sanity checks
  ;;
  
  call vga_get_memory_map       ; Check VGA memory map value is 01
  cmp rax, 0b11
  jne .main_vga_memory_map_error
  
  call vga_get_alpha_disabled   ; Check VGA mode is alphanumeric
  cmp rax, 0
  jne .main_vga_alphanumeric_error
  
  ;; Checks succesfull
  
  ;; Clean the screen
  mov rdi, vga_style_blue
  call clear_long
  
  ;; Success message
  mov rdi, vga_style_blue
  mov rsi, greet_string
  call print_long
  
  .main_exit:
  ret
 
  .main_vga_memory_map_error:
  
  ;; Clean the screen
  mov rdi, vga_style_red
  call clear_long
  
  mov rdi, vga_style_bw
  mov rsi, vga_memory_map_error_string
  call print_long
  jmp .main_exit
  
  .main_vga_alphanumeric_error:
  
  ;; Clean the screen
  mov rdi, vga_style_red
  call clear_long

  mov rdi, vga_style_bw
  mov rsi, vga_alphanumeric_error_string
  call print_long
  jmp .main_exit

  
  section .data
  
greet_string: db `Hello, from povOS!`, 0
