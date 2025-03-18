#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <types.h>

#define TASK_LEADER_BIT (1 << 0)
#define TASK_MODULE_BIT (1 << 1)

/* Can be changed with a compiler options */
#ifndef TASK_LIMIT
#define TASK_LIMIT 1024
#endif

typedef struct TASK_CONTEXT TASK_CONTEXT;

typedef struct TASK TASK;

/* All fields are READONLY for other kernel parts!
 * Changing any field WILL messup everything! */
struct TASK
{
  UINTPTR MemoryMap;
  TASK_CONTEXT *pContext;
  const TASK *pNext;
  const TASK *pPrevious;
  const TASK *pSenderList;
  const TASK *pMessage;
  UINT ParentID;
  UINT GroupID;
  UINT TaskID;
  UINT Flags;
};

void InitTasks();

const TASK *GetTaskByID(UINT TaskID);

const TASK *GetTaskGroupLeader(UINT GroupID);

TASK_CONTEXT *CreateTaskContext();

void SaveTaskContext(TASK_CONTEXT *pContext);

void LoadTaskContext(TASK_CONTEXT *pContext);

const TASK *GetCurrentTask();

const TASK *CreateTask(UINT Flags);

BOOL DeleteTask(const TASK *pTask);

void SwitchTask();

#endif
