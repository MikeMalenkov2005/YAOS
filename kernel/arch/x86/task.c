#include <kernel/arch/x86/task.h>
#include <kernel/memory.h>

static INTERRUPT_FRAME *pTaskFrame;

TASK_CONTEXT *CreateTaskContext()
{
  TASK_CONTEXT *pContext = MapLastFreePages(PAGE_ROUND_UP(sizeof(TASK_CONTEXT)) / PAGE_SIZE, MAPPING_WRITABLE_BIT | MAPPING_READABLE_BIT);
  return pContext;
}

void DeleteTaskContext(TASK_CONTEXT *pContext)
{
  (void)FreeMappedPages((UINTPTR)pContext, PAGE_ROUND_UP(sizeof(TASK_CONTEXT)) / PAGE_SIZE);
}

void SaveTaskContext(TASK_CONTEXT *pContext)
{
  if (pContext)
  {
    SaveFPU(&pContext->FPUState);
    pContext->Frame = *pTaskFrame;
  }
}

void LoadTaskContext(TASK_CONTEXT *pContext)
{
  if (pContext)
  {
    LoadFPU(&pContext->FPUState);
    *pTaskFrame = pContext->Frame;
  }
}

void SetTaskFrame(INTERRUPT_FRAME *pFrame)
{
  pTaskFrame = pFrame;
}

