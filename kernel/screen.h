#ifndef SCREEN_H
#define SCREEN_H

typedef unsigned short cchar_t;

extern void _enable_cursor();
extern void _disable_cursor();
extern int _get_cursor_pos();
extern void _set_cursor_pos(int pos);
extern cchar_t *_get_screen_buffer();
extern int _get_screen_width();
extern int _get_screen_height();

#endif

