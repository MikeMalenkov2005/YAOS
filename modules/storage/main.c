#include <sys/yaos.h>
#include <sys/pci.h>

void InitPCIBlockDevice(PCI_DEVICE_INFO *pInfo)
{
  if (pInfo && pInfo->Class == 1)
  {
    switch (pInfo->Subclass)
    {
      case 1:
        break;
    }
  }
}

void MessageLoop()
{
  MESSAGE Message;
  while (WaitMessage(&Message) == SYSRET_OK)
  {
    switch (Message.Payload[MESSAGE_SIZE - 1])
    {
      case 0:
        InitPCIBlockDevice((void*)Message.Payload);
        break;
    }
  }
}

void Init()
{
  MessageLoop();
  Terminate();
}

