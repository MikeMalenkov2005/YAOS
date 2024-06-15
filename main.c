#include "screen.h"

void kclear(int color) {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    _set_cursor_pos(0);
    int size = _get_screen_width() * _get_screen_height();
    for (int i = 0; i < size; ++i) buffer[i] = color << 8;
  }
}

void kprint(const char *string, int color) {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    int pos = _get_cursor_pos();
    int width = _get_screen_width();
    int size = width * _get_screen_height();
    while (*string) {
      buffer[pos++] = *(string++) | (color << 8);
      if (pos >= size) {
        pos -= width;
        for (int i = 0; i < pos; ++i) buffer[i] = buffer[i + width];
      }
    }
    _set_cursor_pos(pos);
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  kprint(message, 4);
}

int kmain() {
  kclear(2);
  _enable_cursor();
  kprint("Yet Another Operating System!!!", 2);
  return 0;
}

