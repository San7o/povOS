
  global gdt_flush
  
gdt_flush:
    ; 0x08 is the offset of the Code Segment in the GDT. This jump
    ; reloads CS and clears the instruction pipeline
    push 0x08                   
    lea rax, [rel .reload_cs]
    push rax
    retfq                       ; Far Return (effectively a far jump)

.reload_cs:
    ; Reload data segments
    mov ax, 0x10                ; 0x10 is your Data Segment offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
