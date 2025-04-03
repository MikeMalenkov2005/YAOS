#include <sys/yaos.h>

SYSRET SendMessageSync(MESSAGE *pMessage)
{
  SYSRET SR = SendMessage(pMessage);
  if (SR == SYSRET_OK) SR = WaitMessage(pMessage);
  return SR;
}

void Init()
{
  SYSRET SR = SYSRET_OK;
  while (!IsError(SR))
  {
    MESSAGE Message = { .ReceiverID = 2, .Payload = "\x01\xFF" };
    if ((SR = SendMessageSync(&Message)) == SYSRET_OK && Message.Payload[1])
    {
      Message.ReceiverID = 1;
      while ((SR = SendMessage(&Message)) == SYSRET_FALSE);
    }
  }
}

