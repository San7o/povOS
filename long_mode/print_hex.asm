  [bits 64]

  ;; Print number as Hex
  ;; Style stored in rdi, number stored in rsi
print_hex_long:
  push rax
  push rdx
  push rdi
  push rsi

  mov rdx, vga_start
  shl rdi, 8

  ;; Print '0x'
  mov r8, rdi                   ; Set style in r8
  add r8, '0'                   ; Set char in r8
  mov word[rdx], r8w            ; Print char to VGA memory location
  add rdx, 2                    ; Increment VGA cursor
  mov r8, rdi                   ; Set style in r8
  add r8, 'x'                   ; Set char to r8
  mov word[rdx], r8w            ; Print char to VGA memory location
  add rdx, 2                    ; Increment VGA cursor
  
  .print_hex_long_loop:
    ;; if rsi == 0 we are done
    cmp rsi, 0
    je .print_hex_long_done

    ;; Handle numbers that are too long
    cmp rdx, vga_start + vga_extent
    je .print_hex_long_done

    ;; Load high 4 bits of rsi into rax
    mov rax, rsi
    shr rax, 60                 ; Keep 4 bits only
    shl rsi, 4                  ; Discard the high 4 bits

    cmp rax, 10                 ; Check if number is alpha or numeric
    jge .print_hex_long_alpha
  
      ;; Move character to r8l, style to r8h
      mov r8, rdi
      add r8, '0'
      add r8, rax
 
      ;; Print character to vga memory location
      mov word[rdx], r8w
  
      jmp .print_hex_long_increment
  
    .print_hex_long_alpha:
 
      ;; Move character to r8l, style to r8h
      mov r8, rdi
      add r8, 'A'
      sub rax, 10
      add r8, rax 
 
      ;; Print character to vga memory location
      mov word[rdx], r8w
 
    .print_hex_long_increment:

    ;; Increment vga cursor
    add rdx, 2

    ;; Redo loop
    jmp .print_hex_long_loop

  .print_hex_long_done:
  pop rsi
  pop rdi
  pop rdx
  pop rax

  ret
