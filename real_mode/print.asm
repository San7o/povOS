  [bits 16]

  ;; Input pointer to string in bx
print_bios:
  ;; Save state
  push ax
  push bx

  ;; Enter Print Mode
  mov ah, 0x0E

  print_bios_loop:
    cmp byte[bx], 0
    je print_bios_end

    mov al, byte[bx]
    int 0x10

    inc bx
    jmp print_bios_loop

print_bios_end:
  pop bx
  pop ax
  ret
