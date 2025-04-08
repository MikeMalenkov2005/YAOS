#ifndef INPUT_H
#define INPUT_H

#include <types.h>

UINT NextKeyEvent();

void SetSymbol(UINT8 X, UINT8 Y, UINT16 Symbol);

void FlushSymbols();

void PrintString(UINT8 X, UINT8 Y, UINT Length, const char *pString);

void PrintColoredString(UINT8 X, UINT8 Y, UINT Length, const char *pString, UINT8 Color);

BOOL SendCommandBuffer(UINT Size, const char *pBuffer);

#endif

