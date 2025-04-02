#include <kernel/task.h>
#include <kernel/memory.h>

static TASK Tasks[TASK_LIMIT];
static TASK *pCurrentTask = NULL;
static TASK *pCurrentIRQ = NULL;
static UINT NextGroupID = 0;
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
    Tasks[i].GroupID = INVALID_TASK_ID;
    Tasks[i].TaskID = INVALID_TASK_ID;
    Tasks[i].Flags = 0;
  }
}

const TASK *GetTaskByID(UINT TaskID)
{
  if (TaskID == INVALID_TASK_ID) return NULL;
  for (UINT i = 0; i < TASK_LIMIT; ++i)
  {
    if (Tasks[i].TaskID == TaskID) return &Tasks[i];
  }
  return NULL;
}

const TASK *GetMainTask(UINT GroupID)
{
  if (GroupID == INVALID_TASK_ID) return NULL;
  for (UINT i = 0; i < TASK_LIMIT; ++i)
  {
    if (Tasks[i].GroupID == GroupID && !(Tasks[i].Flags & TASK_THREAD_BIT)) return &Tasks[i];
  }
  return NULL;
}

const TASK *GetCurrentTask()
{
  return pCurrentTask;
}

const TASK *CreateTask(SIZE_T StackSize, UINT Flags)
{
  if (pCurrentIRQ) return NULL;
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
      if (Flags & TASK_THREAD_BIT) pTask->MemoryMap = pCurrentTask->MemoryMap;
      else if (!(pTask->MemoryMap = CreateMemoryMap())) return NULL;
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
      Flags &= ~TASK_THREAD_BIT;
    }
    UINTPTR MemoryMap = GetMemoryMap();
    if (MemoryMap != pTask->MemoryMap) SetMemoryMap(pTask->MemoryMap);
    if (!(pTask->pMessageQueue = CreateMessageQueue()) ||
        !(pTask->pContext = CreateTaskContext(StackSize ? StackSize : TASK_STACK_SIZE, Flags)))
    {
      if (MemoryMap != pTask->MemoryMap) SetMemoryMap(MemoryMap);
      DeleteTask(pTask);
      return NULL;
    }
    pTask->GroupID = (Flags & TASK_THREAD_BIT) ? pCurrentTask->GroupID : NextGroupID++;
    pTask->TaskID = NextTaskID++;
    pTask->Flags = Flags;
    pCurrentTask = pTask;
    LoadTaskContext(pCurrentTask->pContext);
  }
  return pTask;
}

BOOL DeleteTask(const TASK *pTask)
{
  /* TODO: add posibility to delete while IRQ is active */
  if (!pTask || pCurrentIRQ) return FALSE;
  TASK *pTaskToDelete = (TASK*)pTask;
  if (!(pTaskToDelete->Flags & TASK_THREAD_BIT))
  {
    for (UINT i = 0; i < TASK_LIMIT; ++i)
    {
      if (Tasks[i].GroupID == pTaskToDelete->GroupID && (Tasks[i].Flags & TASK_THREAD_BIT))
      {
        (void)DeleteTask(&Tasks[i]);
      }
    }
  }
  if (pTaskToDelete == pCurrentTask) SwitchTask(FALSE);
  if (pTaskToDelete == pCurrentTask) return FALSE;
  if (pTaskToDelete->Flags & TASK_THREAD_BIT)
  {
    UINTPTR MemoryMap = GetMemoryMap();
    if (MemoryMap != pTaskToDelete->MemoryMap) SetMemoryMap(pTaskToDelete->MemoryMap);
    DeleteMessageQueue((void*)pTaskToDelete->pMessageQueue);
    DeleteTaskContext(pTaskToDelete->pContext);
    if (MemoryMap != pTaskToDelete->MemoryMap) SetMemoryMap(MemoryMap);
  }
  else if (pTaskToDelete->MemoryMap != GetMemoryMap() && !DeleteMemoryMap(pTaskToDelete->MemoryMap)) return FALSE;
  ((TASK*)pTaskToDelete->pPrevious)->pNext = pTaskToDelete->pNext;
  ((TASK*)pTaskToDelete->pNext)->pPrevious = pTaskToDelete->pPrevious;
  pTaskToDelete->MemoryMap = 0;
  pTaskToDelete->pNext = NULL;
  pTaskToDelete->pPrevious = NULL;
  pTaskToDelete->pContext = NULL;
  pTaskToDelete->pMessageQueue = NULL;
  pTaskToDelete->WaitInfo = 0;
  pTaskToDelete->ParentID = INVALID_TASK_ID;
  pTaskToDelete->GroupID = INVALID_TASK_ID;
  pTaskToDelete->TaskID = INVALID_TASK_ID;
  pTaskToDelete->Flags = 0;
  return TRUE;
}

