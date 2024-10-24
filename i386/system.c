#include <multiboot.h>
#include <cpu.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <fpu.h>
#include <pci.h>

#include <io.h>

#include <cpuid.h>

#include <attributes.h>
#include <stdint.h>

struct tss_struct TSS __aligned(16);

struct screen_mode {
  void* buffer;
  uint16_t pitch;
  uint16_t width;
  uint16_t height;
  uint16_t bpp;
} screen_mode;

void mzero(void* buffer, size_t size) {
  void* finish = buffer + size;
  while (buffer < finish) *(char*)(buffer++) = 0;
}

void invalid_opcode_handler(struct isr_frame* frame) {
  if (*(uint16_t*)frame->frame.ip == 0xA20F) {
    /* Handle unsupported CPUID instruction */
    frame->frame.ip += 2;
    frame->state.bx = 0;
    frame->state.dx = 0;
    frame->state.cx = 0;
    frame->state.ax = 0;
  }
}

void print_decimal(int x, int y, unsigned int d) {
  char s[10];
  int i = sizeof(s);
  do {
    s[--i] = d % 10 + '0';
    d = d / 10;
  } while (d);
  for (int j = i; j < sizeof(s); ++j) {
    ((uint16_t*)screen_mode.buffer)[y * screen_mode.width + x + j - i] = s[j] | 0x200;
  }
}

void print_hex(int x, int y, unsigned int h, int p) {
  char s[8];
  int i = sizeof(s);
  do {
    int d = h & 15;
    s[--i] = d + (d < 10 ? '0' : 'A' - 10);
    h = h >> 4;
    if (p) p--;
  } while (h || p);
  for (int j = i; j < sizeof(s); ++j) {
    ((uint16_t*)screen_mode.buffer)[y * screen_mode.width + x + j - i] = s[j] | 0x200;
  }
}

int pci_device_line;

void pci_device_installer(uint16_t device, uint32_t id, uint32_t interface) {
  print_hex(0, pci_device_line, device, 4);
  print_hex(5, pci_device_line, interface, 8);
  print_hex(14, pci_device_line++, id, 8);
  if (id == 0x11111234) {
    /* SETUP BOCHS GRAPHICS ADAPTER */
    outw(0x1CE, 4);
    outw(0x1CF, 0);
    outw(0x1CE, 1);
    outw(0x1CF, 640);
    outw(0x1CE, 2);
    outw(0x1CF, 480);
    outw(0x1CE, 3);
    outw(0x1CF, 32);
    outw(0x1CE, 4);
    outw(0x1CF, 0x41);
    uint32_t bar0 = pci_read_config(device, 0x10);
    uint32_t* lfb = (void*)(((size_t)(bar0 & ~15)) | (((size_t)(bar0 & 8 ? pci_read_config(device, 0x14) : 0)) << 32));
    screen_mode.buffer = lfb;
    outw(0x1CE, 1);
    screen_mode.width = inw(0x1CF);
    outw(0x1CE, 2);
    screen_mode.height = inw(0x1CF);
    outw(0x1CE, 3);
    screen_mode.bpp = inw(0x1CF);
    screen_mode.pitch = (screen_mode.width * screen_mode.bpp + 7) >> 3;
    for (int i = 0; i < screen_mode.width * screen_mode.height; ++i) lfb[i] = 0xFF00FF;
  }
}

void kinit(struct boot_info* info, uint32_t stack) {
  mzero(&TSS, sizeof(TSS));
  TSS.iopb = sizeof(TSS);
  TSS.sp0 = stack;
  init_gdt(&TSS, sizeof(TSS) - 1);
  init_idt(GDT2SEG(KERNEL_CODE_GDT_INDEX));
  set_cpu_isr(6, invalid_opcode_handler);
  init_pit(1000);

  screen_mode.buffer = (info->flags & MB_VIDEO_BIT) ? (void*)(size_t)info->framebuffer.addr : (void*)0xB8000;
  screen_mode.pitch = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.pitch : 160;
  screen_mode.width = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.width : 80;
  screen_mode.height = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.height : 25;
  screen_mode.bpp = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.bpp : 16;
  mzero(screen_mode.buffer, screen_mode.pitch * screen_mode.height);

  if (init_fpu()) {
    *(uint64_t*)screen_mode.buffer = UINT64_C(0x0220025502500246);
  }

  pci_device_line = 2;
  pci_enumerate_devices(pci_device_installer);

  if (__get_cpuid_max(0, NULL)) {
    uint32_t unused;
    char vendor[12];
    __get_cpuid(0, &unused, (void*)&vendor[0], (void*)&vendor[8], (void*)&vendor[4]);
    for (int i = 0; i < sizeof(vendor); ++i) {
      ((uint16_t*)(screen_mode.buffer + screen_mode.pitch))[i] = vendor[i] | 0x0200;
    }
  }

  for (;;);
}

