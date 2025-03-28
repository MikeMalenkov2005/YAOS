#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

#include <types.h>

/* TODO: Define System Calls */
#define SYSCALL_TERMINATE     0
#define SYSCALL_SEND_MESSAGE  1
#define SYSCALL_POLL_MESSAGE  2
#define SYSCALL_WAIT_MESSAGE  3
#define SYSCALL_PEEK_MESSAGE  4
#define SYSCALL_GET_TASK_ID   5
#define SYSCALL_GET_PARENT_ID 6
#define SYSCALL_CREATE_TASK   7

/* TODO: Define System Returnes */
#define SYSRET_OK               0
#define SYSRET_FALSE            1
#define SYSRET_UNKNOWN_ERROR    -1
#define SYSRET_UNKNOWN_FUNCTION -2
#define SYSRET_INVALID_CALLER   -3
#define SYSRET_INVALID_ARGUMENT -4
#define SYSRET_UNAVAILABLE_TASK -5

inline static UINTPTR InvokeSystemCall(UINTPTR Function, UINTPTR A, UINTPTR B, UINTPTR C)
{
  UINTPTR Result;
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

