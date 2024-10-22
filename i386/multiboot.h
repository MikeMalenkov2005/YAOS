#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#include <attributes.h>

#define MB_MEMORY_BIT (1 << 0)
#define MB_DEVICE_BIT (1 << 1)
#define MB_CMD_BIT    (1 << 2)
#define MB_MODS_BIT   (1 << 3)
#define MB_SYMS_A_BIT (1 << 4)
#define MB_SYMS_B_BIT (1 << 5)
#define MB_MEMMAP_BIT (1 << 6)
#define MB_DIRVES_BIT (1 << 7)
#define MB_CONFIG_BIT (1 << 8)
#define MB_BLNAME_BIT (1 << 9)
#define MB_APM_BIT    (1 << 10)
#define MB_VBE_BIT    (1 << 11)
#define MB_VIDEO_BIT  (1 << 12)

struct boot_info {
  uint32_t flags;
  struct {
    uint32_t lower;
    uint32_t upper;
  } mem;
  uint32_t boot_dev;
  uint32_t cmdline;
  struct {
    uint32_t count;
    uint32_t addr;
  } mods;
  uint32_t syms[4];
  struct {
    uint32_t length;
    uint32_t addr;
  } mmap;
  struct {
    uint32_t length;
    uint32_t addr;
  } drives;
  uint32_t config_table;
  uint32_t loader_name;
  uint32_t apm_table;
  struct {
    uint32_t control_info;
    uint32_t mode_info;
    uint16_t mode;
    struct {
      uint16_t seg;
      uint16_t off;
      uint16_t len;
    } interface;
  } vbe;
  struct {
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t type;
    uint8_t color_info[6];
  }__packed framebuffer;
};

#endif
