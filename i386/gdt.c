#include <gdt.h>
#include <cpu.h>

struct gdt_entry GDT[TOTAL_GDT_ENTRIES] __attribute__((aligned(16)));

void set_gdt_entry(int index, uint32_t base, uint32_t limit, int access, int flags) {
  GDT[index].limit_low = limit & 0xFFFF;
  GDT[index].base_low = base & 0xFFFF;
  GDT[index].base_mid = (base >> 16) & 0xFF;
  GDT[index].access_byte = access;
  GDT[index].limit_high = ((limit >> 16) & 15) | (flags << 4);
  GDT[index].base_high = base >> 24;
}

void init_gdt(struct tss_struct* tss, uint32_t tss_limit) {
  set_gdt_entry(NULL_GDT_INDEX, 0, 0, 0, 0);
  set_gdt_entry(KERNEL_CODE_GDT_INDEX, 0, 0xFFFFF, 0x9A, 0xC);
  set_gdt_entry(KERNEL_DATA_GDT_INDEX, 0, 0xFFFFF, 0x92, 0xC);
  set_gdt_entry(USER_CODE_GDT_INDEX, 0, 0xFFFFF, 0xFA, 0xC);
  set_gdt_entry(USER_DATA_GDT_INDEX, 0, 0xFFFFF, 0xF2, 0xC);
  set_gdt_entry(TSS_GDT_INDEX, (uint32_t)tss, tss_limit, 0x89, 0);
  lgdt(&GDT, sizeof(GDT) - 1);
  lcs(GDT2SEG(KERNEL_CODE_GDT_INDEX));
  lds(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  les(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  lfs(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  lgs(GDT2SEG(KERNEL_DATA_GDT_INDEX));
  tss->ss0 = GDT2SEG(KERNEL_DATA_GDT_INDEX);
  ltr(GDT2SEG(TSS_GDT_INDEX));
}
