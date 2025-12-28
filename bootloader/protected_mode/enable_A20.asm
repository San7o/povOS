  ;; -----------------------------------------------------------------
  ;; 
  ;; The A20 line
  ;; ============
  ;;
  ;;  "The 8088 in the original PC had only 20 address lines, good for
  ;;  1 MB. The maximum address FFFF:FFFF addresses 0x10ffef, and this
  ;;  would silently wrap to 0x0ffef. When the 286 (with 24 address
  ;;  lines) was introduced, it had a real mode that was intended to
  ;;  be 100% compatible with the 8088. However, it failed to do this
  ;;  address truncation (a bug), and people found that there existed
  ;;  programs that actually depended on this truncation. Trying to
  ;;  achieve perfect compatibility, IBM invented a switch to
  ;;  enable/disable the 0x100000 address bit. Since the 8042 keyboard
  ;;  controller happened to have a spare pin, that was used to
  ;;  control the AND gate that disables this address bit. The signal
  ;;  is called A20, and if it is zero, bit 20 of all addresses is
  ;;  cleared."
  ;;
  ;;  Source:
  ;; 
  ;;   - https://aeb.win.tue.nl/linux/kbd/A20.html
  
[bits 32]
[section .text]

  ;; Set rax to 1 if A20 is on, or 0 otherwise
is_A20_on:
  pushad
  mov edi,0x112345              ; odd megabyte address.
  mov esi,0x012345              ; even megabyte address.
  mov [esi],esi                 ; making sure that both addresses
                                ; contain diffrent values.
  mov [edi],edi                 ; (if A20 line is cleared the two
                                ; pointers would point to the address
                                ; 0x012345 that would contain 0x112345
                                ; (edi))
  
  cmpsd                         ; compare addresses to see if the're
                                ; equivalent.
  popad
  jne .A20_on                   ; if not equivalent , A20 line is set.
    xor eax, eax                ; return 0
    ret                         ; if equivalent , the A20 line is
                                ; cleared.
  .A20_on:
    mov eax, 1                  ; return 1
    ret
  
  ;; Enable the A20 line
  ;; Implementation from OSdev https://wiki.osdev.org/A20_Line
enable_A20_protected:
  cli
  
  call .a20wait
  mov al,0xAD
  out 0x64,al

  call .a20wait
  mov al,0xD0
  out 0x64,al

  call .a20wait2
  in al,0x60
  push eax

  call .a20wait
  mov al,0xD1
  out 0x64,al

  call .a20wait
  pop  eax
  or al,2
  out 0x60,al

  call .a20wait
  mov al,0xAE
  out 0x64,al

  sti
  ret

  .a20wait:
  in al,0x64
  test al,2
  jnz .a20wait
  ret

  .a20wait2:
  in al,0x64
  test al,1
  jz .a20wait2
  ret
