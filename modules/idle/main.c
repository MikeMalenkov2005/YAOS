#include <sys/syscall.h>

#define SCREEN ((UINT16*)(void*)0xB8000)

const static char szMessage[] = "Idle loaded";

void Init()
{
  UINTPTR TaskID = InvokeSystemCall(SYSCALL_GET_TASK_ID, 0, 0, 0);
  for (SIZE_T i = 0; i < sizeof(szMessage); ++i) SCREEN[TaskID * 80 + i] = (UINT16)szMessage[i] | 0x200;
  SCREEN[TaskID * 80 + sizeof(szMessage)] = (TaskID + '0') | 0x200;
  for(;;);
}

