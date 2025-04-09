#ifndef KERNEL_ARCH_X86_DBG_H
#define KERNEL_ARCH_X86_DBG_H

#include <types.h>

void InitDBG();

void PrintStringDBG(const char *pString);

void PrintNumberDBG(UINTPTR Number, UINT8 Width, UINT8 Base);

#endif

