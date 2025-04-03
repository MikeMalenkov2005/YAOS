#include "../arch.h"
#include "../port.h"
#include <kernel/arch/x86/pio.h>

inline static BOOL TimeoutWrite(UINT Timeout, UINT16 Port, UINT8 Byte)
{
  while (Timeout-- && (ReadPort8(0x64) & 2));
  if (ReadPort8(0x64) & 2) return FALSE;
  WritePort8(Port, Byte);
  return TRUE;
}

inline static BOOL WaitInput(UINT Timeout)
{
  while (Timeout-- && !(ReadPort8(0x64) & 1)) WritePort8(0x80, 0);
  return (ReadPort8(0x64) & 1);
}

UINT InitPS2()
{
  if (!TimeoutWrite(1024, 0x64, 0xAD) || !TimeoutWrite(1024, 0x64, 0xA7)) return 0;
  for (UINT Times = 1024; Times && (ReadPort8(0x64) & 1); --Times) (void)ReadPort8(0x60); /* Flush the Input Buffer */
  if (!TimeoutWrite(1024, 0x64, 0x20) || !WaitInput(1024)) return 0;
  UINT8 Config = ReadPort8(0x60) & 0x26;
  if (!TimeoutWrite(1024, 0x64, 0x60) || !TimeoutWrite(11024, 0x60, Config)) return 0;
  if (!TimeoutWrite(1024, 0x64, 0xAA) || !WaitInput(1024) || ReadPort8(0x60) != 0x55) return 0;
  UINT PortCount = 1;
  if (!TimeoutWrite(1024, 0x64, 0xA8) || !TimeoutWrite(1024, 0x64, 0x20) || !WaitInput(1024)) return 0;
  Config = ReadPort8(0x60);
  if (!(Config & 0x20))
  {
    if (!TimeoutWrite(1024, 0x64, 0xA7)) return 0;
    PortCount = 2;
    Config &= 4;
    if (!TimeoutWrite(1024, 0x64, 0x60) || !TimeoutWrite(1024, 0x60, Config)) return 0;
  }
  if (!TimeoutWrite(1024, 0x64, 0xAB) || !WaitInput(1024)) return 0;
  BOOLEAN bNoFirstPort = ReadPort8(0x60);
  if (PortCount == 2)
  {
    if (!TimeoutWrite(1024, 0x64, 0xA9) || !WaitInput(1024)) return 0;
    if (ReadPort8(0x60)) PortCount = 1;
  }
  if (bNoFirstPort && PortCount == 1) return 0;
  if (!bNoFirstPort && !TimeoutWrite(1024, 0x64, 0xAE)) return 0;
  if (PortCount == 2 && !TimeoutWrite(1024, 0x64, 0xA8)) return 0;
  Config |= (PortCount == 2 ? (bNoFirstPort ? 2 : 3) : 1);
  if (!TimeoutWrite(1024, 0x64, 0x60) || !TimeoutWrite(1024, 0x60, Config)) return 0;
  return PortCount;
}

BOOL SendByte(UINT PS2PortIndex, UINT8 Byte)
{
  if (PS2PortIndex > 1) return FALSE;
  if (PS2PortIndex && !TimeoutWrite(1024, 0x64, 0xD4)) return FALSE;
  return TimeoutWrite(1024, 0x60, Byte);
}

void HandlePortIRQs(PS2_PORT *pPort)
{
  UINTPTR IRQ = pPort->Index ? 12 : 1;
  while (WaitIRQ(IRQ) == SYSRET_OK)
  {
    while (ReadPort8(0x64) & 1) /* While There are Bytes to Read */
    {
      UINT8 Byte = ReadPort8(0x60);
      if (Byte && ~Byte)
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
  }
}
