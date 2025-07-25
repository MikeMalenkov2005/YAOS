#include "sys/syscall.h"
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
        Message.SenderID = pCurrentTask->TaskID;
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
    case SYSCALL_GET_LEADER_ID:
      *pResult = (pCurrentTask->Flags & TASK_THREAD_BIT) ? GetMainTask(pCurrentTask->GroupID)->TaskID : pCurrentTask->TaskID;
      break;
    case SYSCALL_CREATE_TASK:
      {
        UINT Flags = pCurrentTask->Flags | TASK_THREAD_BIT;
        const TASK *pNewTask = CreateTask(Arguments.B, Flags);
        if (pNewTask)
        {
          SetCurrentUserIP(Arguments.A);
          SwitchTask(TRUE);
          *pResult = pNewTask->TaskID;
        }
        else *pResult = INVALID_TASK_ID;
      }
      break;
    case SYSCALL_MAP_MEMORY:
      if (Arguments.B)
      {
        SIZE_T PageCount = PAGE_ROUND_UP(Arguments.B) >> PAGE_SHIFT;
        UINT MappingFlags = MAPPING_USER_MODE_BIT;
        if (Arguments.C & MAP_MEMORY_READABLE) MappingFlags |= MAPPING_READABLE_BIT;
        if (Arguments.C & MAP_MEMORY_WRITABLE) MappingFlags |= MAPPING_WRITABLE_BIT;
        if (Arguments.C & MAP_MEMORY_EXECUTABLE) MappingFlags |= MAPPING_EXECUTABLE_BIT;
        *pResult = (UINTPTR)MapFirstFreePages(PageCount, MappingFlags);
      }
      else *pResult = 0;
      break;
    case SYSCALL_MAP_DEVICE:
      if ((pCurrentTask->Flags & TASK_MODULE_BIT) && Arguments.B && Arguments.C)
      {
        SIZE_T PageCount = PAGE_ROUND_UP(Arguments.B) >> PAGE_SHIFT;
        UINTPTR FirstPage = FindLastFreeVirtualPages(PageCount);
        UINTPTR Mapping = (Arguments.C & PAGE_ADDRESS_MASK) | MAPPING_USER_MODE_BIT | MAPPING_PRESENT_BIT | MAPPING_EXTERNAL_BIT | MAPPING_CACHE_DISABLE_BIT;
        if (Arguments.C & MAP_MEMORY_READABLE) Mapping |= MAPPING_READABLE_BIT;
        if (Arguments.C & MAP_MEMORY_WRITABLE) Mapping |= MAPPING_WRITABLE_BIT;
        for (UINTPTR i = 0; i < PageCount; ++i) (void)SetPageMapping(FirstPage + i * PAGE_SIZE, Mapping + i * PAGE_SIZE);
        *pResult = FirstPage;
      }
      else *pResult = 0;
      break;
    case SYSCALL_FREE_MAPPING:
      *pResult = SYSRET_INVALID_ARGUMENT;
      if (!(Arguments.A & PAGE_FLAGS_MASK) && Arguments.B)
      {
        SIZE_T PageCount = PAGE_ROUND_UP(Arguments.B) >> PAGE_SHIFT;
        UINT Flags = GetRangeMappingFlags(Arguments.A, Arguments.B);
        *pResult = (Flags & MAPPING_USER_MODE_BIT) ? SYSRET_OK : SYSRET_INVALID_ARGUMENT;
        if (Flags & MAPPING_USER_MODE_BIT)
        {
          if (Flags & MAPPING_EXTERNAL_BIT)
          {
            for (SIZE_T i = 0; i < PageCount; ++i) (void)SetPageMapping(Arguments.A + i * PageCount, 0);
          }
          else (void)FreeMappedPages(Arguments.A, PageCount);
          *pResult = SYSRET_OK;
        }
      }
      break;
    case SYSCALL_SHARE_MAPPING:
      /* TODO: Implement SYSCALL_SHARE_MAPPING */
      break;
    case SYSCALL_REMAP_MEMORY:
      *pResult = SYSRET_INVALID_ARGUMENT;
      if (!(Arguments.A & PAGE_FLAGS_MASK) && Arguments.B)
      {
        SIZE_T PageCount = PAGE_ROUND_UP(Arguments.B) >> PAGE_SHIFT;
        UINT Flags = GetRangeMappingFlags(Arguments.A, Arguments.B);
        if (Flags & MAPPING_USER_MODE_BIT)
        {
          Flags = (Arguments.B & MAP_MEMORY_READABLE) ? (Flags | MAPPING_READABLE_BIT) : (Flags & ~MAPPING_READABLE_BIT);
          Flags = (Arguments.B & MAP_MEMORY_WRITABLE) ? (Flags | MAPPING_WRITABLE_BIT) : (Flags & ~MAPPING_WRITABLE_BIT);
          Flags = (Arguments.B & MAP_MEMORY_EXECUTABLE) ? (Flags | MAPPING_EXECUTABLE_BIT) : (Flags & ~MAPPING_EXECUTABLE_BIT);
          if (RemapPages(Arguments.A, PageCount, Flags)) *pResult = SYSRET_OK;
        }
      }
      break;
    case SYSCALL_WAIT_IRQ:
      *pResult = SYSRET_OK;
      if (!WaitTaskIRQ(Arguments.A)) *pResult = SYSRET_INVALID_CALLER;
      break;
    case SYSCALL_END_IRQ:
      *pResult = SYSRET_OK;
      EndTaskIRQ();
      break;
  }
}
