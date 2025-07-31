#ifndef PCI_H
#define PCI_H

#include <sys/pci.h>
#include <sys/yaos.h>

typedef void (*PCI_DEVICE_CALLBACK)(PCI_DEVICE);

BOOL InitPCI();

void InitPCIDevice(PCI_DEVICE Device);

void EnumeratePCIDevices(PCI_DEVICE_CALLBACK pfnCallback);

UINT32 ReadPCIDeviceConfig32(PCI_DEVICE Device, UINT Offset);

UINT16 ReadPCIDeviceConfig16(PCI_DEVICE Device, UINT Offset);

UINT8 ReadPCIDeviceConfig8(PCI_DEVICE Device, UINT Offset);

#endif

