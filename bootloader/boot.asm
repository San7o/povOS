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
  ;;  - Enable A20 line
  ;;  - Setup GDT again
  ;;  - Setup the page table
  ;;  - Enable long mode
  ;;  - Call the main routine
  ;;

  ;;
  ;; Real mode
  ;; ---------
  ;;
  ;; Program will start executing from 0x7C00 with BIOS. Only the first
  ;; boot sector is loaded in memory, and it must end with a magic
  ;; number 0xAA55. The bootloader needs to load more sectors to
  ;; continue execution.
  ;; 

  [org 0x7C00]                    ; Set program origin
  [bits 16]                       ; 16-bit Mode
  
  ;; Initialize the base pointer and the stack pointer
  ;; It's better to do it explicitly
  mov bp, 0x0500
  mov sp, bp

  ;; Before we do anything else, we want to save the ID of the boot
  ;; drive, which the BIOS stores in register dl. We can offload this
  ;; to a specific location in memory
  mov byte[boot_drive], dl

  mov bx, real_hello_str
  call bios_print

  ;; Load the next sector

  ;; The first sector's already been loaded, so we start with the
  ;; second sector the the drive (the numbering of sectors begins at 1).
  ;; Note: Only bl will be used
  mov bx, 0x0002

  ;; Set the number of sectors to load
  mov cx, [total_bytes]
  shr cx, 9                     ; divide by 512
  
  ;; Finally, we want to store the new sector immediately after the
  ;; first loaded sector, at address 0x7E00. This will help a lot with
  ;; jumping between different sectors of the bootloader.
  mov dx, 0x7E00

  call bios_load                ; load new sectors
  call real_elevate             ; elevate CPU to 32-bit mode
  
  .hang:
  jmp $                         ; infinite loop
  
  ;; 
  ;; Includes
  ;;

  %include "bootloader/real_mode/print.asm"
  %include "bootloader/real_mode/print_hex.asm"
  %include "bootloader/real_mode/load.asm"
  %include "bootloader/real_mode/gdt.asm"
  %include "bootloader/real_mode/elevate.asm"

real_hello_str:    db `\r\nHello World, from the BIOS!\r\n`, 0

total_bytes:                  ; calculate the size of the binary
  dq (sectors_end - sectors_start + 511)
boot_drive: db 0x00           ; boot drive storage, initialized at startup

  times 510 - ($ - $$) db 0x00  ; Pad boot sector for magic number
  dw 0xAA55                     ; Magic number


  ;;
  ;; Begin second sector
  ;;

  ;;
  ;; Protected mode
  ;; --------------
  ;;
  ;; We can use 32 bits, but we don't have access to the BIOS routines
  ;; anymore. We use VGA for printing.
  ;; 

  [bits 32]
  
sectors_start:                  ; used to get the number of sectors to load
bootsector_extended:
begin_protected_mode:

  ;; Enable the A20 line
  ;; The A20 line should be automatically enabled in modern BIOS
  call is_A20_on
  cmp eax, 1
  je .a20_enabled

  ;; Enable_A20_protected is untested
  call enable_A20_protected     ; Enable the A20 line
  call is_A20_on
  cmp eax, 1
  jne .error_enabling_A20

  .a20_enabled:

  ;; Detect long mode
  call detect_lm_protected
  cmp eax, 1
  jne .hang                     ; long mode not available
  
  ;; Test vga-style print function
  call clear_protected          ; clear the screen
  mov esi, protected_message
  call print_protected

  call init_pt_protected        ; initialize the page table
  call elevate_protected        ; set long mode

  .hang:
  jmp $                         ; Infinite loop

  .error_enabling_A20:
    call clear_protected
    mov esi, error_enabling_A20_string
    call print_protected
    jmp .hang
  
  ;; Include

  %include "bootloader/protected_mode/clear.asm"
  %include "bootloader/protected_mode/print.asm"
  %include "bootloader/protected_mode/detect_lm.asm"
  %include "bootloader/protected_mode/init_pt.asm"
  %include "bootloader/protected_mode/gdt.asm"
  %include "bootloader/protected_mode/enable_A20.asm"
  %include "bootloader/protected_mode/elevate.asm"

  ;; Constants

error_enabling_A20_string:  db `Error enabling A20 line`, 0
protected_message:  db `64-bit long mode supported`, 0

  ;; 
  ;; Long mode
  ;; ---------
  ;;
  ;; We can use 64 bits now.
  ;; 

  %include "bootloader/long_mode/print_hex.asm"
  %include "bootloader/long_mode/idt.asm"
  %include "drivers/vga.asm"
  %include "kernel/main.asm"

  section .text
  
begin_long_mode:

  [bits 64]

  cli                           ; disable interrupts. TODO: They
                                ; should be re-enabled once the IDT
                                ; has beed filled.
  call load_idt                 ; Load the interrupt descriptor table
  
  mov r9b, vga_style_bw         ; style
  call vga_clear                ; clean the screen

  ;; Long mode message
  mov r8, 0                     ; Offset from start
  mov r9, long_mode_message     ; String
  mov r10b, vga_style_bw        ; Style
  call vga_print

  call main

  .hang:
  jmp $

  section .data
  
long_mode_message:  db `Now running in fully-enabled, 64-bit long mode!`, 0
  
sectors_end:                    ; used to get the number of sectors to load
  
  ;; 
  ;; 
  ;; -----------------------------------------------------------------
