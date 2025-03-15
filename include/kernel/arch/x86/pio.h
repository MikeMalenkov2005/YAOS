#ifndef KERNEL_ARCH_X86_PIO_H
#define KERNEL_ARCH_X86_PIO_H

#include <types.h>

void WritePort8(UINT16 nPort, UINT8 nData);
void WritePort16(UINT16 nPort, UINT16 nData);
void WritePort32(UINT16 nPort, UINT32 nData);

UINT8 ReadPort8(UINT16 nPort);
UINT16 ReadPort16(UINT16 nPort);
UINT32 ReadPort32(UINT16 nPort);

#endif
