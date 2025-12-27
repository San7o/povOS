  ;; Since we no longer have access to BIOS utilities, this function
  ;; takes advantage of the VGA area.

  [bits 32]

  ;; Simple 32-bit protected print routine
  ;; Message address stores in esi
print_protected:
  ;; The pusha command stores the values of all registers so we don't
  ;; have to worry about them
  pusha
  mov edx, vga_start

  ;; Do main loop
  .print_protected_loop:
    ;; if char == \0, string is done
    cmp byte[esi], 0
    je .print_protected_done

    ;; Move character to al, style to ah
    mov al, byte[esi]
    mov ah, vga_style_bw

    ;; Print character to vga memory location
    mov word[edx], ax

    ;; Increment counter registers
    add esi, 1
    add edx, 2

    ;; Redo loop
    jmp .print_protected_loop

  .print_protected_done:
  ;; Popa does the opposite of pusha, and restores all the registers
  popa
  ret
