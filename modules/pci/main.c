#include "pci.h"
#include "sys/yaos.h"

void MessageLoop()
{
  MESSAGE Message;
  while (WaitMessage(&Message) == SYSRET_OK)
  {
  }
}

void Init()
{
  MESSAGE Message = { 0 };
  WaitMessage(&Message);
  Message.ReceiverID = 1;
  SendMessage(&Message);
  if (InitPCI())
  {
    MessageLoop();
  }
  Terminate();
}

