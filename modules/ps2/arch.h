#ifndef PS2_ARCH_H
#define PS2_ARCH_H

#include <sys/yaos.h>

#include "port.h"

BOOL SendByte(UINT PS2PortIndex, UINT8 Byte);

void HandlePortIRQs(PS2_PORT *pPort);

#endif

