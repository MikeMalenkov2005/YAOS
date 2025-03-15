#include <kernel/arch/x86/task.h>
#include <kernel/memory.h>
#include <kernel/panic.h>

static BOOL bSwitched = FALSE;
static UINTPTR CurrentTask = 0;
static TASK_DATA *pTasks = NULL;
static UINTPTR TaskLimit = 0;
static int NextTaskID = 0;

UINTPTR GetCurrentTask()
{
  return CurrentTask;
}

int GetTaskID(UINTPTR Task)
{
  return (Task && Task <= TaskLimit) ? pTasks[Task - 1].TaskID : -1;
}

UINTPTR GetTaskByID(int TaskID)
{
  if (TaskID < 0) return 0;
  if (TaskID == 0 || (CurrentTask && pTasks[CurrentTask - 1].TaskID == TaskID)) return CurrentTask;
  for (UINTPTR i = 0; i < TaskLimit; ++i)
  {
    if (pTasks[i].TaskID == TaskID) return i + 1;
  }
  return 0;
}

UINTPTR CreateTask(UINTPTR MemoryMap)
{
  if (NextTaskID < 0) return 0;
  for (UINTPTR i = 0; i < TaskLimit; ++i)
  {
    if (pTasks[i].TaskID < 0)
    {
      UINTPTR Task = i + 1;
      pTasks[i].TaskID = NextTaskID++;
      pTasks[i].MemoryMap = MemoryMap;
      if (CurrentTask)
      {
        pTasks[i].PrevTask = CurrentTask;
        pTasks[i].NextTask = pTasks[CurrentTask - 1].NextTask;
        pTasks[pTasks[i].NextTask - 1].PrevTask = Task;
        pTasks[CurrentTask - 1].NextTask = Task;
      }
      else
      {
        pTasks[i].NextTask = Task;
        pTasks[i].PrevTask = Task;
        CurrentTask = Task;
      }
      /* TODO: Finish Task Creation */
      pTasks[i].InMessage.Task = 0;
      pTasks[i].OutMessage.Task = 0;
      pTasks[i].Flags = 0;
      return Task;
    }
  }
  return 0;
}

void SetTaskLimit(UINTPTR MaxTaskCount)
{
  /* TODO: Resize if pTasks is non-zero */
  pTasks = MapFirstFreePages(PAGE_ROUND_UP(MaxTaskCount * sizeof(TASK_DATA)) / PAGE_SIZE, MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT);
  if (!pTasks) KernelPanic("NO TASKS");
  TaskLimit = MaxTaskCount;
  for (UINTPTR i = 0; i < TaskLimit; ++i) pTasks[i].TaskID = -1;
}

void SwitchTask()
{
  if (CurrentTask)
  {
    CurrentTask = pTasks[CurrentTask - 1].NextTask;
    SetMemoryMap(pTasks[CurrentTask - 1].MemoryMap);
    bSwitched = TRUE;
  }
}

void SaveTaskFrame(INTERRUPT_FRAME *pFrame)
{
  if (CurrentTask) pTasks[CurrentTask - 1].Frame = *pFrame;
}

void LoadTaskFrame(INTERRUPT_FRAME *pFrame)
{
  if (CurrentTask && bSwitched) *pFrame = pTasks[CurrentTask - 1].Frame;
}

