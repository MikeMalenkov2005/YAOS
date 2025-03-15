#include <kernel/timer.h>
#include <kernel/task.h>

static UINTPTR TimerTicks = 0;

void HandleTimerTick()
{
  ++TimerTicks;
  SwitchTask();
}

UINTPTR GetTimerTicks()
{
  return TimerTicks;
}
