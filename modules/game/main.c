#include <sys/yaos.h>

#include "io.h"

char aWallTable[16] = /* 1^ 2> 4v 8< */
{
  0x00, 0xD0, 0xC6, 0xC8,
  0xD2, 0xBA, 0xC9, 0xCC,
  0xB5, 0xBC, 0xCD, 0xCA,
  0xBB, 0xB9, 0xCB, 0xCE
};

void EventLoop()
{
  int Event;
  while ((Event = NextKeyEvent()) != -1)
  {
    SetSymbol(39, 12, Event | 0x8000);
    SetSymbol(40, 12, Event | 0x8000);
    FlushSymbols();
  }
}

void Init()
{
  MESSAGE Message = { .ReceiverID = 1, .Payload = "\x07\x80\x0A\x0B" };
  if (SendMessage(&Message) == SYSRET_OK)
  {
    EventLoop();
  }
  Terminate();
}

