#ifndef SYS_PCI_H
#define SYS_PCI_H

#include <types.h>

typedef UINT PCI_DEVICE;

#define FORM_PCI_DEVICE(nBus, nDevice, nFunction) (((PCI_DEVICE)(nBus & 255) << 8) | ((PCI_DEVICE)(nDevice & 31) << 3) | (PCI_DEVICE)(nFunction & 7))

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

#define PCI_COMMAND_READ_CONFIG 1

typedef struct PCI_COMMAND
{
  PCI_DEVICE Device;
  UINT Command;
  UINT Args[0];
} PCI_COMMAND;

#define PCI_VENDOR_ID_OFFSET                0
#define PCI_DEVICE_ID_OFFSET                2
#define PCI_COMMAND_OFFSET                  4
#define PCI_STATUS_OFFSET                   6

#define PCI_INTERFACE_OFFSET                8
#define PCI_REVISION_ID_OFFSET              8
#define PCI_INTERFACE_ID_OFFSET             9

#define PCI_DEVICE_TYPE_OFFSET              10
#define PCI_SUBCLASS_OFFSET                 10
#define PCI_CLASS_OFFSET                    11

#define PCI_CACHE_LINE_SIZE_OFFSET          12
#define PCI_LATENCY_TIMER_OFFSET            13
#define PCI_HEADER_TYPE_OFFSET              14
#define PCI_BIST_OFFSET                     15

#define PCI0_BAR0_OFFSET                    16
#define PCI0_BAR1_OFFSET                    20
#define PCI0_BAR2_OFFSET                    24
#define PCI0_BAR3_OFFSET                    28
#define PCI0_BAR4_OFFSET                    32
#define PCI0_BAR5_OFFSET                    36
#define PCI0_CARDBUS_CIS_PTR_OFFSET         40
#define PCI0_SUBSYSTEM_VENDOR_ID_OFFSET     44
#define PCI0_SUBSYSTEM_ID_OFFSET            46
#define PCI0_EXPANSION_ROM_BASE_OFFSET      48
#define PCI0_CAPABILITIES_PTR_OFFSET        52
#define PCI0_INTERRUPT_LINE_OFFSET          60
#define PCI0_INTERRUPT_PIN_OFFSET           61
#define PCI0_MIN_GRANT_OFFSET               62
#define PCI0_MAX_LATENCY_OFFSET             63

#define PCI1_BAR0_OFFSET                    16
#define PCI1_BAR1_OFFSET                    20
#define PCI1_PRIMARY_BUS_OFFSET             24
#define PCI1_SECONDARY_BUS_OFFSET           25
#define PCI1_SUBORDINATE_BUS_OFFSET         26
#define PCI1_SECONDATY_LATENCY_TIMER_OFFSET 27
#define PCI1_IO_BASE_OFFSET                 28
#define PCI1_IO_LIMIT_OFFSET                29
#define PCI1_SECONDARY_STATUS_OFFSET        30
#define PCI1_MEMORY_BASE_OFFSET             32
#define PCI1_MEMORY_LIMIT_OFFSET            34
#define PCI1_PREFETCH_MEMORY_BASE_OFFSET    36
#define PCI1_PREFETCH_MEMORY_LIMIT_OFFSET   38
#define PCI1_PREFETCH_BASE_UPPER_32_OFFSET  40
#define PCI1_PREFETCH_LIMIT_UPPER_32_OFFSET 44
#define PCI1_IO_BASE_UPPER_16_OFFSET        48
#define PCI1_IO_LIMIT_UPPER_16_OFFSET       50
#define PCI1_CAPABILITIES_PTR_OFFSET        52
#define PCI1_EXPANSION_ROM_BASE_OFFSET      56
#define PCI1_INTERRUPT_LINE_OFFSET          60
#define PCI1_INTERRUPT_PIN_OFFSET           61
#define PCI1_BRIDGE_CONTROL_OFFSET          62

#endif

