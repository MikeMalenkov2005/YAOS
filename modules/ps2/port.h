#ifndef PORT_H
#define PORT_H

#include <types.h>

#define BUFFER_SIZE  0x1000

typedef struct PS2_PORT
{
  UINT Head;
  UINT Tail;
  UINT8 *pBuffer;
} PS2_PORT;

BOOL InitPort(PS2_PORT *pPort, UINTPTR IRQ);

SIZE_T ReadPort(PS2_PORT *pPort, UINT8 *pBuffer, SIZE_T Size);

#endif

