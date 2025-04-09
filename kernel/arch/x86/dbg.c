#include <kernel/arch/x86/dbg.h>
#include <kernel/arch/x86/pio.h>

#define PORT 0x3F8

void InitDBG()
{
  WritePort8(PORT + 1, 0x00);
  WritePort8(PORT + 3, 0x80);
  WritePort8(PORT + 0, 0x03);
  WritePort8(PORT + 1, 0x00);
  WritePort8(PORT + 3, 0x03);
  WritePort8(PORT + 2, 0xC7);
  WritePort8(PORT + 4, 0x0B);
  WritePort8(PORT + 4, 0x0F);
}

UINT8 ReadByte()
{
  while (!(ReadPort8(PORT + 5) & 1));
  return ReadPort8(PORT);
}

void WriteByte(UINT8 Byte)
{
  while (!(ReadPort8(PORT + 5) & 0x20));
  WritePort8(PORT, Byte);
}

void PrintStringDBG(const char *pString)
{
  for (SIZE_T i = 0; pString[i]; ++i) WriteByte(pString[i]);
}

void PrintNumberDBG(UINTPTR Number, UINT8 Width, UINT8 Base)
{
  UINT8 szBuffer[256];
  UINT Position = sizeof(szBuffer);
  szBuffer[--Position] = 0;
  do
  {
    UINT8 Char = Number % Base;
    Number /= Base;
    Char += Char < 10 ? '0' : 'A' - 10;
    szBuffer[--Position] = Char;
  }
  while (Width ? --Width : Number);
  PrintStringDBG((char*)szBuffer + Position);
}

