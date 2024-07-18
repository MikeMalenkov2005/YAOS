#include "kernel.h"
#include "screen.h"
#include "serial.h"
#include "memory.h"

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

int kmain() {
  if (init_console()) return -1;
  write_screen_mode();
  write_kernel_size();
  for (;;) {
    draw_screen();
  }
  return 0;
}

