// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_HEAP_H
#define POVOS_KERNEL_MM_HEAP_H

//
// Heap
// ====
//
// The global heap.
//

#include <kernel/mm/vmmgr.h>

bool  heap_init(vmmgr_t *vmmgr, size_t initial_size);

void *kmalloc(size_t size);
void  kfree(void *ptr);
void *kcalloc(size_t nmemb, size_t size);
void *krealloc(void *ptr, size_t size);
void *kreallocarray(void *ptr, size_t nmemb, size_t size);


#endif // POVOS_KERNEL_MM_HEAP_H
