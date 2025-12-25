  [org 0x7C00]                    ; Set program origin
  [bits 16]                       ; 16-bit Mode

  ;; Initialize the base pointer and the stack pointer
  ;; The initial values should be fine for what we've done so far,
  ;; but it's better to do it explicitly
  mov bp, 0x0500
  mov sp, bp

  ;; Before we do anything else, we want to save the ID of the boot
  ;; drive, which the BIOS stores in register dl. We can offload this
  ;; to a specific location in memory
  mov byte[boot_drive], dl

  mov bx, msg_hello_world
  call print_bios

  ;; Load the next sector

  ;; The first sector's already been loaded, so we start with the
  ;; second sector the the drive (the numbering of sectors begins at 1).
  ;; Note: Only bl will be used
  mov bx, 0x0002

  ;; We only want to load one sector from the disk for now. This will
  ;; be higher later. NoteL Only cl will be used
  mov cx, 0x0001

  ;; Finally, we want to store the new sector immediately after the
  ;; first loaded sector, at address 0x7E00. This will help a lot with
  ;; jumping between different sectors of the bootloader.
  mov dx, 0x7E00

  ;; Now we're fine to load the new sectors
  call load_bios

  ;; And elevate our CPU to 32-bit mode
  call elevate_bios

  ;; Infinite Loop
bootsector_hold:
  jmp $

  ;; 
  ;; Includes
  ;;

  %include "real_mode/print.asm"
  %include "real_mode/print_hex.asm"
  %include "real_mode/load.asm"
  %include "real_mode/gdt.asm"
  %include "real_mode/elevate.asm"

  ;;
  ;; Data storage area
  ;;
  
msg_hello_world:    db `\r\nHello World, from the BIOS!\r\n`, 0
  
  ;; Boot drive storage
boot_drive: db 0x00

  ;; Pad boot sector for magic number
  times 510 - ($ - $$) db 0x00
  dw 0xAA55                     ; Magic number


  ;; 
  ;; Begin second sector. This one contains 32-bit code only
  ;;
  
bootsector_extended:
begin_protected:

  ;; Clear vga memory output
  call clear_protected

  ;; Test vga-style print function
  mov esi, protected_alert
  call print_protected

  jmp $                         ; Infinite loop

  ;; Include

  %include "protected_mode/clear.asm"
  %include "protected_mode/print.asm"

  ;; Define necessary constants
vga_start:   equ 0x000B8000
  ;; vga memory is 80 chars wide by 25 chars tall (one char is 2 bytes)
vga_extent: equ 80*25*2
style_wb: equ 0x0F

protected_alert:  db `Now in 32-bit protected mode`, 0
  
  ;; Fill with zeros to the end of the sector
  times 512 - ($ - bootsector_extended) db 0x00
