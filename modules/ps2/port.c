#include "port.h"

#include <kernel/arch/x86/pio.h>
#include <sys/yaos.h>

typedef struct HANDLER_INFO
{
  PS2_PORT *pPort;
  UINTPTR IRQ;
} HANDLER_INFO;

void PortHandlerThread()
{
  MESSAGE Message;
  if (WaitMessage(&Message) == SYSRET_OK)
  {
    HANDLER_INFO *pInfo = (void*)Message.Payload;
    while (WaitIRQ(pInfo->IRQ) == SYSRET_OK)
    {
      while (ReadPort8(0x64) & 1) /* While There are Bytes to Read */
      {
        UINT8 Byte = ReadPort8(0x60);
        if (~pInfo->pPort->Head)
        {
          pInfo->pPort->pBuffer[pInfo->pPort->Head++] = Byte;
          if (pInfo->pPort->Head == BUFFER_SIZE) pInfo->pPort->Head = 0;
          if (pInfo->pPort->Head == pInfo->pPort->Tail) pInfo->pPort->Head = ~0;
        }
        else
        {
          pInfo->pPort->pBuffer[pInfo->pPort->Tail++] = Byte;
          if (pInfo->pPort->Tail == BUFFER_SIZE) pInfo->pPort->Tail = 0;
        }
      }
    }
  }
  Terminate();
}

BOOL InitPort(PS2_PORT *pPort, UINTPTR IRQ)
{
  if (pPort && (pPort->pBuffer = MapMemory(NULL, BUFFER_SIZE, MAP_MEMORY_READABLE | MAP_MEMORY_WRITABLE)))
  {
    pPort->Head = 0;
    pPort->Tail = 0;
    UINT ThreadID = CreateThread(PortHandlerThread, 0);
    if (~ThreadID)
    {
      MESSAGE Message = { .ReceiverID = ThreadID };
      HANDLER_INFO *pInfo = (void*)Message.Payload;
      pInfo->pPort = pPort;
      pInfo->IRQ = IRQ;
      SendMessage(&Message);
      return TRUE;
    }
    (void)FreeMapping(pPort->pBuffer, BUFFER_SIZE);
    pPort->pBuffer = NULL;
  }
  return FALSE;
}

SIZE_T ReadPort(PS2_PORT *pPort, UINT8 *pBuffer, SIZE_T Size)
{
  for (SIZE_T i = 0; i < Size; ++i)
  {
    if (pPort->Tail == pPort->Head) return i;
    if (!~pPort->Head) pPort->Head = pPort->Tail;
    pBuffer[i] = pPort->pBuffer[pPort->Tail++];
    if (pPort->Tail == BUFFER_SIZE) pPort->Tail = 0;
  }
  return Size;
}

