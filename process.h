#ifndef PROCESS_H
#define PROCESS_H

extern void *_get_process();
extern void _save_process(void *position, void *state);
extern void _load_process(void *position, void *state, void *process);

#endif

