// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MM_FREE_LIST_ALLOC_H
#define POVOS_KERNEL_MM_FREE_LIST_ALLOC_H

//
// Free list allocator
// ===================
//
// Memory allocator based on free lists.
//
// Allocation is O(#allocated_chunks).
//

#include <libk/stddef.h>

// Maximum number of pointers. A limit is required since
// we need to keep track of what pointers were allocated and
// what parts of memory are free.
#ifndef FREE_LIST_ALLOC_MAX_NUM_CHUNKS
  #define FREE_LIST_ALLOC_MAX_NUM_CHUNKS 1024
#endif
  
typedef struct free_list_alloc_chunk {
  char*  start;
  size_t size;
} free_list_alloc_chunk_t;

typedef struct free_list_alloc_chunk_list {
  free_list_alloc_chunk_t chunks[FREE_LIST_ALLOC_MAX_NUM_CHUNKS];
  size_t len;
} free_list_alloc_chunk_list_t;

typedef struct free_list_alloc {
  char *mem;
  free_list_alloc_chunk_list_t free_chunks;
  // In this implementation we store the used chunks because we need
  // to know how bit was the allocated area. An alternative
  // implementation would be to store this information behind the
  // pointer.
  free_list_alloc_chunk_list_t used_chunks;
} free_list_alloc_t;

// O(1)
void free_list_alloc_init(free_list_alloc_t *fla, void* memory, size_t size);
// First fit. O(fla->free_chunks.len)
void *free_list_alloc_malloc(free_list_alloc_t *fla, size_t size);
// O(max(fla->free_chunks.len, ma->used_chunks.len)
void free_list_alloc_free(free_list_alloc_t *fla, void *ptr);
void *free_list_alloc_calloc(free_list_alloc_t *fla, size_t nmemb, size_t size);
void *free_list_alloc_realloc(free_list_alloc_t *fla, void *ptr, size_t size);
void *free_list_alloc_reallocarray(free_list_alloc_t *fla, void *ptr,
                                   size_t nmemb, size_t size);

#endif // POVOS_KERNEL_MM_FREE_LIST_ALLOC_H
