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
    Tasks[i].pNext = NULL;
    Tasks[i].pPrevious = NULL;
    Tasks[i].pContext = NULL;
    Tasks[i].pMessageQueue = NULL;
    Tasks[i].WaitInfo = 0;
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

const TASK *CreateTask(SIZE_T StackSize, UINT Flags)
{
  TASK *pTask = NULL;
  for (UINT i = 0; i < TASK_LIMIT && NextTaskID != INVALID_TASK_ID; ++i)
  {
    if (Tasks[i].TaskID == INVALID_TASK_ID)
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
      SaveTaskContext(pCurrentTask->pContext);
    }
    else
    {
      pTask->MemoryMap = GetMemoryMap();
      pTask->pNext = pTask;
      pTask->pPrevious = pTask;
    }
    UINTPTR MemoryMap = GetMemoryMap();
    if (MemoryMap != pTask->MemoryMap) SetMemoryMap(pTask->MemoryMap);
    BOOL bTMP = !(pTask->pMessageQueue = CreateMessageQueue()); /* FAILES TO ALLOCATE */
    bTMP = bTMP || !(pTask->pContext = CreateTaskContext(StackSize ? StackSize : TASK_STACK_SIZE));
    if (bTMP)
    {
      if (MemoryMap != pTask->MemoryMap) SetMemoryMap(MemoryMap);
      DeleteTask(pTask);
      return NULL;
    }
    pTask->TaskID = NextTaskID++;
    pTask->Flags = Flags;
    pCurrentTask = pTask;
    LoadTaskContext(pCurrentTask->pContext);
  }
  return pTask;
}

BOOL DeleteTask(const TASK *pTask)
{
  if (!pTask) return FALSE;
  TASK *pTaskToDelete = (TASK*)pTask;
  if (pTaskToDelete == pCurrentTask) SwitchTask(FALSE);
  if (pTaskToDelete == pCurrentTask) return FALSE;
  if (pTaskToDelete->MemoryMap != GetMemoryMap() && !DeleteMemoryMap(pTaskToDelete->MemoryMap)) return FALSE;
  ((TASK*)pTaskToDelete->pPrevious)->pNext = pTaskToDelete->pNext;
  ((TASK*)pTaskToDelete->pNext)->pPrevious = pTaskToDelete->pPrevious;
  pTaskToDelete->MemoryMap = 0;
  pTaskToDelete->pNext = NULL;
  pTaskToDelete->pPrevious = NULL;
  pTaskToDelete->pContext = NULL;
  pTaskToDelete->pMessageQueue = NULL;
  pTaskToDelete->WaitInfo = 0;
  pTaskToDelete->ParentID = INVALID_TASK_ID;
  pTaskToDelete->TaskID = INVALID_TASK_ID;
  pTaskToDelete->Flags = 0;
  return TRUE;
}

void SwitchTask(BOOL bReverse)
{
  if (pCurrentTask && pCurrentTask->pNext != pCurrentTask)
  {
    SaveTaskContext(pCurrentTask->pContext);
    do
    {
      pCurrentTask = (TASK*)(bReverse ? pCurrentTask->pPrevious : pCurrentTask->pNext);
      SetMemoryMap(pCurrentTask->MemoryMap);
      if (pCurrentTask->WaitInfo)
      {
        /* TODO: Handle different types of waiting */
        if (ReceiveTaskMessage((void*)pCurrentTask->WaitInfo, FALSE)) pCurrentTask->WaitInfo = 0;
      }
    } while (pCurrentTask->WaitInfo);
    LoadTaskContext(pCurrentTask->pContext);
  }
}

BOOL SendTaskMessage(MESSAGE *pMessage)
{
  TASK *pTargetTask = (TASK*)GetTaskByID(pMessage->ReceiverID);
  if (!pCurrentTask || !pTargetTask) return FALSE;
  UINTPTR MemoryMap = GetMemoryMap();
  if (pTargetTask->MemoryMap != MemoryMap) SetMemoryMap(pTargetTask->MemoryMap);
  BOOL bResult = PushMessage((MESSAGE_QUEUE*)pTargetTask->pMessageQueue, pMessage);
  if (pTargetTask->MemoryMap != MemoryMap) SetMemoryMap(MemoryMap);
  return bResult;
}

BOOL PeekTaskMessage(MESSAGE *pMessage)
{
  if (!pCurrentTask) return FALSE;
  return PeekMessage((MESSAGE_QUEUE*)pCurrentTask->pMessageQueue, pMessage);
}

BOOL ReceiveTaskMessage(MESSAGE *pMessage, BOOL bWait)
{
  if (!pCurrentTask) return FALSE;
  if (!PollMessage((MESSAGE_QUEUE*)pCurrentTask->pMessageQueue, pMessage))
  {
    if (!bWait) return FALSE;
    TASK *pWaitingTask = pCurrentTask;
    SwitchTask(FALSE);
    if (pCurrentTask == pWaitingTask) return FALSE;
    pWaitingTask->WaitInfo = (UINTPTR)(void*)pMessage;
  }
  return TRUE;
}

