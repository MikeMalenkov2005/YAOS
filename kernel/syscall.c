#include <kernel/syscall.h>
#include <kernel/memory.h>
#include <kernel/task.h>

void HandleSystemCall(SYSRET *pResult, SYSCALL Function, SYSCALL_ARGUMENTS Arguments)
{
  const TASK *pCurrentTask = GetCurrentTask();
  if (!pCurrentTask)
  {
    *pResult = SYSRET_INVALID_CALLER;
    return;
  }
  *pResult = SYSRET_INVALID_FUNCTION;
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
    case SYSCALL_CREATE_TASK:
      /* TODO: Implement */
      break;
    case SYSCALL_MAP_MEMORY:
      {
        UINT MappingFlags = MAPPING_USER_MODE_BIT;
        if (Arguments.C & MAP_MEMORY_READABLE) MappingFlags |= MAPPING_READABLE_BIT;
        if (Arguments.C & MAP_MEMORY_WRITABLE) MappingFlags |= MAPPING_WRITABLE_BIT;
        if (Arguments.C & MAP_MEMORY_EXECUTABLE) MappingFlags |= MAPPING_EXECUTABLE_BIT;
        *pResult = (UINTPTR)MapFirstFreePages(Arguments.B, MappingFlags);
      }
      break;
    case SYSCALL_MAP_DEVICE:
      if ((pCurrentTask->Flags & TASK_MODULE_BIT) && Arguments.B && Arguments.C)
      {
        UINTPTR FirstPage = FindLastFreeVirtualPages(Arguments.B);
        UINTPTR Mapping = (Arguments.C & PAGE_ADDRESS_MASK) | MAPPING_USER_MODE_BIT | MAPPING_PRESENT_BIT | MAPPING_EXTERNAL_BIT;
        if (Arguments.C & MAP_MEMORY_READABLE) Mapping |= MAPPING_READABLE_BIT;
        if (Arguments.C & MAP_MEMORY_WRITABLE) Mapping |= MAPPING_WRITABLE_BIT;
        for (UINTPTR i = 0; i < Arguments.B; ++i) (void)SetPageMapping(FirstPage + i * PAGE_SIZE, Mapping + i * PAGE_SIZE);
        *pResult = FirstPage;
      }
      else *pResult = 0;
      break;
    case SYSCALL_FREE_MAPPING:
      /* TODO: Implement */
      break;
    case SYSCALL_SHARE_MAPPING:
      /* TODO: Implement */
      break;
  }
}
