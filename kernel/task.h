#ifndef PROCESS_H
#define PROCESS_H

extern void _save_task(void* position, void* state, void* task);
extern void _load_task(void* position, void* state, void* task);
extern int _get_task_flags(void* task);
extern void _set_task_flags(void* task, int flags);

#define TASK_CRITICAL_BIT (1 << 0)
#define TASK_PAUSED_BIT   (1 << 1)

#endif

