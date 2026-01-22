ENTRY(kernel_entry)

SECTIONS {
         . = 0x8400;

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
