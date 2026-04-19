// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_GDT_H
#define POVOS_KERNEL_GDT_H

//
// Global descriptor table
// =======================
//

#include <kernel/macros.h>
#include <libk/stddef.h>

typedef struct gdtr {
  u16_t limit;
  u64_t base;
} _packed gdtr_t;

// Reload the GDT once you changed virtual memory
void gdt_reload(void);
void gdt_flush(void); // implemented in assembly

#endif // POVOS_KERNEL_GDT_H
