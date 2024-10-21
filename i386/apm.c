#include <apm.h>

#include <gdt.h>
#include <cpu.h>

/* NOT READY */

struct gdt_entry APM_LDT[4] __attribute__((aligned(16)));

void set_apm_segment(int index, uint32_t base, uint16_t limit, uint8_t flags, uint8_t access) {
  APM_LDT[index].limit_low = limit;
  APM_LDT[index].base_low = base & 0xFFFF;
  APM_LDT[index].base_mid = (base >> 16) & 0xFF;
  APM_LDT[index].access_byte = access;
  APM_LDT[index].limit_high = flags << 4;
  APM_LDT[index].base_high = base >> 24;
}

void init_apm(struct apm_table* table) {
  set_apm_segment(0, 0, 0, 0, 0);
  set_apm_segment(1, table->cs32, table->cs32_len - 1, 4, 0x9A);
  set_apm_segment(2, table->cs16, table->cs16_len - 1, 0, 0x9A);
  set_apm_segment(3, table->ds, table->ds_len, 0, 0x92);
}

