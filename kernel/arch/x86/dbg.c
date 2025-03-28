#include <types.h>

static SIZE_T PrintPos = 0;

void DebugPrint(UINT8 Prefix, UINTPTR Page)
{
  ((UINT16*)(void*)0xB8000)[160 + PrintPos++] = Prefix | 0x900;
  for (UINT k = 0; k < 8; ++k)
  {
    UINT16 Char = (Page >> ((7 - k) << 2)) & 0xF;
    Char += Char < 10 ? '0' : 'A' - 10;
    ((UINT16*)(void*)0xB8000)[160 + PrintPos++] = Char | 0x200;
  }
  ((UINT16*)(void*)0xB8000)[160 + PrintPos++] = 0;
}


