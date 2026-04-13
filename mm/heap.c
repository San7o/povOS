// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <mm/heap.h>   // implements
#include <mm/free_list_allocator.h>

static free_list_alloc_t kheap;

bool heap_init(vmmgr_t *vmmgr, size_t initial_size)
{
  void* ptr = (void*)vmm_alloc(vmmgr, initial_size, VMMGR_FLAG_WRITE);
  if (!ptr) return false;
      
  free_list_alloc_init(&kheap, ptr, initial_size);
  
  return true;
}

void *kmalloc(size_t size)
{
  return free_list_alloc_malloc(&kheap, size);
}

void kfree(void *ptr)
{
  free_list_alloc_free(&kheap, ptr);
  return;
}

void *kcalloc(size_t nmemb, size_t size)
{
  return free_list_alloc_calloc(&kheap, nmemb, size);
}

void *krealloc(void *ptr, size_t size)
{
  return free_list_alloc_realloc(&kheap, ptr, size);
}

void *kreallocarray(void *ptr, size_t nmemb, size_t size)
{
  return free_list_alloc_reallocarray(&kheap, ptr, nmemb, size);
}
