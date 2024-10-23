#ifndef APM_H
#define APM_H

#include <stdint.h>

/* NOT READY DUE TO INABILITY TO TEST WITH QEMU */

struct apm_table {
  uint16_t version;
  uint16_t cs32;
  uint32_t entry;
  uint16_t cs16;
  uint16_t ds;
  uint16_t cs32_len;
  uint16_t cs16_len;
  uint16_t ds_len;
};

int init_apm(struct apm_table* table);

#endif

