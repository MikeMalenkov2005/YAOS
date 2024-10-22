#ifndef PCI_H
#define PCI_H

#include <stdint.h>

#define PCI_DEVICE(bus, slot, func) \
  ((uint16_t)(((bus) << 8) | (((slot) & 0x1F) << 3) | ((func) & 7)))

typedef void (*pci_device_enumerator)(uint16_t device, uint32_t interface);

uint32_t pci_read_config(uint16_t device, uint8_t offset);
uint16_t pci_read_config_word(uint16_t device, uint8_t offset);
uint8_t pci_read_config_byte(uint16_t device, uint8_t offset);

void pci_write_config(uint16_t device, uint8_t offset, uint32_t config);
void pci_write_config_word(uint16_t device, uint8_t offset, uint16_t config);
void pci_write_config_byte(uint16_t device, uint8_t offset, uint8_t config);

void pci_enumerate_devices(pci_device_enumerator enumerator);

#endif

