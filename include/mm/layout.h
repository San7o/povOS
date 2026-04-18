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
#define MM_HHDM_OFFSET         0xFFFF800000000000  // Where RAM is mirrored

// Eventually we will want to move the kernel to the higher half
// This is not used yet
#define KERNEL_BASE_ADDR    0xFFFFFFFF80000000

#define MM_PHYS_TO_VIRT(phys)  ((void*)((u64_t)(phys) + MM_HHDM_OFFSET))
#define MM_VIRT_TO_PHYS(virt)  ((u64_t)(virt) - MM_HHDM_OFFSET)

#endif // POVOS_MM_MEMORY_H
