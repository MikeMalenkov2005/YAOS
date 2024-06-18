#ifndef KERNEL_H
#define KERNEL_H

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
void kerror(void *position, void *state, const char *message);
void ktimer(void *position, void *state);
int kmain();

#endif

