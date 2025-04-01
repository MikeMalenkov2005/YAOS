#include <sys/yaos.h>

static MESSAGE Message =
{
  .SenderID = 0,
  .ReceiverID = 1,
  .Payload = "\x0A\x08\x01\x01\x07\x02\x01\x0ESystem Loaded!"
};

void Init()
{
  SendMessage(&Message);
  for(;;);
}

