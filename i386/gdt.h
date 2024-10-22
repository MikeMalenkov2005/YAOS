#ifndef GDT_H
#define GDT_H

#include <tss.h>

#include <attributes.h>

inline static void lgdt(void* base, uint16_t limit) {
  struct {
    uint16_t limit;
    void* base;
  }__packed gdtr = { limit, base };
  asm volatile ("lgdt %0" : : "m"(gdtr));
}

struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access_byte;
  uint8_t limit_high;
  uint8_t base_high;
};

#define NULL_GDT_INDEX        0
#define KERNEL_CODE_GDT_INDEX 1
#define KERNEL_DATA_GDT_INDEX 2
#define USER_CODE_GDT_INDEX   3
#define USER_DATA_GDT_INDEX   4
#define TSS_GDT_INDEX         5
#define LDT_GDT_INDEX         6
#define TOTAL_GDT_ENTRIES     7

#define GDT2SEG(index)  (index << 3)

void init_gdt(struct tss_struct* tss, uint32_t tss_limit);

#endif
