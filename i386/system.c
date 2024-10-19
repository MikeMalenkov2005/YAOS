#include <multiboot.h>
#include <cpu.h>
#include <gdt.h>
#include <idt.h>

struct tss_struct TSS __attribute__((aligned(16)));

struct screen_mode {
  void* buffer;
  int pitch;
  int width;
  int height;
} screen_mode;

void mzero(void* buffer, size_t size) {
  void* finish = buffer + size;
  while (buffer < finish) *(char*)(buffer++) = 0;
}

void kstart(struct boot_info* info, uint32_t stack) {
  mzero(&TSS, sizeof(TSS));
  TSS.iopb = sizeof(TSS);
  TSS.sp0 = stack;
  init_gdt(&TSS, sizeof(TSS) - 1);
  init_idt(GDT2SEG(KERNEL_CODE_GDT_INDEX));
  screen_mode.buffer = (info->flags & MB_VIDEO_BIT) ? (void*)(size_t)info->framebuffer.addr : (void*)0xB8000;
  screen_mode.pitch = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.pitch : 160;
  screen_mode.width = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.width : 80;
  screen_mode.height = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.height : 25;
  mzero(screen_mode.buffer, screen_mode.pitch * screen_mode.height);
  for (;;);
}

