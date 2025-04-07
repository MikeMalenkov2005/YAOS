#include "port.h"
#include "arch.h"

void ThreadInit()
{
  MESSAGE Message;
  if (WaitMessage(&Message) == SYSRET_OK)
  {
    HandlePortIRQs(*(PS2_PORT**)(void*)Message.Payload);
  }
  Terminate();
}

BOOL InitPort(PS2_PORT *pPort)
{
  if (pPort && (pPort->pBuffer = MapMemory(NULL, BUFFER_SIZE, MAP_MEMORY_READABLE | MAP_MEMORY_WRITABLE)))
  {
    pPort->Head = 0;
    pPort->Tail = 0;
    UINT ThreadID = CreateThread(ThreadInit, 0);
    if (~ThreadID)
    {
      MESSAGE Message = { .ReceiverID = ThreadID };
      *(PS2_PORT**)(void*)Message.Payload = pPort;
      SendMessage(&Message);
      return TRUE;
    }
    (void)FreeMapping(pPort->pBuffer, BUFFER_SIZE);
    pPort->pBuffer = NULL;
  }
  return FALSE;
}

SIZE_T ReadInputBuffer(PS2_PORT *pPort, UINT8 *pBuffer, SIZE_T Size)
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

BOOL SendCommand(PS2_PORT *pPort, const PS2_COMMAND *pCommand)
{
  if (!(pCommand->Info[CMD_SIZE_INFO] & 15) || (pPort->Command.Info[CMD_SIZE_INFO] & 15)) return FALSE;
  for (UINT8 i = 0; i < (pCommand->Info[CMD_SIZE_INFO] & 15); ++i)
  {
    if (!SendByte(pPort->Index, pCommand->Info[i + 1])) return FALSE;
  }
  pPort->Command = *pCommand;
  return TRUE;
}

void ClearInputBuffer(PS2_PORT *pPort)
{
  pPort->Head = 0;
  pPort->Tail = 0;
}

void ReceiveByte(PS2_PORT *pPort, UINT8 Byte)
{
  if (Byte == 0xFE)
  {
    MESSAGE Message = { .ReceiverID = 2 };
    Message.Payload[0] = (pPort->Index << 3);
    SendMessage(&Message);
  }
  else if (Byte == 0xFA || (pPort->Command.Info[CMD_RESPONSE] == 0xFA && (pPort->Command.Info[CMD_SIZE_INFO] >> 4)))
  {
    if (pPort->Command.Info[CMD_RESPONSE] == 0xFA)
    {
      UINT8 ResponseSize = pPort->Command.Info[CMD_SIZE_INFO] >> 4;
      pPort->Command.Info[CMD_SIZE_INFO] &= 15;
      pPort->Command.Info[CMD_SIZE_INFO] |= (ResponseSize - 1) << 4;
    }
    else 
    {
      pPort->Command.Info[CMD_SIZE_INFO] &= 0xF0;
      pPort->Command.Info[CMD_SIZE_INFO] |= pPort->Command.Info[CMD_SIZE_INFO] >> 4;
      pPort->Command.Info[CMD_TIMEOUT] = CMD_RESPONSE;
    }
    pPort->Command.Info[pPort->Command.Info[CMD_TIMEOUT]++] = Byte;
    if (!(pPort->Command.Info[CMD_SIZE_INFO] >> 4))
    {
      MESSAGE Message = { .ReceiverID = 2 };
      Message.Payload[0] = (pPort->Index << 3);
      SendMessage(&Message);
    }
  }
  else if (Byte && ~Byte)
  {
    if (~pPort->Head)
    {
      pPort->pBuffer[pPort->Head++] = Byte;
      if (pPort->Head == BUFFER_SIZE) pPort->Head = 0;
      if (pPort->Head == pPort->Tail) pPort->Head = ~0;
    }
    else
    {
      pPort->pBuffer[pPort->Tail++] = Byte;
      if (pPort->Tail == BUFFER_SIZE) pPort->Tail = 0;
    }
  }
}

