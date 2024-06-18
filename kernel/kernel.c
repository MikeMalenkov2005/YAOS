#include "kernel.h"
#include "memory.h"
#include "term.h"

void print_hex(unsigned int num) {
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
  kprint(&string[i]);
}

void print_dec(unsigned int num) {
  char string[11];
  int i = sizeof(string);
  string[--i] = 0;
  do {
    string[--i] = (num % 10) + '0';
    num = num / 10;
  } while (num);
  kprint(&string[i]);
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  kprint(message);
}

static int timeout = 0;

void ktimer(void *position, void *state) {
  timeout++;
}

int kmain() {
  kclear();
  _enable_cursor();
  kprint("Yet Another Operating System!\r\nKernel file size: ");
  print_dec((_get_kernel_file_size() + 1023) / 1024);
  kprint("KiB.\r\nKernel memory usage: ");
  print_dec((_get_kernel_memory_size() + 1023) / 1024);
  kprint("KiB.\r\n");
  for (int c = kscan(); c >= 0; c = kscan()) {
    if (c == '\r') kprint("\r\n");
    else if (c > 0 && c < 0x100) {
      char tmp[2];
      tmp[0] = c;
      tmp[1] = 0;
      kprint(tmp);
    }
  }
  return 0;
}

