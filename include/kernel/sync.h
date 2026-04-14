// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_SYNC_H
#define POVOS_KERNEL_SYNC_H

//
// Synchronization Primitives
// ==========================
//
// - semaphores
// - mutexes (binary semaphores)
// - spinlocks
//

#include <libk/stddef.h>
#include <libk/stdbool.h>

typedef struct semaphore {
  u64_t lock;
} semaphore_t;

// A mutex is a binary semaphore
typedef semaphore_t mutex_t;
// A spinlock loops the cpu while waiting for the lock to unlock
typedef mutex_t spinlock_t;

// Atomic compare and exchange
//
// Compares [a] to [camp_val], if they are equal, copies [b] to [a],
// otherwise returns [a]
// Note: this is implemented in assembly
u64_t atomic_cmpxchg(u64_t *a, u64_t b, u64_t cmp_val);

void mutex_init(mutex_t *mu);
void mutex_lock(mutex_t *mu);       // blocking
bool mutex_try_lock(mutex_t *mu);   // non-blocking
void mutex_unlock(mutex_t *mu);

void spinlock_init(spinlock_t *sl);
void spinlock_lock(spinlock_t *sl);       // blocking
bool spinlock_try_lock(spinlock_t *sl);   // non-blocking
void spinlock_unlock(spinlock_t *sl);

#endif // POVOS_KERNEL_SYNC_H
