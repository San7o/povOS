  ;; -----------------------------------------------------------------
  ;; 
  ;; Programmable Interrupt Controller
  ;; =================================
  ;; 
  ;; The PIC has two chips (Master and Slave). By default, they map
  ;; IRQs 0–15 to Interrupts 8–23, but these are used as x86_64 CPU
  ;; exceptions! We need to move them to Interrupts 32–47.

  ;; Exported symbols
  global PIC1_COMMAND
  global PIC1_DATA
  global PIC2_COMMAND
  global PIC2_DATA
  global ICW1_INIT
  global ICW4_8086
  global pic_remap
  global pic_ack
    
PIC1_COMMAND:  equ 0x20
PIC1_DATA:     equ 0x21
PIC2_COMMAND:  equ 0xA0
PIC2_DATA:     equ 0xA1

ICW1_INIT:     equ 0x11
ICW4_8086:     equ 0x01

pic_remap:
  push rax

  ;; ICW1: Start initialization in cascade mode
  mov al, ICW1_INIT
  out PIC1_COMMAND, al
  out PIC2_COMMAND, al

  ;; ICW2: Set Vector Offsets
  mov al, 0x20        ; Master PIC offsets to 0x20 (Int 32)
  out PIC1_DATA, al
  mov al, 0x28        ; Slave PIC offsets to 0x28 (Int 40)
  out PIC2_DATA, al

  ;; ICW3: Tell Master PIC there is a slave at IRQ 2
  mov al, 4
  out PIC1_DATA, al
  ;; Tell Slave PIC its cascade identity (2)
  mov al, 2
  out PIC2_DATA, al

  ;; ICW4: Set mode to 8086
  mov al, ICW4_8086
  out PIC1_DATA, al
  out PIC2_DATA, al

  ;; Unmask Keyboard (IRQ 1)
  mov al, 0b11111101
  out PIC1_DATA, al     ; Send to Master PIC

  ;; Keep all Slave interrupts masked for now
  mov al, 0xFF          
  out PIC2_DATA, al     ; Send to Slave PIC
  
  pop rax
  ret

  ;; -----------------------------------------------------------------
  ;; Send acknowledgement to PIC
pic_ack:
  push rax
  
  mov al, 0x20                  ; 0x20 is the EOI command
  out PIC1_COMMAND, al          ; Send to Master PIC

  pop rax
  ret
