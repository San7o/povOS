  ;; -----------------------------------------------------------------
  ;;
  ;; Bootloader
  ;; ==========
  ;;
  ;; The x86_64 boot sequence looks like this:
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
  ;; Master Boot Record
  ;; ------------------
  ;;
  ;; Program will start executing from 0x7C00 with BIOS in real mode
  ;; (16 bits). Only the first boot sector (aka "Master Boot Record")
  ;; is loaded in memory, and it must end with a 64 bytes partition
  ;; table and a magic number 0xAA55. The bootloader needs to load
  ;; more sectors in order to continue execution.
  ;; 

  [org 0x7C00]                    ; Set program origin
  [bits 16]

start:
  jmp short begin_bootloader ; Workaround for some BIOSes that require this stub
  
  
  ;; Store the number of sectors to load for the kernel. This value
  ;; is set after compiling the kernel, it is up here to make it
  ;; easy to find. DO NOT CHANGE THE ADDRESS OF THE VALUE.
kernel_size:   db 0
  
  ;; Some BIOSes will do a funny and decide to overwrite bytes of code in
  ;; the section where a FAT BPB would be, potentially overwriting
  ;; bootsector code.
  ;; Avoid that by filling the BPB area with dummy values.
  ;; Some of the values have to be set to certain values in order
  ;; to boot on even quirkier machines.
  .bpb:
    times 3-($-$$) db 0
    .bpb_oem_id:            db "POVOS   "
    .bpb_sector_size:       dw 512
    .bpb_sects_per_cluster: db 0
    .bpb_reserved_sects:    dw 0
    .bpb_fat_count:         db 0
    .bpb_root_dir_entries:  dw 0
    .bpb_sector_count:      dw 0
    .bpb_media_type:        db 0
    .bpb_sects_per_fat:     dw 0
    .bpb_sects_per_track:   dw 18
    .bpb_heads_count:       dw 2
    .bpb_hidden_sects:      dd 0
    .bpb_sector_count_big:  dd 0
    .bpb_drive_num:         db 0
    .bpb_reserved:          db 0
    .bpb_signature:         db 0
    .bpb_volume_id:         dd 0
    .bpb_volume_label:      db "POVOS      "
    .bpb_filesystem_type:   times 8 db 0

begin_bootloader:               ; Stage 1
  
  ;; Initialize the base pointer and the stack pointer
  ;; It's better to do it explicitly

  cli                      ; Disable interrupts during setup
  cld
  jmp 0x0000:.initialize_cs
  .initialize_cs:
  xor ax, ax
  mov ds, ax               ; 2. Fixes DS: Ensure it matches ORG 0x7C00
  mov es, ax
  mov ss, ax
  mov sp, 0x7BFF           ; 3. Setup Stack just below bootloader
  sti                      ; Re-enable interrupts

  ;; Before we do anything else, we want to save the ID of the boot
  ;; drive, which the BIOS stores in register dl. We can store this to
  ;; a specific location in memory
  mov byte[boot_drive_id], dl

  mov ax, 0x0003                ; Make sure we are in text mode
  int 0x10
  
  mov bx, real_hello_str        ; Hello message
  call bios_print

  ;; Load the next sectors

  ;; The first sector's already been loaded, so we start with the
  ;; second sector the the drive (the numbering of sectors begins at
  ;; 1).  Note: Only bl will be used
  mov bx, 0x0002
  ;; Set the number of sectors to load
  mov cx, [bootloader_bytes]    ; the rest of the bootloader
  shr cx, 9                     ; divide by 512
  xor dx, dx
  add dl, byte[kernel_size]     ; the kernel
  add cx, dx

  ;; Finally, we want to store the new sector immediately after the
  ;; first loaded sector, at address 0x7E00 (0x7C00 + 512). This will
  ;; help a lot with jumping between different sectors of the
  ;; bootloader.
  mov dx, 0x7E00
  call bios_load                ; load bootloader sectors
  
  call real_elevate             ; elevate CPU to 32-bit mode
  
  .hang:
  jmp $                         ; infinite loop
  
  ;; 
  ;; Includes
  ;;

  %include "bootloader/x86_64/real_mode/print.asm"
  %include "bootloader/x86_64/real_mode/print_hex.asm"
  %include "bootloader/x86_64/real_mode/load.asm"
  %include "bootloader/x86_64/real_mode/gdt.asm"
  %include "bootloader/x86_64/real_mode/elevate.asm"

real_hello_str:    db `\r\nHello World, from the BIOS!\r\n`, 0
boot_drive_id: db 0x00          ; boot drive storage, initialized at
                                ; startup
bootloader_bytes:               ; calculate the size of the binary
                                ; WARNING: This implies that the size
                                ; can be at most 16 bits!
  dw (sectors_end - sectors_start + 511)
  
  times 446 - ($ - $$) db 0x00  ; Pad boot sector for magic number

  ;; MBR data
  ;; 
  ;; Reserve 64 bytes for the Partition Table (standard MBR)
  ;; This fills the space with zeros, effectively saying "no partitions"
partition_table:
  times 64 db 0x00
  dw 0xAA55                     ; Magic number


  ;;
  ;; Begin sector 2
  ;;
sectors_start: 
protected_mode_sector:

  ;;
  ;; Protected mode
  ;; --------------
  ;;
  ;; We can use 32 bits, but we don't have access to the BIOS routines
  ;; anymore. We use VGA for printing.
  ;; 

  [bits 32]

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
  
  %include "bootloader/x86_64/protected_mode/clear.asm"
  %include "bootloader/x86_64/protected_mode/print.asm"
  %include "bootloader/x86_64/protected_mode/detect_lm.asm"
  
  
  ;; Pad sector
  times 512 - ($ - protected_mode_sector) db 0x00
  
  ;; 
  ;; Begin sector 3
  ;;  
protected_mode_sector_utils:
  
  ;; Include

  %include "bootloader/x86_64/protected_mode/init_pt.asm"
  %include "bootloader/x86_64/protected_mode/gdt.asm"
  %include "bootloader/x86_64/protected_mode/enable_A20.asm"
  %include "bootloader/x86_64/protected_mode/elevate.asm"

  ;; Constants

error_enabling_A20_string:  db `Error enabling A20 line`, 0
protected_message:  db `64-bit long mode supported`, 0

  ;; Pad sector
  times 512 - ($ - protected_mode_sector_utils) db 0x00
  
  ;;
  ;; Begin sector 4
  ;;  
long_mode_sector: 

  ;; 
  ;; Long mode
  ;; ---------
  ;;
  ;; We can use 64 bits now.
  ;; 

  [bits 64]
  
  %include "bootloader/x86_64/long_mode/vga.asm"
  

begin_long_mode:

  mov r9b, vga_style_bw         ; style
  call vga_clear                ; clean the screen
  
  ;; Long mode message
  mov r8, 0                     ; Offset from start
  mov r9, long_mode_message     ; String
  mov r10b, vga_style_bw        ; Style
  call vga_print

  ;; Jump to main
  call kernel_main

  .hang:
  jmp $

kernel_main:   equ 0x8400       ; 0x7C00 + 512 * 4, where 4 is the
                                ; size of the bootloader
long_mode_message:  db `Now running in fully-enabled, 64-bit long mode!`, 0

sectors_end:                    ; used to calculate the amount of
                                ; sectors used by the bootloader

  ;; Pad sector
  times 512 - ($ - long_mode_sector) db 0x00

  ;; 
  ;; 
  ;; -----------------------------------------------------------------
