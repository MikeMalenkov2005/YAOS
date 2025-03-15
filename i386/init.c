#include "tss.h"
#include <multiboot.h>
#include <cpu.h>
#include <gdt.h>
#include <idt.h>
#include <pic.h>
#include <pit.h>
#include <fpu.h>
#include <pci.h>
#include <mmu.h>

#include <io.h>

#include <cpuid.h>

#include <attributes.h>
#include <main.h>

#include <stdint.h>

struct screen_mode {
  void* buffer;
  uint16_t pitch;
  uint16_t width;
  uint16_t height;
  uint16_t bpp;
} screen_mode;

void invalid_opcode_handler(struct isr_frame* frame) {
  if (*(uint16_t*)frame->frame.ip == 0xA20F) {
    /* Handle unsupported CPUID instruction */
    frame->frame.ip += 2;
    frame->state.bx = 0;
    frame->state.dx = 0;
    frame->state.cx = 0;
    frame->state.ax = 0;
  }
  // print usupported instruction and end task
}

void print_decimal(int x, int y, unsigned int d) {
  if (screen_mode.width >= 320) return;
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
  if (screen_mode.width >= 320) return;
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

int pci_device_line = 0;

void pci_device_installer(uint16_t device, uint32_t id, uint32_t interface) {
  print_hex(0, pci_device_line, device, 4);
  print_hex(5, pci_device_line, interface, 8);
  print_hex(14, pci_device_line++, id, 8);
  if (interface >> 24 == 3) {
    switch (id) {
      case 0x11111234:
        /*{
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
          size_t lfb = (((size_t)(bar0 & ~15)) | (((size_t)(bar0 & 8 ? pci_read_config(device, 0x14) : 0)) << 32));
          screen_mode.buffer = (void*)lfb;
          outw(0x1CE, 1);
          screen_mode.width = inw(0x1CF);
          outw(0x1CE, 2);
          screen_mode.height = inw(0x1CF);
          outw(0x1CE, 3);
          screen_mode.bpp = inw(0x1CF);
          screen_mode.pitch = (screen_mode.width * screen_mode.bpp + 7) >> 3;
          for (int i = 0; i < screen_mode.width * screen_mode.pitch; i += PAGE_SIZE) mmu_map_page(lfb + i, lfb + i, 0);
          for (int i = 0; i < screen_mode.width * screen_mode.height; ++i) ((size_t*)lfb)[i] = 0x00FF00FF;
        } break;*/
      default:
        print_hex(0, pci_device_line, pci_read_config(device, 0x10), 8);
        print_hex(9, pci_device_line, pci_read_config(device, 0x14), 8);
        print_hex(18, pci_device_line++, pci_read_config(device, 0x18), 8);
        print_hex(0, pci_device_line, pci_read_config(device, 0x1C), 8);
        print_hex(9, pci_device_line, pci_read_config(device, 0x20), 8);
        print_hex(18, pci_device_line++, pci_read_config(device, 0x24), 8);
        break;
    }
  }
}

struct isr128_frame {
  struct saved_registers state;
  struct interrupt_frame frame;
};

void __naked test() {
  asm volatile ("jmp %P0" : : "i"(test));
}

void key_test(struct irq_frame frame) {
  static unsigned int scancode = 0;
  scancode = (scancode << 8) | inb(0x60);
  print_hex(20, 20, scancode, 2);
}

void kinit(struct interrupt_frame* frame, struct boot_info* info) {
  init_gdt(init_tss(frame), sizeof(struct tss_struct) - 1);
  init_idt(GDT2SEG(KERNEL_CODE_GDT_INDEX));
  set_cpu_isr(6, invalid_opcode_handler);
  init_pit(1000);
  init_mmu(info->flags & MB_MEMORY_BIT ? (void*)(((size_t)info->mem.upper << 10) + 0x100000) : (void*)0xF00000);

  screen_mode.buffer = (info->flags & MB_VIDEO_BIT) ? (void*)(size_t)info->framebuffer.addr : (void*)0xB8000;
  screen_mode.pitch = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.pitch : 160;
  screen_mode.width = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.width : 80;
  screen_mode.height = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.height : 25;
  screen_mode.bpp = (info->flags & MB_VIDEO_BIT) ? (int)info->framebuffer.bpp : 16;
  for (char* p = screen_mode.buffer; p < (char*)(screen_mode.buffer + screen_mode.pitch * screen_mode.height); *p++ = 0);

  int kernel_flags = 0;
  if (!__get_cpuid_max(0, NULL)) kernel_flags |= 2;
  if (!init_fpu()) kernel_flags |= 1;

  pci_enumerate_devices(pci_device_installer);

  set_pic_irqsr(1, key_test);
  set_pic_mask(get_pic_mask() & ~2);

  int status = kmain();

  for (;;);

  /* TODO: Setup first user-mode process (shell) */

  frame->ip = (size_t)test;
  frame->cs = GDT2SEG(USER_CODE_GDT_INDEX) | 3;
  frame->flags = rdflags();
  frame->ss = GDT2SEG(USER_DATA_GDT_INDEX) | 3;
}

