#ifndef SYS_PCI_H
#define SYS_PCI_H

#include <types.h>

typedef UINT PCI_DEVICE;

typedef struct PCI_DEVICE_INFO
{
  PCI_DEVICE Device;
  UINT16 VendorID;
  UINT16 DeviceID;
  UINT8 Class;
  UINT8 Subclass;
  UINT8 InterfaceID;
  UINT8 RevisionID;
} PCI_DEVICE_INFO;

#endif

