#include <kernel/arch/x86/fpu.h>

static BOOL NoFPU = 0x5A5A;

void InitFPU()
{
  UINT32 CR0;
  asm volatile ("mov %%cr0, %0" : "=a"(CR0));
  asm volatile ("mov %0, %%cr0" : : "a"((CR0 & ~(UINT32)0b1100) | 0b10));
  asm volatile ("fninit");
  asm volatile ("fnstsw %0" : "=m"(NoFPU) : : "memory");
  if (NoFPU)
  {
    asm volatile ("mov %%cr0, %0" : "=a"(CR0));
    asm volatile ("mov %0, %%cr0" : : "a"((CR0 & ~(UINT32)0b10) | 0b100));
  }
}

void SaveFPU(FPU_STATE *pState)
{
  if (!NoFPU) asm volatile ("fnsave %0" : "=m"(*pState) : : "memory");
}

void LoadFPU(FPU_STATE *pState)
{
  if (!NoFPU) asm volatile ("frstor %0" : : "m"(*pState) : "memory");
}
