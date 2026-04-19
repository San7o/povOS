// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_LAYOUT_H
#define POVOS_MM_LAYOUT_H

#include <libk/stddef.h>

// Higher-Half Direct Map
// You can safely map RAM after this offset, eg:
// virt_addr = MM_HHDM_OFFSET + phys_addr
// You have to create the paging mapping manually for now.
#define MM_HHDM_OFFSET      0xFFFF800000000000  // Where RAM is mirrored

// Higher half
#define KERNEL_BASE_ADDR    0xFFFFFFFF80000000

// Actual offset of higher-half kernel
// We need this dynamic global value since it is different during eary
// boot and normal execution
extern u64_t kernel_hhdm_offset;

#define PML4T_INDEX(addr)  (((addr) >> 39) & 511)
#define PDPT_INDEX(addr)   (((addr) >> 30) & 511)

#define MM_PHYS_TO_VIRT(phys)  ((void*)((u64_t)(phys) + kernel_hhdm_offset))
#define MM_VIRT_TO_PHYS(virt)  ((u64_t)(virt) - kernel_hhdm_offset)

#endif // POVOS_MM_MEMORY_H
