#ifndef PIT_H
#define PIT_H

#include <stdint.h>

/* NOT READY */

#define PIT_BASE_FREQUENCY  1193182

#define PIT_MAX_FREQUENCY (BASE_FREQUENCY >> 1)
#define PIT_MIN_FREQUENCY (BASE_FREQUENCY >> 16)

void init_pit(uint32_t frequency);
void pit_sound(uint32_t frequency);

#endif

