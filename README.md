![](./povos-banner.jpeg)

povOS is an operating system for the x86_64 architecture.

- Author:  Giovanni Santini
- Mail:    giovanni.santini@proton.me
- License: MIT

## Features

- custom bootloader
- custom standard library
- drivers:
  - ps2
  - uart
  - vga
  - pic (Programmable Interrupt Controller)
  - pit (Programmable Interrupt Timer)
  - keyboard
  - ACPI
- kernel:
  - IDT (Interrupt Descriptor Tabe) and ISR (Interrupt Service Routines)
  - input (supports multiple keyboard layouts)
  - textbuffer
  - framebuffer
  - console
  - tty
  - time tracking
  - memory management
    - physical memory management
    - paging (not dynamic yet, pages are allocated during boot)
    - virtual memory manager (TODO)

The implementation is clean and readable, headers are documentation.

## The boot sequence

The BIOS boot sequence for x86_64 looks like this:

 - CPU starts executing in 16-bit real mode, with BIOS access
 - Use the bios to load the rest of the bootloader and kernel
 - Setup and load the GDT with a flat memory layout
 - Go to protected mode
 - Enable the A20 line
 - Setup GDT again
 - Setup the page table
 - Enable long mode
 - Call the main routine
