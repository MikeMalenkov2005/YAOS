#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock {
  int locked;
};

void spinlock_lock(struct spinlock* spinlock);
void spinlock_release(struct spinlock* spinlock);

#endif
