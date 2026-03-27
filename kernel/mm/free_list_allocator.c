// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/mm/free_list_allocator.h>   // implements

//
// Utility functions
//

// O(1)
static void
free_list_alloc_chunk_list_reset(free_list_alloc_chunk_list_t *chunk_list);
// O(1)
static free_list_alloc_chunk_t*
free_list_alloc_chunk_list_add(free_list_alloc_chunk_list_t *chunk_list,
                               void* start, size_t size);
// O(chunk_list->len) = O(FREE_LIST_ALLOC_MAX_NUM_CHUNKS)
// May invalidate previous pointers to its items
static void
free_list_alloc_chunk_list_remove(free_list_alloc_chunk_list_t *chunk_list,
                                  void* start);
static free_list_alloc_chunk_t*
free_list_alloc_chunk_list_get(free_list_alloc_chunk_list_t *chunk_list,
                               void* start);

//
// Implementation
//

void free_list_alloc_init(free_list_alloc_t *fla, void* memory, size_t size)
{
  if (!fla)
    return;
  free_list_alloc_chunk_list_reset(&fla->free_chunks);
  free_list_alloc_chunk_list_reset(&fla->used_chunks);
  free_list_alloc_chunk_list_add(&fla->free_chunks, memory, size);
  return;
}

void *free_list_alloc_malloc(free_list_alloc_t *fla, size_t size)
{
  if (!fla)
    goto exit;

  for (size_t i = 0; i < fla->free_chunks.len; ++i)
  {
    if (fla->free_chunks.chunks[i].size < size)
      continue;

    free_list_alloc_chunk_t *used_chunk =
      free_list_alloc_chunk_list_add(&fla->used_chunks,
                                     fla->free_chunks.chunks[i].start,
                                     size);
    if (!used_chunk)
      goto exit;
    
    fla->free_chunks.chunks[i].start = fla->free_chunks.chunks[i].start + size;
    fla->free_chunks.chunks[i].size  = fla->free_chunks.chunks[i].size - size;

    return (void*)used_chunk->start;
  }

 exit:
  return NULL;
}

void free_list_alloc_free(free_list_alloc_t *fla, void *ptr)
{
  if (!fla)
    goto exit;
  
  free_list_alloc_chunk_t *used_chunk =
    free_list_alloc_chunk_list_get(&fla->used_chunks, ptr);
  if (!used_chunk)
    goto exit;

  free_list_alloc_chunk_t *free_chunk_after = NULL;
  free_list_alloc_chunk_t *free_chunk_before = NULL;
  for (size_t i = 0; i < fla->free_chunks.len; ++i)
  {
    if (fla->free_chunks.chunks[i].start ==
        used_chunk->start + used_chunk->size)
      free_chunk_after = &fla->free_chunks.chunks[i];
    if (used_chunk->start ==
        fla->free_chunks.chunks[i].start + fla->free_chunks.chunks[i].size)
      free_chunk_before = &fla->free_chunks.chunks[i];
  }

  if (free_chunk_before && free_chunk_after)
  {
    free_chunk_before->size += used_chunk->size + free_chunk_after->size;
    free_list_alloc_chunk_list_remove(&fla->used_chunks, used_chunk->start);
    free_list_alloc_chunk_list_remove(&fla->free_chunks, free_chunk_before->start);
    goto exit;
  }
  if (free_chunk_before)
  {
    free_chunk_before->size += used_chunk->size;
    free_list_alloc_chunk_list_remove(&fla->used_chunks, used_chunk->start);
    goto exit;
  }
  if (free_chunk_after)
  {
    free_chunk_after->start = used_chunk->start;
    free_chunk_after->size += used_chunk->size;
    free_list_alloc_chunk_list_remove(&fla->used_chunks, used_chunk->start);
    goto exit;
  }

  free_list_alloc_chunk_list_add(&fla->free_chunks,
                                 used_chunk->start,
                                 used_chunk->size);
  free_list_alloc_chunk_list_remove(&fla->used_chunks, used_chunk->start);

 exit:
  return;
}

void *free_list_alloc_calloc(free_list_alloc_t *fla, size_t nmemb, size_t size)
{
  if (!fla)
    return NULL;

  char* mem = free_list_alloc_malloc(fla, size * nmemb);
  if (!mem)
    return NULL;

  for (size_t i = 0; i < nmemb * size; ++i)
    mem[i] = 0;

  return mem;
}

void *free_list_alloc_realloc(free_list_alloc_t *fla, void *ptr, size_t size)
{
  if (!fla)
    return NULL;

  if (ptr == NULL)
    return free_list_alloc_malloc(fla, size);

  free_list_alloc_chunk_t *used_chunk =
    free_list_alloc_chunk_list_get(&fla->used_chunks, ptr);
  if (!used_chunk)
    return NULL;
  
  char* mem = free_list_alloc_malloc(fla, size);
  if (!mem)
    return NULL;

  size_t min_size = (used_chunk->size < size) ? used_chunk->size : size;
  for (size_t i = 0; i < min_size; ++i)
    mem[i] = ((char*)ptr)[i];

  free_list_alloc_free(fla, ptr);
  return mem;
}

void *free_list_alloc_reallocarray(free_list_alloc_t *fla, void *ptr,
                                   size_t nmemb, size_t size)
{
  return free_list_alloc_realloc(fla, ptr, nmemb * size);
}

static free_list_alloc_chunk_t*
free_list_alloc_chunk_list_add(free_list_alloc_chunk_list_t *chunk_list,
                               void* start, size_t size)
{
  if (!chunk_list || chunk_list->len + 1 >= FREE_LIST_ALLOC_MAX_NUM_CHUNKS)
  {
    return NULL;
  }

  chunk_list->chunks[chunk_list->len] = (free_list_alloc_chunk_t){
    .start = start,
    .size = size,
  };
  chunk_list->len++;
  return &chunk_list->chunks[chunk_list->len - 1];
}

static void
free_list_alloc_chunk_list_remove(free_list_alloc_chunk_list_t *chunk_list,
                                  void* start)
{
  if (!chunk_list)
    return;
  
  size_t i = 0;
  while (i < chunk_list->len)
  {
    if (chunk_list->chunks[i].start == start)
      break;
    ++i;
  }
  if (i >= chunk_list->len)
    return;
  
  for (i = i+1; i < chunk_list->len; ++i)
  {
    chunk_list->chunks[i-1] = chunk_list->chunks[i];
  }
  chunk_list->len--;
  return;
}

static free_list_alloc_chunk_t*
free_list_alloc_chunk_list_get(free_list_alloc_chunk_list_t *chunk_list,
                               void* start)
{
  if (!chunk_list)
    return NULL;
  
  for (size_t i = 0; i < chunk_list->len; ++i)
    if (chunk_list->chunks[i].start == start)
      return &chunk_list->chunks[i];

  return NULL;
}

static void
free_list_alloc_chunk_list_reset(free_list_alloc_chunk_list_t *chunk_list)
{
  if (!chunk_list)
    return;
  chunk_list->len = 0;
  return;
}
