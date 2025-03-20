#include <kernel/arch/x86/task.h>
#include <kernel/memory.h>

static UINT LastSavedTaskID;

TASK_CONTEXT *CreateTaskContext()
{
  return MapLastFreePages(sizeof(TASK_CONTEXT) / PAGE_SIZE, MAPPING_WRITABLE_BIT | MAPPING_READABLE_BIT);
}

void SaveTaskContext(TASK_CONTEXT *pContext)
{
  if (pContext) SaveFPU(&pContext->FPUState);
}

void LoadTaskContext(TASK_CONTEXT *pContext)
{
  if (pContext) LoadFPU(&pContext->FPUState);
}

void SaveTaskFrame(INTERRUPT_FRAME *pFrame)
{
  const TASK *pCurrentTask = GetCurrentTask();
  if (pCurrentTask)
  {
    LastSavedTaskID = pCurrentTask->TaskID;
    pCurrentTask->pContext->Frame = *pFrame;
  }
}

void LoadTaskFrame(INTERRUPT_FRAME *pFrame)
{
  const TASK *pCurrentTask = GetCurrentTask();
  if (pCurrentTask && LastSavedTaskID != pCurrentTask->TaskID)
  {
    *pFrame = pCurrentTask->pContext->Frame;
  }
}

