#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <types.h>

/* TODO: Define System Calls */
#define SYSCALL_TERMINATE 0
#define SYSCALL_SEND_MSG  1
#define SYSCALL_WAIT_MSG  2
#define SYSCALL_CHECK_MSG 3

UINTPTR HandleSystemCall(UINTPTR Function, UINTPTR Param1, UINTPTR Param2);

#endif
