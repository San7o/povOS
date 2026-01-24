  [bits 32]

  ;; -----------------------------------------------------------------
  ;; Clear the VGA memory. (AKA write blank spaces to every character
  ;; slot). This function takes no arguments.
clear_protected:
  ;; The pusha command stores the values of all registers so we
  ;; don't have to worry about them
  pusha

  ;; Set up constants
  mov ebx, vga_extent
  mov ecx, vga_start
  mov edx, 0

  ;; Do main loop
  .loop:

    ;; While edx < ebx
    cmp edx, ebx
    jge .done

    ;; Free edx to use later
    push edx

    ;; Move character to al, style to ah
    mov al, ' '
    mov ah, vga_style_bw

    ;; Print character to VGA memory
    add edx, ecx
    mov word[edx], ax

    ;; Restore edx
    pop edx

    ;; Increment counter
    add edx, 2

    ;; GOTO beginning of loop
    jmp .loop

  .done:
  ;; Restore all registers and return
  popa
  ret
