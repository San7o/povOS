  [bits 16]

  ;; -----------------------------------------------------------------
  ;; Print a string in BIOS
  ;; Input pointer to string in bx
bios_print:
  push ax
  push bx

  ;; Enter Print Mode
  mov ah, 0x0E                  ; enter print mode

  .loop:
    cmp byte[bx], 0
    je .end

    mov al, byte[bx]
    int 0x10

    inc bx
    jmp .loop

  .end:
  pop bx
  pop ax
  ret
