#include <kernel/arch/x86/pio.h>
#include <sys/yaos.h>

#include "port.h"

static PS2_PORT Port;

void MessageLoop()
{
  MESSAGE Message = { 0 };
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    switch (Message.Payload[0])
    {
      case 1: /* Read Input Buffer (Size < 31) */
        for (UINT i = 2; i < MESSAGE_SIZE; ++i) Message.Payload[i] = 0;
        Message.Payload[1] = ReadInputBuffer(&Port, Message.Payload + 2, Message.Payload[1] < MESSAGE_SIZE - 2 ? Message.Payload[1] : MESSAGE_SIZE - 2);
        Message.ReceiverID = Message.SenderID;
        SendMessage(&Message);
        break;
      case 2: /* Send Command (Response) */
        {
          PS2_COMMAND Command = { .SenderID = Message.SenderID };
          Command.Info[CMD_SIZE_INFO] = Message.Payload[1];
          for (UINT i = CMD_MAIN_BYTE; i < sizeof(Command.Info); ++i)
          {
            Command.Info[i] = Message.Payload[i + 1];
          }
          if (!SendCommand(&Port, &Command))
          {
            for (UINT i = 1; i < MESSAGE_SIZE; ++i) Message.Payload[i] = 0;
            Message.ReceiverID = Message.SenderID;
            SendMessage(&Message);
          }
        }
        break;
      case 3: /* Clear Input Buffer */
        ClearInputBuffer(&Port);
        break;
    }
  }
}

void Init()
{
  Port.Index = 0;
  if (InitPS2() && InitPort(&Port))
  {
    MessageLoop();
  }
  Terminate();
}

