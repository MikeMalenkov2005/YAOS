#ifndef KERNEL_H
#define KERNEL_H

extern int _get_core_index();

extern void _init_process_system();
extern void *_get_process();
extern void _save_process(void *position, void *state);
extern void _load_process(void *position, void *state, void *process);

#endif

