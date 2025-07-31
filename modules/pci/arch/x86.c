#include "../pci.h"

#include <kernel/arch/x86/pio.h>

UINT32 ReadPCIDeviceConfig32(PCI_DEVICE Device, UINT Offset)
{
  UINT32 Address = ((UINT32)1 << 31) | ((UINT32)Device << 8) | (Offset & 0xFC);
  WritePort32(0xCF8, Address);
  return ReadPort32(0xCFC);
}

