  [bits 16]
  
  ;; -----------------------------------------------------------------
  ;; Print hex string in BIOS
  ;; Input in bx
bios_print_hex:
  push ax
  push bx
  push cx

  ;; Enable print mode
  mov ah, 0x0E

  ;; Print prefix
  mov al, '0'
  int 0x10
  mov al, 'x'
  int 0x10

  ;; Initialize cx as counter
  ;; 4 nibbles in 16-bits
  mov cx, 4

  ;; Begin loop
  .loop:
    ;; if cx==0 goto end
    cmp cx, 0
    je .end

    ;; Save bx again
    push bx

    ;; Shift so upper four bits are lower 4 bits
    shr bx, 12

    ;; Check to see if ge 10
    cmp bx, 10
    jge .alpha

      ;; Byte in bx now < 10
      ;; Set the zero char in al, add bl
      mov al, '0'
      add al, bl

      jmp .loop_end

  .alpha:

    ;; Bit is now greater than or equal to 10
    ;; Subtract 1- from bl to get add amount
    sub bl, 10

    ;; Move 'A' to al and add bl
    mov al, 'A'
    add al, bl

  .loop_end:

    int 0x10                    ; print character

    ;; Restore bx
    ;; Shift to next 4 bits
    pop bx
    shl bx, 4

    dec cx                      ; decrement counter
    jmp .loop                   ; loop

  .end:
  pop cx
  pop bx
  pop ax

  ret
