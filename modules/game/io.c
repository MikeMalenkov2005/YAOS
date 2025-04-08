#include "io.h"

#include <sys/yaos.h>

inline static SYSRET SendMessageSync(MESSAGE *pMessage)
{
  SYSRET SR = SendMessage(pMessage);
  if (SR == SYSRET_OK) SR = WaitMessage(pMessage);
  return SR;
}

inline static BOOL NextKeyboardByte(UINT8 *pByte)
{
  static MESSAGE Message = { 0 };
  static UINT8 Position = 0;
  if (!pByte) return FALSE;
  while (Position >= Message.Payload[1])
  {
    Message.ReceiverID = 2;
    Message.Payload[0] = 1;
    Message.Payload[1] = 0xFF;
    SYSRET SR = SendMessageSync(&Message);
    if (SR == SYSRET_FALSE) Message.Payload[1] = 0;
    else if (IsError(SR)) return FALSE;
    Position = 0;
  }
  *pByte = Message.Payload[2 + Position++];
  return TRUE;
}

UINT NextKeyEvent()
{
  UINT Event = 0;
  UINT8 Byte = 0;
  while (!Byte) if (!NextKeyboardByte(&Byte)) return -1;
  if (Byte == 0xE0)
  {
    if (!NextKeyboardByte(&Byte)) return -1;
    if (Byte == 0xF0)
    {
      if (!NextKeyboardByte(&Byte)) return -1;
      Event = 0x500;
    }
    else Event = 0x100;
  }
  else if (Byte == 0xE1)
  {
for (UINT i = 0; i < 7; ++i) if (!NextKeyboardByte(&Byte)) return -1;
    Event = 0x700;
  }
  else if (Byte == 0xF0)
  {
    if (!NextKeyboardByte(&Byte)) return -1;
    Event = 0x400;
  }
  return Event | Byte;
}

void SetSymbol(UINT8 X, UINT8 Y, UINT16 Symbol)
{
  static MESSAGE Message = { .ReceiverID = 1 };
  static UINT SymbolIndex = 0;
  if (X < 80 && Y < 25)
  {
    UINT Index = SymbolIndex++ * 6;
    Message.Payload[Index++] = 8;
    Message.Payload[Index++] = X;
    Message.Payload[Index++] = Y;
    Message.Payload[Index++] = 6;
    Message.Payload[Index++] = Symbol >> 8;
    Message.Payload[Index++] = Symbol;
  }
  if (SymbolIndex == 5 || X >= 80 || Y >= 25)
  {
    SendMessage(&Message);
    SymbolIndex = 0;
  }
}

void FlushSymbols()
{
  SetSymbol(255, 255, 0);
}

void PrintString(UINT8 X, UINT8 Y, UINT Length, const char *pString)
{
  if (!pString || !Length) return;
  MESSAGE Message = { .ReceiverID = 1 };
  UINT Index = 0;
  Message.Payload[0] = 8;
  Message.Payload[1] = X;
  Message.Payload[2] = Y;
  Message.Payload[3] = 1;
  for (UINT i = 5; i < MESSAGE_SIZE && Index < Length; ++i) Message.Payload[i] = pString[Index++];
  Message.Payload[4] = Index;
  SendMessage(&Message);
  for (UINT Base = Index; Index < Length; Base = Index)
  {
    Message.Payload[0] = 1;
    for (UINT i = 2; i < MESSAGE_SIZE && Index < Length; ++i) Message.Payload[i] = pString[Index++];
    Message.Payload[1] = Index - Base;
    SendMessage(&Message);
  }
}

void PrintColoredString(UINT8 X, UINT8 Y, UINT Length, const char *pString, UINT8 Color)
{
  if (!pString || !Length) return;
  MESSAGE Message = { .ReceiverID = 1 };
  UINT Index = 0;
  Message.Payload[0] = 8;
  Message.Payload[1] = X;
  Message.Payload[2] = Y;
  Message.Payload[3] = 2;
  Message.Payload[4] = Color;
  for (UINT i = 6; i < MESSAGE_SIZE && Index < Length; ++i) Message.Payload[i] = pString[Index++];
  Message.Payload[5] = Index;
  SendMessage(&Message);
  for (UINT Base = Index; Index < Length; Base = Index)
  {
    Message.Payload[0] = 1;
    for (UINT i = 2; i < MESSAGE_SIZE && Index < Length; ++i) Message.Payload[i] = pString[Index++];
    Message.Payload[1] = Index - Base;
    SendMessage(&Message);
  }
}

BOOL SendCommandBuffer(UINT Size, const char *pBuffer)
{
  MESSAGE Message = { .ReceiverID = 1 };
  if (Size >= MESSAGE_SIZE) return FALSE;
  for (UINT i = 0; i < Size; ++i) Message.Payload[i] = pBuffer[i];
  return SendMessage(&Message) == SYSRET_OK;
}

