#ifndef SCREEN_H
#define SCREEN_H

typedef unsigned char color_t;

extern color_t *_get_screen_buffer();
extern int _get_screen_pitch();
extern int _get_screen_width();
extern int _get_screen_height();

#endif

