#include <sys/yaos.h>

const static UINT8 Payload[] = "\x0A\x08\x01\x01\x07\x02\x01\x0ESystem Loaded!";

void Init()
{
  MESSAGE Message = { 0 };
  Message.ReceiverID = 1;
  for (UINT i = 0; i < sizeof(Payload); ++i) Message.Payload[i] = Payload[i];
  SendMessage(&Message);
  for(;;);
}

