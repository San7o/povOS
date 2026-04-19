ENTRY(kernel_entry)

KERNEL_PHYS = 0x8600;
KERNEL_VIRT = 0xFFFFFFFF80000000;

SECTIONS {
         . = KERNEL_VIRT + KERNEL_PHYS;

        .startup : AT(KERNEL_PHYS) {
               _start = .;
               *(.startup)
         }
         
         .text : AT(ADDR(.text) - KERNEL_VIRT) {
               *(.text*)
         }

         .rodata : AT(ADDR(.rodata) - KERNEL_VIRT) {
                 *(.rodata*)
         }

         .data : AT(ADDR(.data) - KERNEL_VIRT) {
               *(.data*)
         }

         .bss : AT(ADDR(.bss) - KERNEL_VIRT) {
              *(COMMON)
              *(.bss*)
         }
         _kernel_end = .;
}
