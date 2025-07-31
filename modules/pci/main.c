#include <sys/yaos.h>
#include "pci.h"


void MessageLoop()
{
  MESSAGE Message;
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    UINT32 Result = 0;
    PCI_COMMAND *pCommand = (void*)Message.Payload;
    switch (pCommand->Command)
    {
      case PCI_COMMAND_READ_CONFIG:
        Result = ReadPCIDeviceConfig32(pCommand->Device, pCommand->Args[0]);
        Result >>= ((pCommand->Args[0] & 3) << 3);
        break;
    }
    PrepareResponse(&Message);
    *(UINT32*)Message.Payload = Result;
    SendMessage(&Message);
  }
}

void Init()
{
  /* TODO: Remove 4 lines */
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

