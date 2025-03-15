#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <types.h>

#define TASK_LEADER_BIT (1 << 0)
#define TASK_MODULE_BIT (1 << 1)

typedef struct TASK
{
  UINTPTR MemoryMap;
  UINTPTR NextInLine;
  UINTPTR NextInGroup;
  UINTPTR FirstSender;
  UINTPTR Message;
  UINTPTR Parent;
  UINT Flags;
  int GroupID;
  int TaskID;
} TASK;

void InitTaskSlots(UINTPTR MaxTaskCount);

UINTPTR GetCurrentTask();

int GetTaskID(UINTPTR Task);

UINTPTR GetTaskByID(int TaskID);

int GetTaskGroupID(UINTPTR Task);

UINTPTR GetTaskGroupLeader(int GroupID);

UINTPTR CreateTask(UINT Flags);

void DeleteTask(UINTPTR Task);

void SwitchTask();

#endif
