  ;; The PIC has two chips (Master and Slave). By default, they map
  ;; IRQs 0–15 to Interrupts 8–23. We need to move them to Interrupts
  ;; 32–47.

%define PIC1_COMMAND 0x20
%define PIC1_DATA    0x21
%define PIC2_COMMAND 0xA0
%define PIC2_DATA    0xA1

%define ICW1_INIT    0x11
%define ICW4_8086    0x01
  
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

    ;; Mask all interrupts (except maybe the ones you want)
    ;; For now, let's keep them all masked (0xFF)
    mov al, 0xFF
    out PIC1_DATA, al
    out PIC2_DATA, al

    pop rax
    ret
