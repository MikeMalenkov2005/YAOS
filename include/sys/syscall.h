#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

#include <types.h>

typedef UINTPTR SYSCALL;

/* Task Termination */
#define SYSCALL_TERMINATE     0   /* (0, 0, 0) */
/* Message Passing */
#define SYSCALL_SEND_MESSAGE  1   /* (MESSAGE*, 0, 0) */
#define SYSCALL_POLL_MESSAGE  2   /* (MESSAGE*, 0, 0) */
#define SYSCALL_WAIT_MESSAGE  3   /* (MESSAGE*, 0, 0) */
#define SYSCALL_PEEK_MESSAGE  4   /* (MESSAGE*, 0, 0) */
/* Task Management */
#define SYSCALL_GET_TASK_ID   5   /* (0, 0, 0) */
#define SYSCALL_GET_GROUP_ID  6   /* (0, 0, 0) */
#define SYSCALL_GET_PARENT_ID 7   /* (0, 0, 0) */
#define SYSCALL_GET_LEADER_ID 8   /* (0, 0, 0) */
#define SYSCALL_CREATE_THREAD 9   /* (EntryPoint, StackSize, 0) */
/* Memory Management */
#define SYSCALL_MAP_MEMORY    10  /* (Reserved, Size, Flags) */
#define SYSCALL_MAP_DEVICE    11  /* (Reserved, Size, Mapping) System Module Only */
#define SYSCALL_FREE_MAPPING  12  /* (VAddr, Size, 0) */
#define SYSCALL_SHARE_MAPPING 13  /* (VAddr, Size, TaskID) */
/* Device IRQ Handling */
#define SYSCALL_WAIT_IRQ      30  /* (IRQ, 0, 0) System Module Only */
#define SYSCALL_END_IRQ       31  /* (0, 0, 0) System Module Only */

/* Create Task Flags (NOT IMPLEMENTED) */

/* Map Memory Flags */
#define MAP_MEMORY_WRITABLE   1
#define MAP_MEMORY_READABLE   2
#define MAP_MEMORY_EXECUTABLE 4

typedef INTPTR SYSRET;

/* System Call Results */
#define SYSRET_OK               0
#define SYSRET_FALSE            1
#define SYSRET_INVALID_FUNCTION -1
#define SYSRET_INVALID_CALLER   -2
#define SYSRET_INVALID_ARGUMENT -3
#define SYSRET_UNAVAILABLE_TASK -4

inline static SYSRET InvokeSystemCall(SYSCALL Function, UINTPTR A, UINTPTR B, UINTPTR C)
{
  SYSRET Result;
#if defined(__x86_64__)
  asm volatile ("syscall" : "=a"(Result) : "0"(Function), "D"(A), "S"(B), "d"(C) : "rcx", "r11", "memory");
#elif defined(__i386__)
  asm volatile ("int $0x80" : "=a"(Result) : "0"(Function), "b"(A), "c"(B), "d"(C) : "memory");
#else
#error "Unsupported Processor!"
#endif
  return Result;
}

#endif

