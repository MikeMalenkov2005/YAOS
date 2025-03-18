#ifndef KERNEL_ARCH_X86_TASK_H
#define KERNEL_ARCH_X86_TASK_H

#include <kernel/arch/x86/isr.h>
#include <kernel/arch/x86/fpu.h>
#include <kernel/task.h>

struct TASK_CONTEXT
{
  INTERRUPT_FRAME Frame;
  FPU_STATE FPUState;
};

void SaveTaskFrame(INTERRUPT_FRAME *pFrame);

void LoadTaskFrame(INTERRUPT_FRAME *pFrame);

#endif
