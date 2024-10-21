#ifndef APM_H
#define APM_H

#include <stdint.h>

/* NOT READY */

struct apm_table {
  uint16_t version;
  uint16_t cs32;
  uint32_t entry;
  uint16_t cs16;
  uint16_t ds;
  uint16_t cs32_len;
  uint16_t cs16_len;
  uint16_t ds_len;
}__attribute__((packed));

#endif

