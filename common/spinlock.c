#include <spinlock.h>

void spinlock_acquire(struct spinlock* spinlock) {
  while (__sync_lock_test_and_set(&spinlock->locked, 1));
}

void spinlock_release(struct spinlock* spinlock) {
  __sync_lock_release(&spinlock->locked);
}