void SwitchTask(BOOL bReverse)
{
  /* TODO: meybe add IRQ task switching ? */
  if (!pCurrentIRQ && pCurrentTask && pCurrentTask->pNext != pCurrentTask)
  {
    SaveTaskContext(pCurrentTask->pContext);
    do
    {
      pCurrentTask = (TASK*)(bReverse ? pCurrentTask->pPrevious : pCurrentTask->pNext);
      SetMemoryMap(pCurrentTask->MemoryMap);
      if (pCurrentTask->WaitInfo)
      {
        /* TODO: Handle different types of waiting */
        if (!(pCurrentTask->Flags & TASK_WAIT_IRQ_BIT) &&
            ReceiveTaskMessage((void*)pCurrentTask->WaitInfo, FALSE))
          pCurrentTask->WaitInfo = 0;
      }
    } while (pCurrentTask->WaitInfo);
    LoadTaskContext(pCurrentTask->pContext);
  }
}

BOOL SendTaskMessage(MESSAGE *pMessage)
{
  TASK *pTargetTask = (TASK*)GetTaskByID(pMessage->ReceiverID);
  if (!pTargetTask) return FALSE;
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

BOOL WaitTaskIRQ(UINTPTR IRQ)
{
  TASK *pTask = pCurrentIRQ ? pCurrentIRQ : pCurrentTask;
  if (!pTask || !(pTask->Flags & TASK_MODULE_BIT)) return FALSE;
  EndTaskIRQ();
  if (pTask == pCurrentTask) SwitchTask(FALSE);
  if (pTask == pCurrentTask) return FALSE;
  pTask->WaitInfo = IRQ;
  pTask->Flags |= TASK_WAIT_IRQ_BIT;
  ((TASK*)pTask->pNext)->pPrevious = pTask->pPrevious;
  ((TASK*)pTask->pPrevious)->pNext = pTask->pNext;
  pTask->pNext = NULL;
  pTask->pPrevious = NULL;
  return TRUE;
}

void DebugPrint(UINT8 Prefix, UINTPTR Page);

BOOL BeginTaskIRQ(UINTPTR IRQ)
{
  for (UINT i = 0; i < TASK_LIMIT; ++i)
  {
    if (Tasks[i].WaitInfo == IRQ && (Tasks[i].Flags & TASK_WAIT_IRQ_BIT))
    {
      if (!pCurrentIRQ)
      {
        pCurrentIRQ = &Tasks[i];
        SaveTaskContext(pCurrentTask->pContext);
        SetMemoryMap(pCurrentIRQ->MemoryMap);
        LoadTaskContext(pCurrentIRQ->pContext);
      }
      else if (!Tasks[i].pPrevious && pCurrentIRQ != &Tasks[i])
      {
        TASK *pLastIRQ = pCurrentIRQ;
        while (pLastIRQ->pNext) pLastIRQ = (TASK*)pLastIRQ->pNext;
        pLastIRQ->pNext = &Tasks[i];
        Tasks[i].pPrevious = pLastIRQ;
      }
      return TRUE;
    }
  }
  return FALSE;
}

void EndTaskIRQ()
{
  if (pCurrentIRQ)
  {
    TASK *pTask = pCurrentIRQ;
    TASK *pNextTask = pCurrentTask;
    pCurrentIRQ = (TASK*)pTask->pNext;
    pTask->WaitInfo = 0;
    pTask->Flags &= ~TASK_WAIT_IRQ_BIT;
    pTask->pNext = pCurrentTask;
    pTask->pPrevious = pCurrentTask->pPrevious;
    ((TASK*)pTask->pNext)->pPrevious = pTask;
    ((TASK*)pTask->pPrevious)->pNext = pTask;
    if (pCurrentIRQ)
    {
      pCurrentIRQ->pPrevious = NULL;
      pNextTask = pCurrentIRQ;
    }
    SaveTaskContext(pTask->pContext);
    SetMemoryMap(pNextTask->MemoryMap);
    LoadTaskContext(pNextTask->pContext);
  }
}

