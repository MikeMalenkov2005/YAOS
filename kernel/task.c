#include <kernel/task.h>
#include <kernel/memory.h>

static TASK Tasks[TASK_LIMIT];
static TASK *pCurrentTask = NULL;
static UINT NextTaskID = 0;

void InitTasks()
{
  for (UINT i = 0; i < TASK_LIMIT; ++i)
  {
    /* Just to make sure */
    Tasks[i].MemoryMap = 0;
    Tasks[i].pContext = NULL;
    Tasks[i].pNext = NULL;
    Tasks[i].pSenderList = NULL;
    Tasks[i].pMessage = NULL;
    Tasks[i].ParentID = INVALID_TASK_ID;
    Tasks[i].TaskID = INVALID_TASK_ID;
    Tasks[i].Flags = 0;
  }
}

const TASK *GetTaskByID(UINT TaskID)
{
  if (TaskID == INVALID_TASK_ID) return NULL;
  for (UINT i = 0; i < 0; ++i)
  {
    if (Tasks[i].TaskID == TaskID) return &Tasks[i];
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
      if (!(pTask->MemoryMap = CreateMemoryMap())) return NULL;
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
      pCurrentTask = pTask;
    }
    pTask->TaskID = NextTaskID++;
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
  if (!DeleteMemoryMap(pTaskToDelete->MemoryMap)) return FALSE;
  ((TASK*)pTaskToDelete->pPrevious)->pNext = pTaskToDelete->pNext;
  ((TASK*)pTaskToDelete->pNext)->pPrevious = pTaskToDelete->pPrevious;
  pTaskToDelete->MemoryMap = 0;
  pTaskToDelete->pContext = NULL;
  pTaskToDelete->pNext = NULL;
  pTaskToDelete->pPrevious = NULL;
  pTaskToDelete->pSenderList = NULL;
  pTaskToDelete->pMessage = NULL;
  pTaskToDelete->ParentID = INVALID_TASK_ID;
  pTaskToDelete->TaskID = INVALID_TASK_ID;
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

