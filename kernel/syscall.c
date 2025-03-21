#include <kernel/syscall.h>
#include <kernel/memory.h>
#include <kernel/task.h>

void HandleSystemCall(UINTPTR *pResult, UINTPTR Function, SYSCALL_ARGUMENTS Arguments)
{
  const TASK *pCurrentTask = GetCurrentTask();
  if (!pCurrentTask)
  {
    *pResult = SYSRET_INVALID_CALLER;
    return;
  }
  *pResult = SYSRET_UNKNOWN_FUNCTION;
  /* TODO: Handle System Calls */
  switch (Function)
  {
    case SYSCALL_TERMINATE:
      *pResult = SYSRET_INVALID_CALLER;
      (void)DeleteTask(pCurrentTask);
      break;
    case SYSCALL_SEND_MESSAGE:
      *pResult = SYSRET_INVALID_ARGUMENT;
      if (CheckUserAccess((void*)Arguments.A, sizeof(MESSAGE)))
      {
        MESSAGE Message = *(MESSAGE*)(void*)Arguments.A;
        *pResult = SendTaskMessage(&Message) ? SYSRET_OK : SYSRET_UNAVAILABLE_TASK;
      } /* Probobly should segfault otherwize */
      break;
    case SYSCALL_POLL_MESSAGE:
    case SYSCALL_WAIT_MESSAGE:
      *pResult = SYSRET_OK;
      if (!CheckUserAccess((void*)Arguments.A, sizeof(MESSAGE))) *pResult = SYSRET_INVALID_ARGUMENT;
      else if (!ReceiveTaskMessage((MESSAGE*)Arguments.A, Function == SYSCALL_WAIT_MESSAGE))
      {
        *pResult = Function == SYSCALL_WAIT_MESSAGE ? SYSRET_INVALID_CALLER : SYSRET_FALSE;
      }
      break;
    case SYSCALL_PEEK_MESSAGE:
      if (!CheckUserAccess((void*)Arguments.A, sizeof(MESSAGE))) *pResult = SYSRET_INVALID_ARGUMENT;
      else *pResult = PeekTaskMessage((void*)Arguments.A) ? SYSRET_OK : SYSRET_FALSE;
      break;
    case SYSCALL_GET_TASK_ID:
      *pResult = pCurrentTask->TaskID;
      break;
    case SYSCALL_GET_PARENT_ID:
      *pResult = pCurrentTask->ParentID;
      break;
  }
}
