#include "kernel.h"
#include "screen.h"
#include "serial.h"

#define ABS(n) ((n) < 0 ? -(n) : (n))

#define COLOR_BACKGROUND  169
#define COLOR_MOUSE       12

void draw_screen(int mouse_x, int mouse_y) {
  color_t *buffer = _get_screen_buffer();
  if (buffer) {
    int width = _get_screen_width();
    int height = _get_screen_height();
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        int index = x + y * width;
        if (ABS(x - mouse_x) < 3 && ABS(y - mouse_y) < 3) {
          buffer[index] = COLOR_MOUSE;
        }
        else buffer[index] = COLOR_BACKGROUND;
      }
    }
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  color_t *buffer = _get_screen_buffer();
  if (buffer) {
    int width = _get_screen_width();
    int height = _get_screen_height();
    for (int i = 0; i < width * height; ++i) buffer[i] = 1;
  }
}

static int timeout = 0;

void ktimer(void *position, void *state) {
  timeout++;
}

int kmain() {
  int x = 0, y = 0;
  for (;;) {
    draw_screen(x, y);
    int c = _wait_first_ps2();
    x += 2 * ((c == 0x7A || c == 0x74 || c == 0x7D) - (c == 0x69 || c == 0x6B || c == 0x6C));
    y += 2 * ((c == 0x69 || c == 0x72 || c == 0x7A) - (c == 0x6C || c == 0x75 || c == 0x7D));
    x = x < 0 ? 0 : (x > 319 ? 319 : x);
    y = y < 0 ? 0 : (y > 199 ? 199 : y);
    if (c == 0x76) return 0;
  }
  return 0;
}

