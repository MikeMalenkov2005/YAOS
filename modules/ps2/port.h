#ifndef PS2_PORT_H
#define PS2_PORT_H

#include <types.h>

#define BUFFER_SIZE  0x1000

#define CMD_SIZE_INFO 0
#define CMD_MAIN_BYTE 1

#define CMD_TIMEOUT   16
#define CMD_RESPONSE  17

typedef struct PS2_COMMAND
{
  UINT SenderID;
  UINT8 Info[32];
} PS2_COMMAND;

typedef struct PS2_PORT
{
  UINT Head;
  UINT Tail;
  UINT8 *pBuffer;
  UINT Index;
  PS2_COMMAND Command;
} PS2_PORT;

UINT InitPS2();

BOOL InitPort(PS2_PORT *pPort);

SIZE_T ReadInputBuffer(PS2_PORT *pPort, UINT8 *pBuffer, SIZE_T Size);

BOOL SendCommand(PS2_PORT *pPort, const PS2_COMMAND *pCommand);

void ClearInputBuffer(PS2_PORT *pPort);

#endif

