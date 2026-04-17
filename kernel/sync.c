// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sync.h>   // implements
#include <kernel/macros.h>
#include <kernel/utils.h>

void semaphore_init(semaphore_t *s, u64_t n)
{
  s->count = n;
}

void semaphore_wait(semaphore_t *s)
{
  u64_t current_count;
  
  while (1)
  {
    current_count = s->count;
        
    if (current_count > 0)
    {
      if (atomic_cmpxchg(&s->count, current_count - 1, current_count)
          == current_count)
        break; 
    }
    else
    {
      X86_PAUSE;
    }
  }
}

void semaphore_signal(semaphore_t *s)
{
  u64_t current_count;
  do {
    current_count = s->count;
  } while (atomic_cmpxchg(&s->count, current_count + 1, current_count)
           != current_count);
}

void mutex_init(mutex_t *mu)
{
  mu->count = 0;
}

void mutex_lock(mutex_t *mu)
{
  while (atomic_cmpxchg(&mu->count, 1, 0) == 1) { hcf(); }
}

bool mutex_try_lock(mutex_t *mu)
{
  return atomic_cmpxchg(&mu->count, 1, 0) == 0;
}

void mutex_unlock(mutex_t *mu)
{
  mu->count = 0;
}

void spinlock_init(spinlock_t *sl)
{
  sl->count = 0;
}

void spinlock_lock(spinlock_t *sl)
{
  while (atomic_cmpxchg(&sl->count, 1, 0) == 1) { X86_PAUSE; }
}

bool spinlock_try_lock(spinlock_t *sl)
{
  return atomic_cmpxchg(&sl->count, 1, 0) == 0;
}

void spinlock_unlock(spinlock_t *sl)
{
  sl->count = 0;
}
