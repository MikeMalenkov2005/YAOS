#ifndef KERNEL_ARCH_X86_TASK_H
#define KERNEL_ARCH_X86_TASK_H

#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/fpu.h>
#include <kernel/task.h>

struct TASK_CONTEXT
{
  FPU_STATE FPUState;
  INTERRUPT_FRAME Frame;
};

void SetTaskFrame(INTERRUPT_FRAME *pFrame);

#endif
