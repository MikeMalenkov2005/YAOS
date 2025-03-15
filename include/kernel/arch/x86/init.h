#ifndef KERNEL_ARCH_X86_INIT_H
#define KERNEL_ARCH_X86_INIT_H

#include <types.h>
#include <attrib.h>

#define BOOT_MAGIC  0x2BADB002

typedef struct COLOR_PALETTE_INFO
{
  UINT32 Address;
  UINT16 Length;
} __packed COLOR_PALETTE_INFO;

typedef struct COLOR_PACKED_INFO
{
  UINT8 RedShift;
  UINT8 RedBits;
  UINT8 GreenShift;
  UINT8 GreenBits;
  UINT8 BlueShift;
  UINT8 BlueBits;
} __packed COLOR_PACKED_INFO;

typedef union COLOR_INFO
{
  COLOR_PALETTE_INFO Palette;
  COLOR_PACKED_INFO Packed;
} __packed COLOR_INFO;

#define BOOT_INFO_MEMORY_INFO_FLAG  (1 << 0)
#define BOOT_INFO_BOOT_DEVICE_FLAG  (1 << 1)
#define BOOT_INFO_COMMAND_LINE_FLAG (1 << 2)
#define BOOT_INFO_MODULES_FLAG      (1 << 3)
#define BOOT_INFO_A_OUT_SYMS_FLAG   (1 << 4)
#define BOOT_INFO_ELF_SYMS_FLAG     (1 << 5)
#define BOOT_INFO_MEMORY_MAP_FLAG   (1 << 6)
#define BOOT_INFO_DRIVES_INFO_FLAG  (1 << 7)
#define BOOT_INFO_CONFIG_TABLE_FLAG (1 << 8)
#define BOOT_INFO_LOADER_NAME_FLAG  (1 << 9)
#define BOOT_INFO_APM_TABLE_FLAG    (1 << 10)
#define BOOT_INFO_VBE_INFO_FLAG     (1 << 11)
#define BOOT_INFO_FRAMEBUFFER_FLAG  (1 << 12)

typedef struct BOOT_INFO
{
  UINT32 Flags;
  UINT32 LowerMemory;
  UINT32 UpperMemory;
  UINT32 BootDevice;
  UINT32 CmdLine;
  UINT32 ModulesCount;
  UINT32 ModulesAddress;
  UINT32 SymbolTableInfo[4];
  UINT32 MemoryRegionsLength;
  UINT32 MemoryRegionsAddress; /* MUST SUBTRACT 4 TO GET ADDRESS */
  UINT32 DevicesLength;
  UINT32 DevicesAddress;
  UINT32 ConfigTable;
  UINT32 BootLoaderName;
  UINT32 APMTable;
  UINT32 VBEControlInfo;
  UINT32 VBEModeInfo;
  UINT16 VBEMode;
  UINT16 VBEInterfaceSegment;
  UINT16 VBEInterfaceOffset;
  UINT16 VBEInterfaceLength;
  UINT64 FramebufferAddress;
  UINT32 FramebufferPitch;
  UINT32 FramebufferWidth;
  UINT32 FramebufferHeight;
  UINT8 FramebufferBPP;
  UINT8 FramebufferType;
  COLOR_INFO ColorInfo;
} __packed BOOT_INFO;

typedef struct MODULE_INFO
{
  UINT32 StartAddress;
  UINT32 EndAddress;
  UINT32 String;
  UINT32 Reserved;
} MODULE_INFO;

#define MEMORY_REGION_AVAILABLE_TYPE  1
#define MEMORY_REGION_ACPI_INFO_TYPE  3
#define MEMORY_REGION_PRESERVED_TYPE  4
#define MEMORY_REGION_DEFECTIVE_TYPE  5

typedef struct MEMORY_REGION_INFO
{
  UINT32 BaseAddress;
  UINT32 Length;
  UINT32 Type;
} MEMORY_REGION_INFO;

typedef struct DRIVE_INFO
{
  UINT32 StructureSize;
  UINT8 DriveNumber;
  UINT8 bIsLinear;
  UINT16 CylinderCount;
  UINT8 HeadCount;
  UINT8 SectorsPerTrack;
  UINT16 DrivePorts[0];
} __packed DRIVE_INFO;

#endif
