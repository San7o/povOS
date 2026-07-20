  global _port_outb
  global _port_outw
  global _port_outdw
  global _port_inb
  global _port_inw
  global _port_indw

  ;; 
  ;; out - x86_64
  ;; ------------
  ;; 
  ;; Copies the value from the second operand (source operand) to the
  ;; I/O port specified with the destination operand (first
  ;; operand). The source operand can be register AL, AX, or EAX,
  ;; depending on the size of the port being accessed (8, 16, or 32
  ;; bits, respectively); the destination operand can be a
  ;; byte-immediate or the DX register. Using a byte immediate allows
  ;; I/O port addresses 0 to 255 to be accessed; using the DX register
  ;; as a source operand allows I/O ports from 0 to 65,535 to be
  ;; accessed.

  ;; -----------------------------------------------------------------
  ;; Writes `sil` value into port `di`
_port_outb:

  push rax
  push rdx

  xor rax, rax
  xor rdx, rdx

  mov al, sil
  mov dx, di
  out dx, al

  pop rdx
  pop rax
  ret

  ;; -----------------------------------------------------------------
  ;; Writes `si` value into port `dx`
_port_outw:

  push rax
  push rdx

  xor rax, rax
  xor rdx, rdx

  mov ax, si
  mov dx, di
  out dx, ax

  pop rdx
  pop rax
  ret

  ;; -----------------------------------------------------------------
  ;; Writes `esi` value into port `dx`
_port_outdw:

  push rax
  push rdx

  xor rax, rax
  xor rdx, rdx

  mov eax, esi
  mov dx, di
  out dx, eax

  pop rdx
  pop rax
  ret


  ;; 
  ;; in - x86_64
  ;; -----------
  ;;
  ;; Copies the value from the I/O port specified with the second
  ;; operand (source operand) to the destination operand (first
  ;; operand). The source operand can be a byte-immediate or the DX
  ;; register; the destination operand can be register AL, AX, or EAX,
  ;; depending on the size of the port being accessed (8, 16, or 32
  ;; bits, respectively). Using the DX register as a source operand
  ;; allows I/O port addresses from 0 to 65,535 to be accessed; using
  ;; a byte immediate allows I/O port addresses 0 to 255 to be
  ;; accessed.

  ;; -----------------------------------------------------------------
  ;; Returns in `al` the value from port `di`
_port_inb:

  push rdx

  xor rax, rax
  xor rdx, rdx

  mov dx, di
  in al, dx

  pop rdx
  ret

  ;; -----------------------------------------------------------------
  ;; Returns in `ax` the value from port `di`
_port_inw:

  push rdx

  xor rax, rax
  xor rdx, rdx

  mov dx, di
  in ax, dx

  pop rdx
  ret


  ;; -----------------------------------------------------------------
  ;; Returns in `eax` the value from port `di`
_port_indw:

  push rdx

  xor rax, rax
  xor rdx, rdx

  mov dx, di
  in eax, dx

  pop rdx
  ret
