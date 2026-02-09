ENTRY(kernel_entry)

SECTIONS {
         . = 0x8600;

        .startup : {
               _start = .;
               *(.startup)
         }
         
         .text : {
               *(.text*)
         }

         .rodata : {
                 *(.rodata*)
         }

         .data : {
               *(.data*)
         }

         .bss : {
              *(COMMON)
              *(.bss*)
         }
}
