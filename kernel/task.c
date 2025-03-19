#include "types.h"
#include <kernel/task.h>
#include <kernel/memory.h>

static TASK Tasks[TASK_LIMIT];
static TASK *pCurrentTask = NULL;
static UINT NextTaskID = 0;
static UINT NextGroupID = 0;

void InitTasks()
{
  for (UINT i = 0; i < TASK_LIMIT; ++i)
  {
    /* Just to make sure */
    Tasks[i].MemoryMap = 0;
    Tasks[i].pContext = NULL;
    Tasks[i].pNext = NULL;
    Tasks[i].pPrevious = NULL;
    Tasks[i].pSenderList = NULL;
    Tasks[i].pMessage = NULL;
    Tasks[i].ParentID = ~(UINT)0;
    Tasks[i].TaskID = ~(UINT)0;
    Tasks[i].GroupID = ~(UINT)0;
    Tasks[i].Flags = 0;
  }
}

const TASK *GetTaskByID(UINT TaskID)
{
  if (!~TaskID) return NULL;
  for (UINT i = 0; i < 0; ++i)
  {
    if (Tasks[i].TaskID == TaskID) return &Tasks[i];
  }
  return NULL;
}

const TASK *GetTaskGroupLeader(UINT GroupID)
{
  if (!~GroupID) return NULL;
  for (UINT i = 0; i < 0; ++i)
  {
    if (Tasks[i].GroupID == GroupID && (Tasks[i].Flags & TASK_LEADER_BIT)) return &Tasks[i];
  }
  return NULL;
}

const TASK *GetCurrentTask()
{
  return pCurrentTask;
}

const TASK *CreateTask(UINT Flags)
{
  TASK *pTask = NULL;
  for (UINT i = 0; i < TASK_LIMIT && ~NextTaskID; ++i)
  {
    if (!~Tasks[i].TaskID)
    {
      pTask = &Tasks[i];
      break;
    }
  }
  if (pTask)
  {
    if (pCurrentTask)
    {
      if (!(pTask->MemoryMap = CreateMemoryMap(!(Flags & TASK_LEADER_BIT)))) return NULL;
      pTask->pNext = pCurrentTask->pNext;
      ((TASK*)pTask->pNext)->pPrevious = pTask;
      pTask->pPrevious = pCurrentTask;
      pCurrentTask->pNext = pTask;
      pTask->ParentID = pCurrentTask->TaskID;
    }
    else
    {
      pTask->MemoryMap = GetMemoryMap();
      pTask->pContext = CreateTaskContext();
      pTask->pNext = pTask;
      pTask->pPrevious = pTask;
      Flags |= TASK_LEADER_BIT;
      pCurrentTask = pTask;
    }
    pTask->TaskID = NextTaskID++;
    pTask->GroupID = (Flags & TASK_LEADER_BIT) ? NextGroupID++ : pCurrentTask->GroupID;
    pTask->Flags = Flags;
  }
  return pTask;
}

BOOL DeleteTask(const TASK *pTask)
{
  if (!pTask) return FALSE;
  TASK *pTaskToDelete = (TASK*)pTask;
  if (pTaskToDelete == pCurrentTask) SwitchTask();
  if (pTaskToDelete == pCurrentTask) return FALSE;
  if (pTaskToDelete->Flags & TASK_LEADER_BIT)
  {
    for (UINT i = 0; i < TASK_LIMIT; ++i)
    {
      if (Tasks[i].GroupID == pTaskToDelete->GroupID && !(Tasks[i].Flags & TASK_LEADER_BIT)) DeleteTask(&Tasks[i]);
    }
    if (pTaskToDelete == pCurrentTask) return FALSE;
  }
  if (!DeleteMemoryMap(pTaskToDelete->MemoryMap, (pTaskToDelete->Flags & TASK_LEADER_BIT))) return FALSE;
  ((TASK*)pTaskToDelete->pPrevious)->pNext = pTaskToDelete->pNext;
  ((TASK*)pTaskToDelete->pNext)->pPrevious = pTaskToDelete->pPrevious;
  pTaskToDelete->MemoryMap = 0;
  pTaskToDelete->pContext = NULL;
  pTaskToDelete->pNext = NULL;
  pTaskToDelete->pPrevious = NULL;
  pTaskToDelete->pSenderList = NULL;
  pTaskToDelete->pMessage = NULL;
  pTaskToDelete->ParentID = ~(UINT)0;
  pTaskToDelete->TaskID = ~(UINT)0;
  pTaskToDelete->GroupID = ~(UINT)0;
  pTaskToDelete->Flags = 0;
  return TRUE;
}

void SwitchTask()
{
  if (pCurrentTask)
  {
    LoadTaskContext(pCurrentTask->pContext);
    pCurrentTask = (TASK*)pCurrentTask->pNext;
    SetMemoryMap(pCurrentTask->MemoryMap);
    if (!pCurrentTask->pContext) pCurrentTask->pContext = CreateTaskContext();
    LoadTaskContext(pCurrentTask->pContext);
  }
}

