#ifndef KERNEL_ARCH_X86_FPU_H
#define KERNEL_ARCH_X86_FPU_H

#include <types.h>
#include <attrib.h>

typedef struct FPU_FLOAT
{
  UINT64 Significand; /* 1 bit integer and 63 bit fraction */
  UINT16 Exponent;    /* 1 bit sign and 15 bit exponent */
}__packed FPU_FLOAT;

typedef struct FPU_STATE
{
  UINT32 ControlWord;
  UINT32 StatusWord;
  UINT32 TagWord;
  UINT32 InstructionOffset;
  UINT16 InstructionSelector;
  UINT16 InstructionOpcode;
  UINT32 OperandOffset;
  UINT32 OperandSelector;
  FPU_FLOAT RegisterStack[8];
}__packed FPU_STATE;

void InitFPU();

void SaveFPU(FPU_STATE *pState);

void LoadFPU(FPU_STATE *pState);

#endif
