// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/sync.h>   // implements
#include <kernel/macros.h>

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
