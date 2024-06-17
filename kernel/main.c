#include "screen.h"
#include "memory.h"

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
      unsigned char c = *(string++);
      buffer[pos++] = c | (color << 8);
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

void print_hex(unsigned int num, int color) {
  char string[11];
  int i = sizeof(string);
  string[--i] = 0;
  do {
    int d = num & 15;
    string[--i] = d < 10 ? d + '0' : d - 10 + 'A';
    num = num >> 4;
  } while (num);
  string[--i] = 'x';
  string[--i] = '0';
  kprint(&string[i], color);
}

void print_dec(unsigned int num, int color) {
  char string[11];
  int i = sizeof(string);
  string[--i] = 0;
  do {
    string[--i] = (num % 10) + '0';
    num = num / 10;
  } while (num);
  kprint(&string[i], color);
}

extern int _read_first_ps2();

int kmain() {
  kclear(2);
  _enable_cursor();
  kprint("Yet Another Operating System! File size: ", 2);
  print_dec((_get_kernel_file_size() + 1023) / 1024, 2);
  kprint("KiB. Full size: ", 2);
  print_dec((_get_kernel_memory_size() + 1023) / 1024, 2);
  kprint("KiB. >:) ", 2);
  for (int c = _read_first_ps2(); c != 0x81; c = _read_first_ps2()) {
    if (c > 0) {
      char tmp[2];
      tmp[0] = c;
      tmp[1] = 0;
      kprint(tmp, 2);
    }
  }
  return 0;
}

