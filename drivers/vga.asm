  ;; -----------------------------------------------------------------
  ;; 
  ;; VGA driver
  ;; ==========
  ;; 
  ;; You use I/O ports to interact with the registers in an indexed
  ;; style (base + offset). By writing a byte to the Address Register
  ;; equal to the index of the particular sub-register you wish to
  ;; access, one can address the data pointed to by that index by
  ;; reading and writing the Data Register.
  ;; 
  ;; Really useful reference for registers:
  ;; 
  ;; - http://www.osdever.net/FreeVGA/vga/vga.htm
  ;;
  ;; 

  [bits 64]
  
  ;;
  ;; I/O Ports
  ;; ---------
  ;; 
  %define VGA_GRAPHICS_ADDRESS_REGISTER     0x3CE
  %define VGA_GRAPHICS_DATA_REGISTER        0x3CF
  
  ;; 
  ;; Graphics controller registers
  ;; ------------------------------
  ;;
  %define VGA_GRAPHICS_SET_RESET_REGISTER_INDEX        0x00
  %define VGA_GRAPHICS_ENABLE_SET_RESET_REGISTER_INDEX 0x01
  %define VGA_GRAPHICS_COLOR_COMPARE_REGISTER_INDEX    0x02
  %define VGA_GRAPHICS_DATA_ROTATE_REGISTER_INDEX      0x03
  %define VGA_GRAPHICS_READ_MAP_SELECT_REGISTER_INDEX  0x04
  %define VGA_GRAPHICS_MODE_REGISTER_INDEX             0x05
  %define VGA_GRAPHICS_MISC_REGISTER_INDEX             0x06
  %define VGA_GRAPHICS_COLOR_DONT_CARE_REGISTER_INDEX  0x07
  %define VGA_GRAPHICS_BIT_MASK_REGISTER_INDEX         0x08
  ;; 
  ;; Masks
  ;; -----
  ;; 
  ;; VGA_GRAHPICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK
  ;; 
  ;; This field specifies the range of host memory addresses that is
  ;; decoded by the VGA hardware and mapped into display memory
  ;; accesses.  The values of this field and their corresponding host
  ;; memory ranges are:
  ;; 
  ;;  00b -- A0000h-BFFFFh (128K region)
  ;;  01b -- A0000h-AFFFFh (64K region)
  ;;  10b -- B0000h-B7FFFh (32K region)
  ;;  11b -- B8000h-BFFFFh (32K region)
  ;; 
  %define VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK 0b1100
  ;;
  ;; GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK
  ;;
  ;; This bit controls alphanumeric mode addressing. When set to 1,
  ;; this bit selects graphics modes, which also disables the
  ;; character generator latches.
  ;; 
  %define VGA_GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK 0b1

  ;; 
  ;; Functions
  ;;

  section .text

  ;; -----------------------------------------------------------------
  ;; Draw a pixel in vga graphics
  ;;
  ;; Args:
  ;;   r8: x coordinate
  ;;   r9: y coordinate
  ;;   r10: color
vga_draw_pixel:

  ;; TODO
  
  ret

  ;; -----------------------------------------------------------------
  ;; Returns the range of host memory addresses decoded by the VGA
  ;; in rax, which can be one of the following values:
  ;;
  ;;  00b -- A0000h-BFFFFh (128K region)
  ;;  01b -- A0000h-AFFFFh (64K region)
  ;;  10b -- B0000h-B7FFFh (32K region)
  ;;  11b -- B8000h-BFFFFh (32K region)
  ;; 
vga_get_memory_map:
  push rdx

  ;; Select Graphics Controller Misc Register
  xor rax, rax
  mov dx, VGA_GRAPHICS_ADDRESS_REGISTER
  mov al, VGA_GRAPHICS_MISC_REGISTER_INDEX
  out dx, al

  ;; Read register value
  mov dx, VGA_GRAPHICS_DATA_REGISTER
  in al, dx

  ;; Read memory map field
  and al, VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK
  shr al, 2
  
  pop rdx
  ret

  ;; -----------------------------------------------------------------
  ;; Set rax to 1 if alphanumeric mode is disabled, 0 otherwise
vga_is_alpha_disabled:
  push rdx

  ;; Select Graphics Controller Misc Register
  xor rax, rax
  mov dx, VGA_GRAPHICS_ADDRESS_REGISTER
  mov al, VGA_GRAPHICS_MISC_REGISTER_INDEX
  out dx, al

  ;; Read register value
  mov dx, VGA_GRAPHICS_DATA_REGISTER
  in al, dx

  ;; Read alpha disabled field
  and al, VGA_GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK
  
  pop rdx
  ret

  ;; -----------------------------------------------------------------
  ;; Print character to screen
  ;;
  ;; Args:
  ;;   - r8: offset from start of vga framebuffer (aka character position)
  ;;   - r9b: character byte
  ;;   - r10b: character style
vga_print_char:
  push rdx
  push rax

  mov rdx, vga_start            ; Base address of VGA framebuffer
  add rdx, r8                   ; Calculate final address
  mov ax, r10w                  ; Save style in ah
  shl ax, 8
  mov al, r9b                   ; Save char in al
  mov word[rdx], ax

  pop rax
  pop rdx
  ret

  ;; -----------------------------------------------------------------
  ;; Clear the screen with color
  ;;
  ;; Args:
  ;;   r8b: style, should be one of vga_style_*
vga_clear:
  push rdi
  push rax
  push rcx

  ;; Move style to ah
  mov di, r8w
  shl di, 8
  mov ax, di
  
  mov al, ' '                   ; character to write
  mov rdi, vga_start            ; Starting address
  mov rcx, vga_extent / 2       ; Ending position

  rep stosw                     ; Repeats stosw the number of times
                                ; specified in the count register
                                ; stosw copies a character


  pop rcx
  pop rax
  pop rdi
  ret

  ;; -----------------------------------------------------------------
  ;; Print NULL-terminated string at offset with style
  ;;
  ;; Args:
  ;;   - r8: offset from start of vga framebuffer (aka character position)
  ;;   - r9: string address
  ;;   - r10b: style
vga_print:
  push r8
  push r9
  push r10
  push rax

  .print_long_loop:
    ;; if char == \0, string is done
    cmp byte[r9], 0
    je .print_long_done

    ;; Handle strings that are too long
    cmp r9, vga_start + vga_extent
    je .print_long_done

    push r9
    mov r9b, byte[r9]
    call vga_print_char
    pop r9

    add r9, 1                   ; Increment string address
    add r8, 2                   ; Increment VGA memory offset

    jmp .print_long_loop        ; Loop again

  .print_long_done:
  pop rax
  pop r10
  pop r9
  pop r8
  ret

vga_print_hex:

  ;; TODO

  ret
  
  ;; 
  ;; Constants
  ;;

  section .data
  
  ;; We assume VGA memory starts at 0xB8000. This can be checked with
  ;; vga_get_memory_map
vga_start:   equ 0x000B8000
  ;; vga text memory is 80 chars wide by 25 chars tall (one char is
  ;; 2 bytes)
vga_extent: equ 80 * 25 * 2

  ;; Colors
  
vga_style_blue:  equ 0x1F
vga_style_bw:    equ 0xF
vga_style_red:   equ 0x4F
   
  ;; Strings
  
vga_memory_map_error_string:  db `Memory map range not supported`, 0
vga_alphanumeric_error_string:  db `VGA alphanumeric mode is disabled`, 0
