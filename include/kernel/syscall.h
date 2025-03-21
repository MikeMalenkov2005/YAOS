#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <sys/syscall.h>

typedef struct SYSCALL_ARGUMENTS
{
  UINTPTR A, B, C;
} SYSCALL_ARGUMENTS;

void HandleSystemCall(UINTPTR *pResult, UINTPTR Function, SYSCALL_ARGUMENTS Arguments);

#endif
