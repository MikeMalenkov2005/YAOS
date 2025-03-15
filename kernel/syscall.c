#include <kernel/syscall.h>
#include <kernel/task.h>

UINTPTR HandleSystemCall(UINTPTR Function, UINTPTR Param1, UINTPTR Param2)
{
  /* TODO: Handle System Calls */
  return -(Function && Param1 && Param2);
}
