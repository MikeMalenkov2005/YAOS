#include "pci.h"

#define HEX(n)  (((n) & 15) < 10 ? ((n) & 15) + '0' : ((n) & 15) + 'A' - 10)

static UINT BaseTaskID;

BOOL InitPCI()
{
  BaseTaskID = GetTaskID();
  EnumeratePCIDevices(InitPCIDevice);
  return TRUE;
}

void PrintPCIDevice(PCI_DEVICE_INFO *pInfo);

void InitPCIDevice(PCI_DEVICE Device)
{
  MESSAGE Message = { 0 };
  PCI_DEVICE_INFO *pInfo = (void*)Message.Payload;
  pInfo->Device = Device;
  pInfo->VendorID = ReadPCIDeviceConfig16(Device, PCI_VENDOR_ID_OFFSET);
  pInfo->DeviceID = ReadPCIDeviceConfig16(Device, PCI_DEVICE_ID_OFFSET);
  pInfo->Class = ReadPCIDeviceConfig8(Device, PCI_CLASS_OFFSET);
  pInfo->Subclass = ReadPCIDeviceConfig8(Device, PCI_SUBCLASS_OFFSET);
  pInfo->InterfaceID = ReadPCIDeviceConfig8(Device, PCI_INTERFACE_ID_OFFSET);
  pInfo->RevisionID = ReadPCIDeviceConfig8(Device, PCI_REVISION_ID_OFFSET);
  Message.ReceiverID = BaseTaskID + pInfo->Class;
  SendMessage(&Message);
  PrintPCIDevice(pInfo);
}

void EnumeratePCIBus(UINT8 nBus, PCI_DEVICE_CALLBACK pfnCallback);

void EnumeratePCIDevices(PCI_DEVICE_CALLBACK pfnCallback)
{
  UINT8 HeaderType = ReadPCIDeviceConfig8(0, PCI_HEADER_TYPE_OFFSET);
  if (HeaderType & 0x80)
  {
    for (UINT8 i = 0; i < 8 && ~ReadPCIDeviceConfig16((PCI_DEVICE)i, PCI_VENDOR_ID_OFFSET); ++i)
    {
      EnumeratePCIBus(i, pfnCallback);
    }
  }
  else EnumeratePCIBus(0, pfnCallback);
}

UINT16 ReadPCIDeviceConfig16(PCI_DEVICE Device, UINT Offset)
{
  UINT32 Result = ReadPCIDeviceConfig32(Device, Offset);
  return Result >> ((Offset & 2) << 3);
}

UINT8 ReadPCIDeviceConfig8(PCI_DEVICE Device, UINT Offset)
{
  UINT32 Result = ReadPCIDeviceConfig32(Device, Offset);
  return Result >> ((Offset & 3) << 3);
}

void EnumeratePCIFunction(PCI_DEVICE Device, PCI_DEVICE_CALLBACK pfnCallback)
{
  UINT16 DeviceType = ReadPCIDeviceConfig16(Device, PCI_DEVICE_TYPE_OFFSET);
  pfnCallback(Device);
  if (DeviceType == 0x604) EnumeratePCIBus(ReadPCIDeviceConfig8(Device, PCI1_SECONDARY_BUS_OFFSET), pfnCallback);
}

void EnumeratePCIDevice(UINT8 nBus, UINT8 nDevice, PCI_DEVICE_CALLBACK pfnCallback)
{
  PCI_DEVICE Device = FORM_PCI_DEVICE(nBus, nDevice, 0);
  UINT16 VendorID = ReadPCIDeviceConfig16(Device, PCI_VENDOR_ID_OFFSET);
  if (VendorID != 0xFFFF)
  {
    UINT8 HeaderType = ReadPCIDeviceConfig8(Device, PCI_HEADER_TYPE_OFFSET);
    if (HeaderType & 0x80)
    {
      for (UINT8 i = 0; i < 8; ++i)
      {
        VendorID = ReadPCIDeviceConfig16(Device | i, PCI_VENDOR_ID_OFFSET);
        if (VendorID != 0xFFFF) EnumeratePCIFunction(Device | i, pfnCallback);
      }
    }
    else EnumeratePCIFunction(Device, pfnCallback);
  }
}

void EnumeratePCIBus(UINT8 nBus, PCI_DEVICE_CALLBACK pfnCallback)
{
  for (UINT8 nDevice = 0; nDevice < 32; ++nDevice)
  {
    EnumeratePCIDevice(nBus, nDevice, pfnCallback);
  }
}

void PrintPCIDevice(PCI_DEVICE_INFO *pInfo)
{
  MESSAGE Message = { .ReceiverID = 1, .Payload = "\x01\x1EXXXX-XXXX: XXXX XXXX XX-XX-XX" };
  Message.Payload[2] = HEX(pInfo->Device >> 28);
  Message.Payload[3] = HEX(pInfo->Device >> 24);
  Message.Payload[4] = HEX(pInfo->Device >> 20);
  Message.Payload[5] = HEX(pInfo->Device >> 16);
  Message.Payload[7] = HEX(pInfo->Device >> 12);
  Message.Payload[8] = HEX(pInfo->Device >> 8);
  Message.Payload[9] = HEX(pInfo->Device >> 4);
  Message.Payload[10] = HEX(pInfo->Device);
  Message.Payload[13] = HEX(pInfo->VendorID >> 12);
  Message.Payload[14] = HEX(pInfo->VendorID >> 8);
  Message.Payload[15] = HEX(pInfo->VendorID >> 4);
  Message.Payload[16] = HEX(pInfo->VendorID);
  Message.Payload[18] = HEX(pInfo->DeviceID >> 12);
  Message.Payload[19] = HEX(pInfo->DeviceID >> 8);
  Message.Payload[20] = HEX(pInfo->DeviceID >> 4);
  Message.Payload[21] = HEX(pInfo->DeviceID);
  Message.Payload[23] = HEX(pInfo->Class >> 4);
  Message.Payload[24] = HEX(pInfo->Class);
  Message.Payload[26] = HEX(pInfo->Subclass >> 4);
  Message.Payload[27] = HEX(pInfo->Subclass);
  Message.Payload[29] = HEX(pInfo->InterfaceID >> 4);
  Message.Payload[30] = HEX(pInfo->InterfaceID);
  SendMessage(&Message);
  for (UINT i = 2; i < MESSAGE_SIZE; ++i) Message.Payload[i] = 0;
  Message.Payload[1] = 10;
  SendMessage(&Message);
}

