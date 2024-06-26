#include "kernel.h"
#include "screen.h"
#include "serial.h"
#include "sound.h"

#include <stddef.h>

#define ABS(n) ((n) < 0 ? -(n) : (n))

#define COLOR_BACKGROUND  169
#define COLOR_MOUSE       12

void draw_screen(int mouse_x, int mouse_y) {
  color_t *buffer = _get_screen_buffer();
  if (buffer) {
    int pitch = _get_screen_pitch();
    int width = _get_screen_width();
    int height = _get_screen_height();
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        int index = x + y * pitch;
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
    int pitch = _get_screen_pitch();
    int height = _get_screen_height();
    for (int i = 0; i < pitch * height; ++i) buffer[i] = 1;
  }
}

static int timeout = 0;

void ktimer(void *position, void *state) {
  timeout++;
}

int com_print(int index, const char *string) {
  while (*string) {
    for (int i = 0; _write_com_port(index, *string) < 0; ++i) {
      if (i == 1000) return -1;
    }
    string++;
  }
  return 0;
}

#define num_to_char(n) ((n) < 10 ? (n) + '0' : (n) + 'A' - 10)

int kmain() {
  _init_com_port(0, 115200, 8, UART_PARITY_NONE, 1);
  com_print(0, "SVGA Buffer: ");
  color_t *buffer = _get_screen_buffer();
  for (int i = 0; i < 8; ++i) {
    int n = (((size_t)buffer) >> ((7 - i) << 2)) & 15;
    _write_com_port(0, num_to_char(n));
  }
  com_print(0, "\r\nSVGA Pitch: ");
  int pitch = _get_screen_pitch();
  for (int i = 0; i < 4; ++i) {
    int n = (pitch >> ((3 - i) << 2)) & 15;
    _write_com_port(0, num_to_char(n));
  }
  com_print(0, "\r\nSVGA Width: ");
  int width = _get_screen_width();
  for (int i = 0; i < 4; ++i) {
    int n = (width >> ((3 - i) << 2)) & 15;
    _write_com_port(0, num_to_char(n));
  }
  com_print(0, "\r\nSVGA Height: ");
  int height = _get_screen_height();
  for (int i = 0; i < 4; ++i) {
    int n = (height >> ((3 - i) << 2)) & 15;
    _write_com_port(0, num_to_char(n));
  }
  com_print(0, "\r\n");
  for (;;) {
    draw_screen(10, 10);
    int c = _read_first_ps2();
    if (c >= 0) {
      if (c == 0x29) _play_sound(1000);
      if (c == 0x76) return 0;
      if (c == 0xF0) {
        c = _wait_first_ps2();
        if (c == 0x29) _play_sound(0);
      }
    }
  }
  return 0;
}

