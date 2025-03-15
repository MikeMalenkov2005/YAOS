#include <kernel/timer.h>

#include <kernel/arch/x86/pio.h>

void InitTimer()
{
  UINT16 Divisor = (UINT16)(1193180 / TIMER_TICKS_PER_SECOND);
  WritePort8(0x43, 0x36);
  WritePort8(0x40, (UINT8)Divisor);
  WritePort8(0x40, (UINT8)(Divisor >> 8));
}
