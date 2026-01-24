  ;; Since we no longer have access to BIOS utilities, we use VGA
  ;; instead.

  [bits 32]

  ;; Simple 32-bit protected print routine
  ;; Message address stores in esi
print_protected:
  ;; The pusha command stores the values of all registers so we don't
  ;; have to worry about them. Use 'popa' to pop them before exiting.
  pusha
  mov edx, vga_start

  .loop:
    cmp byte[esi], 0            ; Check for NULL terminator
    je .done

    mov al, byte[esi]           ; Move character to al
    mov ah, vga_style_bw        ; Move style to ah

    mov word[edx], ax           ; Copy character to VGA framebuffer

    add esi, 1                  ; Increment string
    add edx, 2                  ; Increment VGA framebuffer cursor

    jmp .loop                   ; loop

  .done:
  popa
  ret
