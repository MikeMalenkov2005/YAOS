#include <kernel/panic.h>

#include <types.h>

void KernelPanic(const char *pError)
{
  UINT16 *pDebugDisplay = (void*)0xB8000;
  while (*pError) *(pDebugDisplay++) = (UINT16)*(pError++) | 0x400;
  for (;;);
}
