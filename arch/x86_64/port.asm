  global port_outb

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
port_outb:
  
  push rax
  push rdx

  mov al, sil
  mov dx, di
  out dx, al

  pop rdx
  pop rax
  ret
