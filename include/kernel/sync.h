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
// This headers includes:
//
// - semaphores
// - mutexes (binary semaphores)
// - spinlocks
//
//
// Synchronization Techniques
// --------------------------
//
// There are many ways you can make a data structure thread safe to
// consider, here is a quick overview:
// 
// - one big mutex: lock all reads and writes with a single
//   mutex. This will solve all concurrency problems, but will not
//   take advantage of multithreading
//
// - several big mutexes: devide the data structure in multiple
//   sections where each one is guarded by a mutex. Allows for some
//   parallelism and it is quite simple to implement.
//
// - read-writer locks: this allows for multiple readers and a single
//   writer. You have a lock that guards the count of active readers.
//   Readers take this lock when increasing / decreasing the counter.
//   Then you then have a writer lock, which is locked / unlocked by
//   the first reader when locking the readers lock. The writer only
//   writes if there are no readers, that is when the writer lock is
//   unlocked. A naive implementation could lead to writer-starvation
//   as it may wait indefinitely for the readers, robust approaches
//   would use condition varialbes (like std::condition_variable).
//
// - lock-free approaches: you use atomic operations on each element
//   of the data structure, which allows for multiple readers and
//   writers to the same elements. May not be practical in all cases.
//
// - RCU: used for read-mostly data structures. Readers keep a
//   reader-count on the data. When a writer writes something, it
//   copies the entire data structure, then modifies it and replaces
//   the pointer of the old one with the new one incrementing a
//   generation counter. Then, it waits for all readers to finish
//   reading the old generation and frees the old memory.
//   

#include <libk/stddef.h>
#include <libk/stdbool.h>

typedef struct semaphore {
  volatile u64_t count;
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
u64_t atomic_cmpxchg(volatile u64_t *a, u64_t b, u64_t cmp_val);

// Initializes semaphore count to [n]
void semaphore_init(semaphore_t *mu, u64_t n);
// decrements the value of the semaphore, and if the semaphore is
// negative, waits until the semaphore is released by the process
// holding it.
void semaphore_wait(semaphore_t *mu);
//  increments the semaphore and, if it is still negative, indicates
//  to the scheduler to wake the next waiting process in the queue.
void semaphore_signal(semaphore_t *mu);

void mutex_init(mutex_t *mu);
void mutex_lock(mutex_t *mu);               // blocking
bool mutex_try_lock(mutex_t *mu);           // non-blocking
void mutex_unlock(mutex_t *mu);

void spinlock_init(spinlock_t *sl);
void spinlock_lock(spinlock_t *sl);         // blocking
bool spinlock_try_lock(spinlock_t *sl);     // non-blocking
void spinlock_unlock(spinlock_t *sl);

#endif // POVOS_KERNEL_SYNC_H
