// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_MM_FREE_LIST_ALLOC_H
#define POVOS_MM_FREE_LIST_ALLOC_H

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

struct free_list_alloc_chunk {
  char*  start;
  size_t size;
};

#define FREE_LIST_ALLOC_CHUNK_MAKE(_start, _size) \
  (struct free_list_alloc_chunk) { .start = _start, .size = _size }

struct free_list_alloc_chunk_list {
  struct free_list_alloc_chunk chunks[FREE_LIST_ALLOC_MAX_NUM_CHUNKS];
  size_t len;
};

struct free_list_alloc {
  char *mem;
  struct free_list_alloc_chunk_list free_chunks;
  // In this implementation we store the used chunks because we need
  // to know how bit was the allocated area. An alternative
  // implementation would be to store this information behind the
  // pointer.
  struct free_list_alloc_chunk_list used_chunks;
};

// O(1)
void free_list_alloc_init(struct free_list_alloc *fla, void* memory, size_t size);
// First fit. O(fla->free_chunks.len)
void *free_list_alloc_malloc(struct free_list_alloc *fla, size_t size);
// O(max(fla->free_chunks.len, ma->used_chunks.len)
void free_list_alloc_free(struct free_list_alloc *fla, void *ptr);
void *free_list_alloc_calloc(struct free_list_alloc *fla, size_t nmemb, size_t size);
void *free_list_alloc_realloc(struct free_list_alloc *fla, void *ptr, size_t size);
void *free_list_alloc_reallocarray(struct free_list_alloc *fla, void *ptr,
                                   size_t nmemb, size_t size);

#endif // POVOS_MM_FREE_LIST_ALLOC_H
