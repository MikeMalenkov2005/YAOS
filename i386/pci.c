#include <pci.h>

#include <io.h>

#define PCI_CONFIG_ADDRESS(device, offset) \
  (UINT32_C(0x80000000) | (((offset) & 15) << 24) | ((device) << 8) | ((offset) & 0xFC))

uint32_t pci_read_config(uint16_t device, uint16_t offset) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  return inl(0xCFC);
}

uint16_t pci_read_config_word(uint16_t device, uint16_t offset) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  return inw(0xCFC + (offset & 2));
}

uint8_t pci_read_config_byte(uint16_t device, uint16_t offset) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  return inb(0xCFC + (offset & 3));
}

void pci_write_config(uint16_t device, uint16_t offset, uint32_t config) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  outl(0xCFC, config);
}

void pci_write_config_word(uint16_t device, uint16_t offset, uint16_t config) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  outw(0xCFC + (offset & 2), config);
}

void pci_write_config_byte(uint16_t device, uint16_t offset, uint8_t config) {
  outl(0xCF8, PCI_CONFIG_ADDRESS(device, offset));
  outb(0xCFC + (offset & 3), config);
}

void pci_enumerate_bus(int bus, pci_device_enumerator enumerator);

void pci_enumerate_device(uint16_t device, pci_device_enumerator enumerator) {
  uint32_t interface = pci_read_config(device, 8);
  if ((interface >> 16) == 0x0604) {
    uint8_t bus = pci_read_config_byte(device, 0x19);
    pci_enumerate_bus(bus, enumerator);
  }
  else enumerator(device, interface);
}

void pci_enumerate_slot(int bus, int slot, pci_device_enumerator enumerator) {
  uint16_t device = PCI_DEVICE(bus, slot, 0);
  uint16_t vendor = pci_read_config_word(device, 0);
  if (vendor != 0xFFFF) {
    uint8_t ht = pci_read_config_byte(device, 14);
    if (ht & 0x80) {
      for (int fn = 0; fn < 8; ++fn) {
        if (pci_read_config_word(device | fn, 0) != 0xFFFF) {
          pci_enumerate_device(device | fn, enumerator);
        }
      }
    }
    else pci_enumerate_device(device, enumerator);
  }
}

void pci_enumerate_bus(int bus, pci_device_enumerator enumerator) {
  for (int slot = 0; slot < 32; ++slot) {
    pci_enumerate_slot(bus, slot, enumerator);
  }
}

void pci_enumerate_devices(pci_device_enumerator enumerator) {
  uint8_t ht = pci_read_config_byte(0, 14);
  if (ht & 0x80) {
    for (int hc = 0; hc < 8; ++hc) {
      if (pci_read_config_word(hc, 0) != 0xFFFF) {
        pci_enumerate_bus(hc, enumerator);
      }
    }
  }
  else pci_enumerate_bus(0, enumerator);
}

