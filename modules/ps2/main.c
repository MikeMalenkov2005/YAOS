#include <sys/yaos.h>

#include "port.h"

static PS2_PORT Port;

void MessageLoop()
{
  MESSAGE Message = { 0 };
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    switch (Message.Payload[0] & 7)
    {
      case 0: /* Propogate / Resend */
        if (Port.Command.Info[CMD_RESPONSE] == 0xFA)
        {
          MESSAGE Message = { .ReceiverID = Port.Command.SenderID };
          Message.Payload[0] = 2;
          Message.Payload[1] = Port.Command.Info[CMD_SIZE_INFO];
          for (UINT8 i = 0; i < Message.Payload[1]; ++i)
          {
            Message.Payload[2 + i] = Port.Command.Info[CMD_RESPONSE + i];
          }
          SendMessage(&Message);
        }
        else
        {
          PS2_COMMAND Command = Port.Command;
          Port.Command = (PS2_COMMAND){ 0 };
          if (!(Command.Info[CMD_TIMEOUT]--) || !SendCommand(&Port, &Command))
          {
            for (UINT i = 1; i < MESSAGE_SIZE; ++i) Message.Payload[i] = 0;
            Message.ReceiverID = Message.SenderID;
            SendMessage(&Message);
          }
        }
        break;
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
          for (UINT i = CMD_MAIN_BYTE; i < CMD_RESPONSE; ++i)
          {
            Command.Info[i] = Message.Payload[i + 1];
          }
          Command.Info[CMD_TIMEOUT] = 3;
          if (Command.Info[CMD_MAIN_BYTE] == 0xEE || !SendCommand(&Port, &Command))
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

