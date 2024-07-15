#include "kernel.h"
#include "screen.h"
#include "serial.h"
#include "floppy.h"
#include "sound.h"

#include <stddef.h>

void plot_pixel(int x, int y, int screen_color) {
  if (x < 0 || y < 0) return;
  unsigned char* buffer = _get_screen_buffer();
  if (!buffer) return;
  int width = _get_screen_width();
  int height = _get_screen_height();
  if (x >= width || y >= height) return;
  int offset = x + y * _get_screen_pitch();
  int depth = _get_screen_depth();
  for (int bytes = (_get_screen_depth() + 7) >> 3; bytes > 0; --bytes) {
    buffer[offset++] = screen_color & 255;
    screen_color = screen_color >> 8;
  }
}

void fill_rect(int x, int y, int width, int height, int screen_color) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      plot_pixel(x + j, y + i, screen_color);
    }
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  fill_rect(0, 0, _get_screen_width(), _get_screen_height(), _get_screen_color(COLOR(255, 0, 0)));
}

void ktimer(void *position, void *state) {
}

int init_console() {
  return _init_com_port(0, 9600, 8, UART_PARITY_NONE, 1);
}

#define num_to_hex(num) ((num) < 10 ? ((num) + '0') : ((num) - 10 + 'A'))

int write_hex(unsigned int number) {
  char buf[8];
  int i = 8;
  do {
    buf[--i] = num_to_hex(number & 15);
    number = number >> 4;
  } while (number);
  while (i < 8) if (_write_com_port(0, buf[i++]) < 0) return -1;
  return 0;
}

int write_decimal(int number) {
  if (number < 0) {
    if (_write_com_port(0, '-') < 0) return -1;
    number = -number;
  }
  char buf[16];
  int i = 16;
  do {
    buf[--i] = number % 10 + '0';
    number = number / 10;
  } while (number);
  while (i < 16) if (_write_com_port(0, buf[i++]) < 0) return -1;
  return 0;
}

int write_string(const char* string) {
  while (*string) if (_write_com_port(0, *(string++)) < 0) return -1;
  return 0;
}

int kmain() {
  if (init_console()) return -1;
  write_string("Mode: ");
  write_hex(_get_screen_mode());
  write_string("\r\nWidth: ");
  write_decimal(_get_screen_width());
  write_string("\r\nHeight: ");
  write_decimal(_get_screen_height());
  write_string("\r\nDepth: ");
  write_decimal(_get_screen_depth());
  write_string("\r\nPitch: ");
  write_decimal(_get_screen_pitch());
  //fill_rect(0, 0, _get_screen_width(), _get_screen_height(), 0xF);
  for (;;);
  return 0;
}

