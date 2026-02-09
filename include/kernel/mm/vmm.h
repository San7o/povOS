// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_VMM_H
#define POVOS_KERNEL_MM_VMM_H

//
// Virtual Memory Manager
// ----------------------
//

// TODO

#include <libk/stddef.h>

#define VM_FLAG_NONE    0
#define VM_FLAG_WRITE   (1 << 0)
#define VM_FLAG_EXEC    (1 << 1)
#define VM_FLAG_USER    (1 << 2)

typedef struct vm_object {
  void*    base;
  size_t   length;
  size_t   flags;
} vm_object_t;

#endif // POVOS_KERNEL_MM_VMM_H
