// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sync.h>   // implements
#include <kernel/macros.h>
#include <kernel/utils.h>

void semaphore_init(struct semaphore *s, u64_t n)
{
  s->count = n;
}

void semaphore_wait(struct semaphore *s)
{
  u64_t current_count;
  
  while (1) {
    current_count = s->count;
        
    if (current_count > 0) {
      if (atomic_cmpxchg(&s->count, current_count - 1, current_count)
          == current_count)
        break; 
    } else {
      X86_PAUSE;
    }
  }
}

void semaphore_signal(struct semaphore *s)
{
  u64_t current_count;
  do {
    current_count = s->count;
  } while (atomic_cmpxchg(&s->count, current_count + 1, current_count)
           != current_count);
}

void mutex_init(struct mutex *mu)
{
  mu->count = 0;
}

void mutex_lock(struct mutex *mu)
{
  while (atomic_cmpxchg(&mu->count, 1, 0) == 1) {
    cpu_halt();
  }
}

bool mutex_try_lock(struct mutex *mu)
{
  return atomic_cmpxchg(&mu->count, 1, 0) == 0;
}

void mutex_unlock(struct mutex *mu)
{
  mu->count = 0;
}

void spinlock_init(struct spinlock *sl)
{
  sl->count = 0;
}

void spinlock_lock(struct spinlock *sl)
{
  while (atomic_cmpxchg(&sl->count, 1, 0) == 1){
    X86_PAUSE;
  }
}

bool spinlock_try_lock(struct spinlock *sl)
{
  return atomic_cmpxchg(&sl->count, 1, 0) == 0;
}

void spinlock_unlock(struct spinlock *sl)
{
  sl->count = 0;
}
