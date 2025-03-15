#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <types.h>

#define TIMER_TICKS_PER_SECOND 1000

void InitTimer();

void HandleTimerTick();

UINTPTR GetTimerTicks();

#endif
