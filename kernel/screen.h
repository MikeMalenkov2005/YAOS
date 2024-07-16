#ifndef SCREEN_H
#define SCREEN_H

extern void* _get_screen_buffer();
extern int _get_screen_pitch();
extern int _get_screen_width();
extern int _get_screen_height();
extern int _get_screen_depth();
extern int _get_screen_mode();
extern int _get_screen_color(int color);

#define COLOR(red, green, blue) ((((red) & 255) << 16) | (((green) & 255) << 8) | ((blue) & 255))
#define RED(color) (((color) >> 16) & 255)
#define GREEN(color) (((color) >> 8) & 255)
#define BLUE(color) ((color) & 255)

#endif

