#ifndef SYS_YAOS_H
#define SYS_YAOS_H

#include <sys/message.h>
#include <sys/syscall.h>

#define IsError(SR) (((SYSRET)(SR)) < 0)

inline static void Terminate()
{
  (void)InvokeSystemCall(SYSCALL_TERMINATE, 0, 0, 0);
  for (;;);
}

inline static SYSRET SendMessage(MESSAGE *pMessage)
{
  return InvokeSystemCall(SYSCALL_SEND_MESSAGE, (UINTPTR)pMessage, 0, 0);
}

inline static SYSRET PollMessage(MESSAGE *pMessage)
{
  return InvokeSystemCall(SYSCALL_POLL_MESSAGE, (UINTPTR)pMessage, 0, 0);
}

inline static SYSRET WaitMessage(MESSAGE *pMessage)
{
  return InvokeSystemCall(SYSCALL_WAIT_MESSAGE, (UINTPTR)pMessage, 0, 0);
}

inline static SYSRET PeekMessage(MESSAGE *pMessage)
{
  return InvokeSystemCall(SYSCALL_PEEK_MESSAGE, (UINTPTR)pMessage, 0, 0);
}

inline static UINT GetTaskID()
{
  return (UINT)InvokeSystemCall(SYSCALL_GET_TASK_ID, 0, 0, 0);
}

inline static UINT GetParentID()
{
  return (UINT)InvokeSystemCall(SYSCALL_GET_PARENT_ID, 0, 0, 0);
}

inline static UINT GetLeaderID()
{
  return (UINT)InvokeSystemCall(SYSCALL_GET_LEADER_ID, 0, 0, 0);
}

inline static UINT CreateThread(void (*pfnEntry)(void), SIZE_T StackSize)
{
  return (UINT)InvokeSystemCall(SYSCALL_CREATE_TASK, (UINTPTR)(void*)pfnEntry, StackSize, 0);
}

inline static void* MapMemory(void *pReserved, SIZE_T Size, UINT Flags)
{
  return (void*)(UINTPTR)InvokeSystemCall(SYSCALL_MAP_MEMORY, (UINTPTR)pReserved, Size, Flags);
}

inline static void* MapDevice(void *pReserved, SIZE_T Size, UINTPTR Mapping)
{
  return (void*)(UINTPTR)InvokeSystemCall(SYSCALL_MAP_DEVICE, (UINTPTR)pReserved, Size, Mapping);
}

inline static SYSRET FreeMapping(void *pStart, SIZE_T Size)
{
  return InvokeSystemCall(SYSCALL_FREE_MAPPING, (UINTPTR)pStart, Size, 0);
}

inline static SYSRET ShareMapping(void *pStart, SIZE_T Size, UINT TaskID)
{
  return InvokeSystemCall(SYSCALL_SHARE_MAPPING, (UINTPTR)pStart, Size, TaskID);
}

inline static SYSRET WaitIRQ(UINTPTR IRQ)
{
  return InvokeSystemCall(SYSCALL_WAIT_IRQ, IRQ, 0, 0);
}

inline static SYSRET EndIRQ()
{
  return InvokeSystemCall(SYSCALL_END_IRQ, 0, 0, 0);
}

inline static void PrepareResponse(MESSAGE *Message)
{
  for (UINT8 Index = 0; Index < sizeof(Message->Payload); ++Index)
  {
    Message->Payload[Index] = 0;
  }
  Message->ReceiverID = Message->SenderID;
}

#endif

