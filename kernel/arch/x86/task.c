#include <kernel/arch/x86/task.h>
#include <kernel/memory.h>
#include <kernel/panic.h>

static BOOL bSwitched = FALSE;
static UINTPTR CurrentTask = 0;
static TASK_SLOT *pSlots = NULL;
static UINTPTR TaskLimit = 0;
static int NextGroupID = 0;
static int NextTaskID = 0;

void InitTaskSlots(UINTPTR MaxTaskCount)
{
  if (pSlots) return; /* I do not know how to change the task limit. */
  pSlots = MapFirstFreePages(PAGE_ROUND_UP(MaxTaskCount * sizeof(TASK_SLOT)) / PAGE_SIZE, MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT | MAPPING_GLOBAL_BIT);
  if (!pSlots) KernelPanic("NO TASKS");
  TaskLimit = MaxTaskCount;
  for (UINTPTR i = 0; i < TaskLimit; ++i) pSlots[i].Task.TaskID = -1;
}

UINTPTR GetCurrentTask()
{
  return CurrentTask;
}

int GetTaskID(UINTPTR Task)
{
  return (Task && Task <= TaskLimit) ? pSlots[Task - 1].Task.TaskID : -1;
}

UINTPTR GetTaskByID(int TaskID)
{
  if (TaskID < 0) return 0;
  if (TaskID == 0 || (CurrentTask && pSlots[CurrentTask - 1].Task.TaskID == TaskID)) return CurrentTask;
  for (UINTPTR i = 0; i < TaskLimit; ++i)
  {
    if (pSlots[i].Task.TaskID == TaskID) return i + 1;
  }
  return 0;
}

int GetTaskGroupID(UINTPTR Task)
{
  return (Task && Task <= TaskLimit) ? pSlots[Task - 1].Task.GroupID : -1;
}

UINTPTR GetTaskGroupLeader(int GroupID)
{
  if (GroupID < 0) return 0;
  if (GroupID == 0 || (CurrentTask && pSlots[CurrentTask - 1].Task.GroupID == GroupID && (pSlots[CurrentTask - 1].Task.Flags & TASK_LEADER_BIT))) return CurrentTask;
  for (UINTPTR i = 0; i < TaskLimit; ++i)
  {
    if (pSlots[i].Task.GroupID == GroupID && (pSlots[i].Task.Flags & TASK_LEADER_BIT)) return i + 1;
  }
  return 0;
}

/* TODO: Add loading initial task state and image!! */
UINTPTR CreateTask(UINT Flags)
{
  if (NextTaskID < 0) return 0;
  for (UINTPTR i = 0; i < TaskLimit; ++i)
  {
    if (pSlots[i].Task.TaskID < 0)
    {
      UINTPTR Task = i + 1;
      if (CurrentTask)
      {
        UINTPTR j = CurrentTask - 1;
        if (Flags & TASK_LEADER_BIT)
        {
          pSlots[i].Task.MemoryMap = CreateMemoryMap(FALSE);
          pSlots[i].Task.NextInGroup = 0;
          pSlots[i].Task.GroupID = NextGroupID++;
        }
        else
        {
          pSlots[i].Task.MemoryMap = CreateMemoryMap(TRUE);
          pSlots[i].Task.NextInGroup = pSlots[j].Task.NextInGroup;
          pSlots[j].Task.NextInGroup = Task;
          pSlots[i].Task.GroupID = pSlots[j].Task.GroupID;
        }

        pSlots[i].Task.NextInLine = pSlots[j].Task.NextInLine;
        pSlots[j].Task.NextInLine = Task;
        pSlots[i].Task.Parent = CurrentTask;
      }
      else
      {
        Flags |= TASK_LEADER_BIT;
        pSlots[i].Task.MemoryMap = GetMemoryMap();
        pSlots[i].Task.NextInLine = Task;
        pSlots[i].Task.NextInGroup = 0;
        pSlots[i].Task.Parent = 0;
        pSlots[i].Task.GroupID = NextGroupID++;
        CurrentTask = Task;
      }
      pSlots[i].Task.FirstSender = 0;
      pSlots[i].Task.Message = 0;
      pSlots[i].Task.Flags = Flags;
      pSlots[i].Task.TaskID = NextTaskID++;
      if (Task != CurrentTask) SetMemoryMap(pSlots[i].Task.MemoryMap);
      pSlots[i].pContext = MapFirstFreePages(PAGE_ROUND_UP(sizeof(TASK_CONTEXT)) / PAGE_SIZE, MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT | MAPPING_LOCAL_BIT);
      if (Task != CurrentTask) SetMemoryMap(pSlots[CurrentTask - 1].Task.MemoryMap);
      return Task;
    }
  }
  return 0;
}

void SwitchTask()
{
  if (CurrentTask)
  {
    SaveFPU(&pSlots[CurrentTask - 1].pContext->FPUState);
    CurrentTask = pSlots[CurrentTask - 1].Task.NextInLine;
    SetMemoryMap(pSlots[CurrentTask - 1].Task.MemoryMap);
    LoadFPU(&pSlots[CurrentTask - 1].pContext->FPUState);
    bSwitched = TRUE;
  }
}

void SaveTaskFrame(INTERRUPT_FRAME *pFrame)
{
  if (CurrentTask) pSlots[CurrentTask - 1].pContext->Frame = *pFrame;
}

void LoadTaskFrame(INTERRUPT_FRAME *pFrame)
{
  if (CurrentTask && bSwitched) *pFrame = pSlots[CurrentTask - 1].pContext->Frame;
}

