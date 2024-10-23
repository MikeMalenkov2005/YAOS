#include <apm.h>

#include <attributes.h>

#include <gdt.h>
#include <cpu.h>

/* NOT READY DUE TO INABILITY TO TEST WITH QEMU */

struct {
  uint32_t offset;
  uint16_t segment;
}__packed entry;
uint16_t version;
struct gdt_entry APM_LDT[4] __aligned(16);

int init_apm(struct apm_table* table) {
  entry.segment = GDT2SEG(1) | 4;
  entry.offset = table->entry;
  version = table->version;
  APM_LDT[0] = GDT_ENTRY(0, 0, 0, 0);
  APM_LDT[1] = GDT_ENTRY(table->cs32, table->cs32_len - 1, 0x9A, 4);
  APM_LDT[2] = GDT_ENTRY(table->cs16, table->cs16_len - 1, 0x9A, 0);
  APM_LDT[3] = GDT_ENTRY(table->ds, table->ds_len - 1, 0x92, 0);
  set_ldt(APM_LDT, 4);
  int error = 0;
  /* asm volatile ("lcall *%0" : : "m"(entry), "a"(0x530E), "b"(0), "c"(version));
  asm volatile ("sbb %%eax, %%eax" : "=a"(error)); */
  return error;
}

