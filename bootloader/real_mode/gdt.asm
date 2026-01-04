  ;; -----------------------------------------------------------------
  ;; 
  ;; The GDT (real mode)
  ;; ===================
  ;;
  ;; Define the Flat Mode Configuration Global Descriptor Table (GDT)
  ;; The flat mode table allows us to read and write code anywhere,
  ;; without restriction.
  ;; 

  ;;
  ;; Table
  ;; -----
  ;;
  ;; The entries in the GDT are 8 bytes long and for a table like this:
  ;;
  ;;     + --------------------------- +
  ;;     | Address           | Content |
  ;;     | ================= | ======= |
  ;;     | GDTR Offset + 0   | Null    |
  ;;     | GDTR Offset + 8   | Entry 1 |
  ;;     | GDTR Offset + 16  | Entry 2 |
  ;;     | GDTR Offset + 24  | Entry 3 |
  ;;     | ...               | ...     |
  ;;     + --------------------------- +
  ;;
  ;; The first entry in the GDT should always be null. A single entry,
  ;; the segment descriptor, has the following structure:
  ;;
  ;;    0  - 15  Limit[0..15]: Maximum addressable unit
  ;;    16 - 39  Base[0..23]: the linear address where the segment begins
  ;;    40 - 47  Flags 1, Access type
  ;;    48 - 51  Limit[16..19]
  ;;    52 - 55  Flags 2
  ;;    56 - 63  Base[24..31]
  ;;    

  ;; each descriptor is 8 bytes, so we align 8
  align 8
  
gdt_32_start:

  ;; Define the null sector for the 32 bit gdt
  ;; Null sector is required for memory integrity check
gdt_32_null:
  dd 0x00000000
  dd 0x00000000

  ;; Define the code segment for the 32 bit gdt
gdt_32_code:
  ;; Base:   0x00000
  ;; Limit:  0xFFFFF
  ;; 1st Flags:        0b1001
  ;;   Present:        1
  ;;   Privelege:      00
  ;;   Descriptor:     1
  ;; Type Flags:       0b1010
  ;;   Executable:     1
  ;;   Conforming:     0
  ;;   Readable:       1
  ;;   Accessed:       0
  ;; 2nd Flags:        0b1100
  ;;   Granularity:    1
  ;;   32-bit Default: 1
  ;;   64-bit Segment: 0
  ;;   AVL:            0

  dw 0xFFFF                     ; Limit (bits 0-15)
  dw 0x0000                     ; Base (bits 0-15)
  db 0x00                       ; Base (bits 16-23)
  db 0b10011010                 ; 1st Flags, Type flags
  db 0b11001111                 ; 2nd Flags, Limit (bits 16-19)
  db 0x00                       ; Base (bits 24-31)

  ;; Define the data segment for the 32 bit gdt
gdt_32_data:
  ;; Base:   0x00000
  ;; Limit:  0xFFFFF
  ;; 1st Flags:        0b1001
  ;;   Present:        1
  ;;   Privelege:      00
  ;;   Descriptor:     1
  ;; Type Flags:       0b0010
  ;;   Executable:     0
  ;;   Exapnd Down:    0
  ;;   Writeable:      1
  ;;   Accessed:       0
  ;; 2nd Flags:        0b1100
  ;;   Granularity:    1
  ;;   32-bit Default: 1
  ;;   64-bit Segment: 0
  ;;   AVL:            0

  dw 0xFFFF                     ; Limit (bits 0-15)
  dw 0x0000                     ; Base (bits 0-15)
  db 0x00                       ; Base (bits 16-23)
  db 0b10010010                 ; 1st Flags, Type flags
  db 0b11001111                 ; 2nd Flags, Limit (bits 16-19)
  db 0x00                       ; Base (bits 24-31)

gdt_32_end:

  ;; 
  ;; The gdt register
  ;; ----------------
  ;; 
  ;; This is the structure that we pass to the CPU via the 'lgdt'
  ;; instruction which specifies the location of the GDT.
  ;;
  ;; Format:
  ;;
  ;;     + --------------------- +
  ;;     |  79 --- 16 | 15 --- 0 |
  ;;     | ========== | ======== |
  ;;     | offset     | size     |
  ;;     + --------------------- +
  ;;
  ;; Where size is the size of the table in bytes subtracted by 1, and
  ;; the offset is the linear address of the GDT (not phisical
  ;; address, paging pallies).
gdt_32_register:
  dw gdt_32_end - gdt_32_start -1 ; Size of GDT, one byte less than true size
  dd gdt_32_start

  ;; Define helpers to find pointers to Code and Data segments
code_seg: equ gdt_32_code - gdt_32_start
data_seg: equ gdt_32_data - gdt_32_start
