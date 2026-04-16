# OS Design

## Input

The input subsystem is the first subsystem I implemented. It is not
trivial at all because we need to handle multiple physical keyboards,
possibly using many different buses, with support for multiple
keyboard layouts and higher-level input events (you may want a
character stream instead of raw keycodes).

Here is the keyboard input stack, from the physical keyboard to the
display output:

![input](./input-subsystem.png)

## Virtual memory

Memory management is really tricky to work with, and a big part of it
the the virtual memory. We need a Virtual Memory Manager to allocate /
deallocate the virtual memory, then a Physical Memory Manager to do
the same for pages, and a translation mechanism implemented in
hardware (paging).

Here is a picture that visualizes this relationship:

![virtual memory diagram](./virtual-memory-diagram.png)

