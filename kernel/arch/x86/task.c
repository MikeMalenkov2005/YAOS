#include <kernel/arch/x86/task.h>
#include <kernel/memory.h>

static INTERRUPT_FRAME *pTaskFrame;

static __naked UINT32 GetEFLAGS()
{
  asm volatile ("pushf");
  asm volatile ("pop %eax");
  asm volatile ("ret");
}

TASK_CONTEXT *CreateTaskContext(SIZE_T StackSize, UINT Flags)
{
  TASK_CONTEXT *pContext = MapLastFreePages(PAGE_ROUND_UP(sizeof(TASK_CONTEXT)) / PAGE_SIZE, MAPPING_WRITABLE_BIT | MAPPING_READABLE_BIT);
  if (pContext)
  {
    void *pStack = MapLastFreePages(PAGE_ROUND_UP(StackSize) >> PAGE_SHIFT, MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT | MAPPING_USER_MODE_BIT);
    if (!pStack)
    {
      (void)FreeMappedPages((UINTPTR)(void*)pContext, PAGE_ROUND_UP(sizeof(TASK_CONTEXT)) / PAGE_SIZE);
      return NULL;
    }
    for (SIZE_T i = 0; i < sizeof(TASK_CONTEXT); ++i) ((UINT8*)(void*)pContext)[i] = 0;
    pContext->Frame.CS = 0x1B;
    pContext->Frame.DS = 0x23;
    pContext->Frame.SS = 0x23;
    pContext->Frame.ESP = (UINTPTR)pStack + StackSize;
    pContext->Frame.EFLAGS = GetEFLAGS();
    pContext->Frame.EFLAGS &= ~(UINT32)((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 7) | (1 << 10) | (1 << 11) | (3 << 12)); /* Reset Status Flags And IOPL */
    pContext->Frame.EFLAGS |= (1 << 9); /* Set Interrupt Enable Flag */
    if (Flags & TASK_MODULE_BIT) pContext->Frame.EFLAGS |= (3 << 12); /* Set IOPL = 3 */
  }
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

void SetCurrentUserIP(UINTPTR IP)
{
  if (pTaskFrame) pTaskFrame->EIP = IP;
}

void SetTaskFrame(INTERRUPT_FRAME *pFrame)
{
  pTaskFrame = pFrame;
}

