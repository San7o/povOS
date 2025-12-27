  ;; -----------------------------------------------------------------
  ;;
  ;; Bootloader
  ;; ==========
  ;;
  ;; The classic boot sequence looks like this:
  ;;
  ;;  - CPU starts executing in 16-bit real mode, with BIOS access
  ;;  - Use the bios to load the rest of the bootloader
  ;;  - Setup and load the GDT with a flat memory layout
  ;;  - Go to protected mode
  ;;  - Setup GDT again
  ;;  - Setup the page table
  ;;  - Enable A20 line
  ;;  - Enable long mode
  ;;  - Call the main routine
  ;;
  
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

  ;; Load 3 sectors to load our entire bootloader.
  mov cx, 0x0003

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

msg_hello_world:    db `\r\nHello World, from the BIOS!\r\n`, 0
  
  ;; Boot drive storage
boot_drive: db 0x00

  ;; Pad boot sector for magic number
  times 510 - ($ - $$) db 0x00
  dw 0xAA55                     ; Magic number


  ;;
  ;; Begin second sector. This one contains 32-bit code only
  ;;

  ;;
  ;; Protected mode
  ;; 

bootsector_extended:
begin_protected:

[bits 32]

  ;; Clear vga memory output
  call clear_protected

  ;; Detect long mode
  ;; This function will erturn if there's no error
  call detect_lm_protected
  
  ;; Test vga-style print function
  mov esi, protected_alert
  call print_protected

  ;; Initialize the page table
  call init_pt_protected

  ;; Enable the A20 line
  call enable_A20_protected
  call is_A20_on
  cmp eax, 1
  jne .error_enabling_A20
  
  call elevate_protected

  .hang:
  
  jmp $                         ; Infinite loop

  .error_enabling_A20:
    mov esi, error_enabling_A20_string
    call print_protected
    jmp .hang
  
  ;; Include

  %include "protected_mode/clear.asm"
  %include "protected_mode/print.asm"
  %include "protected_mode/detect_lm.asm"
  %include "protected_mode/init_pt.asm"
  %include "protected_mode/gdt.asm"
  %include "protected_mode/enable_A20.asm"
  %include "protected_mode/elevate.asm"

  ;; Define necessary constants

kernel_start:  equ 0x00100000

error_enabling_A20_string:  db `Error enabling A20 line`, 0
protected_alert:  db `64-bit long mode supported`, 0

  ;; 
  ;; Long mode
  ;;
  
begin_long_mode:

  [bits 64]

  ;; Clean the screen
  mov rdi, vga_style_bw
  call clear_long

  ;; Long mode message
  mov rdi, vga_style_bw
  mov rsi, long_mode_note
  call print_long
  
  call main
  
  jmp $

  %include "long_mode/clear.asm"
  %include "long_mode/print.asm"
  %include "long_mode/print_hex.asm"
  %include "vga.asm"
  %include "main.asm"

long_mode_note:  db `Now running in fully-enabled, 64-bit long mode!`, 0
