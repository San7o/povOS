
  ;; 
  ;; Memory map
  ;; ----------
  ;; 
  ;; The memory layout looks like this:
  ;;
  ;; =================================================================
  ;; start     end       size   usage
  ;; -----------------------------------------------------------------
  ;; 0x80000 - 0xFFFFF   512 KB   BIOS reserved, VGA
  ;; 0x07E00 - 0x7FFFF   480 KB   rest of the bootloader and kernel 
  ;; 0x07DFF - 0x07C00   512 B    bootsector
  ;; 0x05401 - 0x07BFF   6 KB     stack
  ;; 0x05000 - 0x05400   1 KB     Memory Map
  ;; 0x01000 - 0x04FFF   20 KB    Page Tables
  ;; 0x00500 - 0x00FFF   2.7 KB   unused
  ;; 0x00000 - 0x004FF   1.24 KB  bios data area / real mode INT vector
  ;; ==================================================================
  ;;
  ;; The bootloader loads the kernel immediately after its own code
  ;; (at $BOOTLOADER_SECTORS * $SECTOR_SIZE). It maps the first
  ;; virtual 4MB of memory and the first virtual 4MB from $HIGHER_HALF
  ;; to the physical address starting at 0x0.
  ;;
  ;; It also sets up the recursive trick at PML4T[$RECURSIVE_SLOT].
  ;;

  %define $BOOTLOADER_SECTORS   5
  %define $SECTOR_SIZE          512
  %define $KERNEL_LOAD_ADDR     0x8600

  %define $REAL_MODE_STACK_BEGIN        0x7BFF
  %define $PROTECTED_MODE_STACK_BEGIN   0x90000

  %define $MMAP_ADDR          0x5000

  %define $RECURSIVE_SLOT     510
  %define $HIGHER_HALF        0xFFFFFFFF80000000
  
  %define $BOOTLOADER_START   0x7C00
  %define $CODE_ADDR          $BOOTLOADER_START + $SECTOR_SIZE
  %define $KERNEL_MAIN_ADDR   $HIGHER_HALF + $KERNEL_LOAD_ADDR

  %define $PML4T_ADDR         0x1000
  %define $PDPT_LOW_ADDR      $PML4T_ADDR + 0x1000
  %define $PDPT_HIGH_ADDR     $PDPT_LOW_ADDR + 0x1000
  %define $PDT_ADDR           $PDPT_HIGH_ADDR + 0x1000
  
