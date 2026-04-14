// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sync.h>   // implements
#include <kernel/macros.h>
#include <kernel/utils.h>

void mutex_init(mutex_t *mu)
{
  mu->lock = 0;
}

void mutex_lock(mutex_t *mu)
{
  while (atomic_cmpxchg(&mu->lock, 1, 0) == 1) { halt_cpu(); }
}

bool mutex_try_lock(mutex_t *mu)
{
  return atomic_cmpxchg(&mu->lock, 1, 0) == 0;
}

void mutex_unlock(mutex_t *mu)
{
  mu->lock = 0;
}

void spinlock_init(spinlock_t *sl)
{
  sl->lock = 0;
}

void spinlock_lock(spinlock_t *sl)
{
  while (atomic_cmpxchg(&sl->lock, 1, 0) == 1) { X86_PAUSE; }
}

bool spinlock_try_lock(spinlock_t *sl)
{
  return atomic_cmpxchg(&sl->lock, 1, 0) == 0;
}

void spinlock_unlock(spinlock_t *sl)
{
  sl->lock = 0;
}
