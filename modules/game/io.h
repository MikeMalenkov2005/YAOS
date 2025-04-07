#ifndef INPUT_H
#define INPUT_H

int NextKeyEvent();

void SetSymbol(int X, int Y, int Symbol);

void FlushSymbols();

void PrintString(int X, int Y, unsigned int Length, const char *pString);

void PrintColoredString(int X, int Y, unsigned int Length, const char *pString, char Color);

#endif

