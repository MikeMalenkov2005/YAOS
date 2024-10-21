#ifndef PCI_H
#define PCI_H

#include <io.h>

/* NOT READY */

/* Only support one mode of addressing (NOT deprecated) */

#define PCI_CFG_ADDR_PORT 0xCF8
#define PCI_CFG_DATA_PORT 0xCFC

inline static uint32_t pci_read_cfg_reg(int bus, int slot, int func, int offset) {
  uint32_t address = ((bus & 0xFF) << 16) | ((slot & 0x1F) << 11) |
      ((func & 7) << 8) | (offset & 0xFC) | UINT32_C(0x80000000);
  outl(PCI_CFG_ADDR_PORT, address);
  return inl(PCI_CFG_DATA_PORT);
}

#define pci_read_cfg_word(bus, slot, func, offset) \
  ((uint16_t)((pci_read_cfg_reg(bus, slot, func, offset) >> (((offset) & 2) << 3)) & 0xFFFF))

#define pci_read_cfg_byte(bus, slot, func, offset) \
  ((uint8_t)((pci_read_cfg_reg(bus, slot, func, offset) >> (((offset) & 3) << 3)) & 0xFF))

#define pci_get_vendor_id(bus, slot) pci_read_cfg_word(bus, slot, 0, 0)
#define pci_get_device_id(bus, slot) pci_read_cfg_word(bus, slot, 0, 2)
#define pci_get_function_info(bus, slot, func) pci_read_cfg_reg(bus, slot, func, 8)

#endif

