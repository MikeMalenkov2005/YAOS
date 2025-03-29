#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <kernel/message.h>

#define INVALID_TASK_ID (~(UINT)0)

#define TASK_MODULE_BIT   (1 << 0)

/* Can be changed with a compiler options */
#ifndef TASK_LIMIT
#define TASK_LIMIT 1024
#endif

#ifndef TASK_STACK_SIZE
#define TASK_STACK_SIZE 0x2000
#endif

typedef struct TASK_CONTEXT TASK_CONTEXT;

typedef struct TASK TASK;

/* All fields are READONLY for other kernel parts!
 * Changing any field WILL messup everything! */
struct TASK
{
  UINTPTR MemoryMap;
  const TASK *pNext;
  const TASK *pPrevious;
  TASK_CONTEXT *pContext;
  const MESSAGE_QUEUE *pMessageQueue;
  UINTPTR WaitInfo;
  UINT ParentID;
  UINT TaskID;
  UINT Flags;
};

void InitTasks();

const TASK *GetTaskByID(UINT TaskID);

TASK_CONTEXT *CreateTaskContext(SIZE_T StackSize, UINT Flags);

void DeleteTaskContext(TASK_CONTEXT *pContext);

void SaveTaskContext(TASK_CONTEXT *pContext);

void LoadTaskContext(TASK_CONTEXT *pContext);

void SetCurrentUserIP(UINTPTR IP);

const TASK *GetCurrentTask();

const TASK *CreateTask(SIZE_T StackSize, UINT Flags);

BOOL DeleteTask(const TASK *pTask);

void SwitchTask(BOOL bReverse);

BOOL SendTaskMessage(MESSAGE *pMessage);

BOOL PeekTaskMessage(MESSAGE *pMessage);

BOOL ReceiveTaskMessage(MESSAGE *pMessage, BOOL bWait);

#endif
