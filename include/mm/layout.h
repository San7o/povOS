// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_LAYOUT_H
#define POVOS_MM_LAYOUT_H

//
// Memory layout
// =============
//
// This file contains values that define the virtual memory layout of
// the kernel.
//
// The virtual memory is setup like so:
//
//   Start                End                 Size     Usage
//   ---------------------------------------------------------------
//   0x0000000000000000 - 0xFFFF7FFFFFFFFFFF  128 TB   Userspace 
//   0xFFFF800000000000 - 0xFFFFFDFFFFFFFFFF  127 TB   RAM
//   0xFFFFFE0000000000 - 0xFFFFFE7FFFFFFFFF  512 GB   Page Tables
//   0xFFFFFE8000000000 - 0xFFFFFFFF7FFFFFFF  510 GB   Free
//   0xFFFFFFFF80000000 - 0xFFFFFFFFFFFFFFFF  2 GB     Kernel
// 

#include <libk/stddef.h>

// Higher-Half Direct Map
// RAM is mapped here. You can access RAM after this offset, eg:
// virt_addr = MM_HHDM_OFFSET + phys_addr
#define MM_HHDM_OFFSET      0xFFFF800000000000

// Higher half kernel
#define KERNEL_BASE_ADDR    0xFFFFFFFF80000000

// Recursive trick
//
// Our index of choice
#define RECURSIVE_SLOT 510UL
// The base prefix for the recursive window (Sign-extended for 64-bit)
#define RECURSIVE_BASE (0xFFFF000000000000ULL | (RECURSIVE_SLOT << 39))

// Actual offset of higher-half kernel
// We need this dynamic global value since it is different during eary
// boot and normal execution
extern u64_t kernel_hhdm_offset;

#define PML4T_INDEX(addr)  (((addr) >> 39) & 511)
#define PDPT_INDEX(addr)   (((addr) >> 30) & 511)

#define MM_PHYS_TO_VIRT(phys)  ((void*)((u64_t)(phys) + kernel_hhdm_offset))
#define MM_VIRT_TO_PHYS(virt)  ((u64_t)(virt) - kernel_hhdm_offset)

#endif // POVOS_MM_MEMORY_H
