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
  ;; Graphics controller registers
  ;; ------------------------------
  ;; 
  %define VGA_GRAPHICS_ADDRESS_REGISTER     0x3CE
  %define VGA_GRAPHICS_DATA_REGISTER        0x3CF
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
  
  ;; Draw a pixel in vga graphics
  ;;   r8: x coordinate
  ;;   r9: y coordinate
  ;;   r10: color
vga_draw_pixel:

  ;; TODO
  
  ret

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

  ;; Set rax to 0b1 if alphanumeric mode is disabled, 0x0 otherwise
vga_get_alpha_disabled:
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
