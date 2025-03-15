#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <types.h>

typedef struct TASK_MESSAGE
{
  UINTPTR Task;
  UINTPTR Payload;
} TASK_MESSAGE;

UINTPTR GetCurrentTask();

int GetTaskID(UINTPTR Task);

UINTPTR GetTaskByID(int TaskID);

UINTPTR CreateTask(UINTPTR MemoryMap);

void SetTaskLimit(UINTPTR MaxTaskCount);

void SwitchTask();

#endif
