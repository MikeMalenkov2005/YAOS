#include "kernel.h"
#include "task.h"
#include "time.h"

#include <stddef.h>

void* task = NULL;

void ktimer(void* position, void* state) {
  int flags = _get_task_flags(task);
  if (!(flags & TASK_CRITICAL_BIT)) {
    _save_task(position, state, task);
    // TODO: CHANGE TASK TO NEXT
    _load_task(position, state, task);
  }
}

