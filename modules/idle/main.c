#include <types.h>
#include <attrib.h>

#define SCREEN ((UINT16*)(void*)0xB8000)

const static char szMessage[] = "Idle loaded";

void Init()
{
  for (SIZE_T i = 0; i < sizeof(szMessage); ++i) SCREEN[i] = (UINT16)szMessage[i] | 0x200;
  for(;;);
}

