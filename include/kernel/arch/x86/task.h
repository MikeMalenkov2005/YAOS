#ifndef KERNEL_ARCH_X86_TASK_H
#define KERNEL_ARCH_X86_TASK_H

#include <kernel/arch/x86/isr.h>
#include <kernel/task.h>

typedef struct TASK_DATA
{
  UINTPTR MemoryMap;
  UINTPTR NextTask;
  UINTPTR PrevTask;
  TASK_MESSAGE InMessage;
  TASK_MESSAGE OutMessage;
  INTERRUPT_FRAME Frame;
  UINT32 Flags;
  int TaskID;
} TASK_DATA;

void SaveTaskFrame(INTERRUPT_FRAME *pFrame);

void LoadTaskFrame(INTERRUPT_FRAME *pFrame);

#endif
