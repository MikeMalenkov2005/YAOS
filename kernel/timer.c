#include <kernel/timer.h>
#include <kernel/task.h>

static UINTPTR TimerTicks = 0;

void HandleTimerTick()
{
  ++TimerTicks;
  SwitchTask(FALSE);
}

UINTPTR GetTimerTicks()
{
  return TimerTicks;
}
