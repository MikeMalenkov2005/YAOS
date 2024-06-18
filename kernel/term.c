#include "term.h"
#include "input.h"

static int color = 7;

void kclear() {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    _set_cursor_pos(0);
    int size = _get_screen_width() * _get_screen_height();
    for (int i = 0; i < size; ++i) buffer[i] = color << 8;
  }
}

void kprint(const char *string) {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    int pos = _get_cursor_pos();
    int width = _get_screen_width();
    int size = width * _get_screen_height();
    while (*string) {
      unsigned char c = *(string++);
      switch (c) {
        case '\b':
          if (!pos) break;
          if (buffer[--pos] & 0xFF) {
            buffer[pos] = color << 8;
            break;
          }
          while (!(buffer[--pos] & 0xFF) && ((pos + 1) % width));
          ++pos;
          break;
        case '\n':
          pos += width;
          break;
        case '\r':
          pos -= pos % width;
          break;
        default:
          buffer[pos++] = c | (color << 8);
          break;
      }
      if (pos >= size) {
        pos -= width;
        for (int i = 0; i < pos; ++i) buffer[i] = buffer[i + width];
        for (int i = 0; i < width; ++i) buffer[pos + i] = color << 8;
      }
    }
    buffer[pos] = color << 8;
    _set_cursor_pos(pos);
  }
}

static unsigned char keymap_normal[0x80] =
"\x00\0001234567890-=\b\tQWERTYUIOP[]\r\000ASDFGHJKL;'`\x00\\ZXCVBNM,./\x00*"
"\x00 \x00\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xE0\x00\000789-456+1230.\x00"
"\x00\x00\xE1\xE2\x00\x00\x00\x00";

static int mods = 0;

int kscan() {
  int c = _read_first_ps2();
  if (c < 0) return 0;
  if (c == 0xE0) {
    for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
    return 0;
  }
  if (c == 0xE1) {
    for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
    for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
    return 0;
  }
  int vk = keymap_normal[c & 0x7F];
  return (c & 0x80) ? 0 : vk;
}

