  ;; -----------------------------------------------------------------
  ;;
  ;; Global Descriptor Table (protected mode)
  ;; ========================================
  ;; 
  ;; Define the Flat Mode Configuration Global Descriptor Table (GDT).
  ;; The flat mode table allows us to read and write code anywhere,
  ;; without restriction
  ;;
  ;; Base, limit and granularity are ignored in long mode.
  ;; 
  ;; For more information about the GDT, read the code for the GDT for
  ;; real mode.
  ;; 

  align 8

gdt_64_start:

  ;; Define the null sector for the 64 bit gdt
  ;; Null sector is required for memory integrity check
gdt_64_null:
  dd 0x00000000
  dd 0x00000000

  ;; Define the code sector for the 64 bit gdt
gdt_64_code:
  ;; Base:     0x00000 (ignored)
  ;; Limit:    0x00000 (ignored)
  ;; 1st Flags:        0b1001
  ;;   Present:        1
  ;;   Privelege:      00
  ;;   Descriptor:     1
  ;; Type Flags:       0b1010
  ;;   Executable:     1
  ;;   Conforming:     0
  ;;   Readable:       1
  ;;   Accessed:       0
  ;; 2nd Flags:        0b0010
  ;;   Granularity:    0
  ;;   32-bit Default: 0
  ;;   64-bit Segment: 1
  ;;   AVL:            0

  dw 0x0000                     ; Limit (bits 0-15)
  dw 0x0000                     ; Base (bits 0-15)
  db 0x00                       ; Base (bits 16-23)
  db 0b10011010                 ; 1st Flags, Type flags
  db 0b00100000                 ; 2nd Flags, Limit (bits 16-19)
  db 0x00                       ; Base (bits 24-31)

  ;; Define the data sector for the 64 bit gdt
gdt_64_data:
  ;; Base:    0x00000 (ignored)
  ;; Limit:   0x00000 (ignored)
  ;; 1st Flags:        0b1001
  ;;   Present:        1
  ;;   Privelege:      00
  ;;   Descriptor:     1
  ;; Type Flags:       0b0010
  ;;   Executable:     0
  ;;   Exapnd Down:    0
  ;;   Readable:       1
  ;;   Accessed:       0
  ;; 2nd Flags:        0b0000
  ;;   Granularity:    0
  ;;   32-bit Default: 0
  ;;   64-bit Segment: 0 (must be 0 for data)
  ;;   AVL:            0

  dw 0x0000                     ; Limit (bits 0-15)
  dw 0x0000                     ; Base (bits 0-15)
  db 0x00                       ; Base (bits 16-23)
  db 0b10010010                 ; 1st Flags, Type flags
  db 0b00000000                 ; 2nd Flags, Limit (bits 16-19)
  db 0x00                       ; Base (bits 24-31)

gdt_64_end:

  ;; Define the gdt descriptor
  ;; This data structure gives cpu length and start address of gdt
  ;; We will feed this structure to the CPU in toder to set the
  ;; protected mode GDT
gdt_64_descriptor:
  dw gdt_64_end - gdt_64_start -1 ; Size of GDT, one byte less than true size
  dq gdt_64_start                 ; Start of the 64 bit gdt

  ;; Define helpers to find pointers to Code and Data segments
code_seg_64:  equ gdt_64_code - gdt_64_start
data_seg_64:  equ gdt_64_data - gdt_64_start
