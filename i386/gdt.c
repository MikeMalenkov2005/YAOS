#include "gdt.h"

__attribute__((aligned(16)))
struct gdt_entry GDT[6];

__attribute__((aligned(16)))
struct tss TSS;

extern void* STACK;

void _init_gdt() {
  _set_gdt_entry(0, 0, 0, 0, 0);
  _set_gdt_entry(1, 0, 0xFFFFF, 0x9A, 0xC);
  _set_gdt_entry(2, 0, 0xFFFFF, 0x92, 0xC);
  _set_gdt_entry(3, 0, 0xFFFFF, 0xFA, 0xC);
  _set_gdt_entry(4, 0, 0xFFFFF, 0xF2, 0xC);
  _set_gdt_entry(5, (size_t)&TSS, sizeof(struct tss) - 1, 0x89, 0);
  TSS = (struct tss){ 0 };
  TSS.sp0 = (size_t)STACK;
  TSS.ss0 = KDATA;
  TSS.sp = (size_t)STACK;
  TSS.ss = KDATA | 3;
  TSS.ds = KDATA | 3;
  TSS.es = KDATA | 3;
  TSS.fs = KDATA | 3;
  TSS.gs = KDATA | 3;
  TSS.cs = KCODE | 3;
  lgdt(GDT, sizeof(GDT) - 1);
  lds(KDATA);
  les(KDATA);
  lfs(KDATA);
  lgs(KDATA);
  lcs(KCODE);
  ltr(0x28 | 3);
}

void _set_gdt_entry(int index, size_t offset, uint32_t limit, uint8_t access, uint8_t flags) {
  GDT[index].base_low = offset & 0xFFFF;
  GDT[index].base_mid = (offset >> 16) & 0xFF;
  GDT[index].base_high = offset >> 24;
  GDT[index].limit_low = limit & 0xFFFF;
  GDT[index].limit_high = ((limit >> 16) & 15) | (flags << 4);
  GDT[index].access_byte = access;
}

