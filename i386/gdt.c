#include <gdt.h>
#include <cpu.h>

struct gdt_entry GDT[TOTAL_GDT_ENTRIES] __aligned(16);

void init_gdt(struct tss_struct* tss, uint32_t tss_limit) {
  GDT[NULL_GDT_INDEX] = GDT_ENTRY(0, 0, 0, 0);
  GDT[KERNEL_CODE_GDT_INDEX] = GDT_ENTRY(0, 0xFFFFF, 0x9A, 0xC);
  GDT[KERNEL_DATA_GDT_INDEX] = GDT_ENTRY(0, 0xFFFFF, 0x92, 0xC);
  GDT[USER_CODE_GDT_INDEX] = GDT_ENTRY(0, 0xFFFFF, 0xFA, 0xC);
  GDT[USER_DATA_GDT_INDEX] = GDT_ENTRY(0, 0xFFFFF, 0xF2, 0xC);
  GDT[TSS_GDT_INDEX] = GDT_ENTRY((uint32_t)tss, tss_limit, 0x89, 0);
  GDT[LDT_GDT_INDEX] = GDT_ENTRY(-1, 0, 0, 0);
  tss->ss0 = GDT2SEG(KERNEL_DATA_GDT_INDEX);
  lgdt(&GDT, sizeof(GDT) - 1);
  lcs(GDT2SEG(KERNEL_CODE_GDT_INDEX));
  lds(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  les(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  lfs(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  lgs(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  tss->ss0 = GDT2SEG(KERNEL_DATA_GDT_INDEX);
  ltr(GDT2SEG(TSS_GDT_INDEX));
  set_ldt(NULL, 0);
}

void set_ldt(struct gdt_entry* table, uint8_t length) {
  if (GDT_ENTRY_BASE(GDT[LDT_GDT_INDEX]) != (uint32_t)table) {
    if (!table || !length) GDT[LDT_GDT_INDEX] = GDT_ENTRY(0, 0, 0x82, 0);
    else GDT[LDT_GDT_INDEX] = GDT_ENTRY((uint32_t)table, length * sizeof(*table) - 1, 0x82, 0);
    uint16_t selector = GDT2SEG(LDT_GDT_INDEX);
    asm volatile ("lldt %0" : : "r"(selector));
  }
}

