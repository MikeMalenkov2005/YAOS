#include "kernel.h"
#include "screen.h"
#include "serial.h"
#include "memory.h"
#include "floppy.h"
#include "sound.h"

#define ABS(n) ((n) < 0 ? -(n) : (n))

static int stripes[] = {
  COLOR(255, 255, 255),
  COLOR(255, 255, 0),
  COLOR(0, 255, 255),
  COLOR(0, 255, 0),
  COLOR(255, 0, 255),
  COLOR(255, 0, 0),
  COLOR(0, 0, 255),
  COLOR(0, 0, 0)
};

int test_pattern(int x, int y, int w, int h, int th) {
  if (x < th || y < th || x >= w - th || y >= h - th) return COLOR(0, 255, 0);
  int cx = w >> 1;
  int cy = h >> 1;
  int or = cx > cy ? cy : cx;
  int ir = or - (th << 1);
  int dx = x - cx + (x < cx);
  int dy = y - cy + (y < cy);
  int dd = dx * dx + dy * dy;
  int si = (((dx + or + (x >= cx)) << 2) / (or + 1)) & 7;
  if (dd < ir * ir) return stripes[si];
  if (dd < or * or) return (si & 1) ? COLOR(255, 255, 255) : COLOR(127, 127, 127);
  if ((ABS(dx) % (or >> 1)) < th || (ABS(dy) % (or >> 1)) < th) return COLOR(255, 0, 0);
  return COLOR(0, 0, 0);
}

void plot_pixel(int x, int y, int color) {
  unsigned char* buffer = _get_screen_buffer();
  int bytes = (_get_screen_depth() + 7) >> 3;
  int index = x * bytes + y * _get_screen_pitch();
  for (int i = 0; i < bytes; ++i) {
    buffer[index + i] = (color >> (i << 3)) & 255;
  }
}

int init_console() {
  return _init_com_port(0, 9600, 8, UART_PARITY_NONE, 1);
}

#define num_to_hex(num) ((num) < 10 ? ((num) + '0') : ((num) - 10 + 'A'))

int write_hex(unsigned long number) {
  char buf[16];
  int i = 16;
  do {
    buf[--i] = num_to_hex(number & 15);
    number = number >> 4;
  } while (number);
  while (i < 16) if (_write_com_port(0, buf[i++]) < 0) return -1;
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

int write_screen_mode() {
  if (write_string("Mode: ") |
      write_hex(_get_screen_mode()) |
      write_string("\r\nBuffer: ") |
      write_hex((unsigned long)_get_screen_buffer()) |
      write_string("\r\nWidth: ") |
      write_decimal(_get_screen_width()) |
      write_string("\r\nHeight: ") |
      write_decimal(_get_screen_height()) |
      write_string("\r\nDepth: ") |
      write_decimal(_get_screen_depth()) |
      write_string("\r\nPitch: ") |
      write_decimal(_get_screen_pitch()) |
      write_string("\r\n")) return -1;
  return 0;
}

int write_kernel_size() {
  size_t fs = _get_kernel_file_size();
  size_t ms = _get_kernel_memory_size();
  if (write_string("Kernel file size: ") |
      write_decimal(fs / 1024 + (fs % 1024 > 0)) |
      write_string(" KiB\r\nKernel memory size: ") |
      write_decimal(ms / 1024 + (ms % 1024 > 0)) |
      write_string(" KiB\r\n")) return -1;
  return 0;
}

void draw_screen() {
  int w = _get_screen_width();
  int h = _get_screen_height();
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      plot_pixel(x, y, _get_screen_color(test_pattern(x, y, w, h, 4)));
    }
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  write_string(message);
}

void ktimer(void *position, void *state) {
}

int kmain() {
  if (init_console()) return -1;
  write_screen_mode();
  write_kernel_size();
  for (;;) {
    draw_screen();
  }
  return 0;
}

