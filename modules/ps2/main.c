#include "port.h"

#include <kernel/arch/x86/pio.h>
#include <sys/yaos.h>

static PS2_PORT Port;

void MessageLoop()
{
  MESSAGE Message = { 0 };
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    switch (Message.Payload[0])
    {
      case 0: /* Read */
        for (UINT i = 0; i < MESSAGE_SIZE; ++i) Message.Payload[i] = 0;
        Message.Payload[1] = ReadPort(&Port, Message.Payload + 2, MESSAGE_SIZE - 2);
        Message.ReceiverID = Message.SenderID;
        SendMessage(&Message);
        break;
      case 1: /* Write */
        break;
    }
  }
}

void Init()
{
  if (InitPort(&Port, 1))
  {
    for (;;)
    {
      MESSAGE Message = { .ReceiverID = 1 };
      Message.Payload[0] = 1;
      Message.Payload[1] = ReadPort(&Port, Message.Payload + 2, MESSAGE_SIZE - 2);
      if (Message.Payload[1]) SendMessage(&Message);
    }
    MessageLoop();
  }
  Terminate();
}

