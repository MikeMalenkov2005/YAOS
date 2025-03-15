#ifndef KERNEL_ARCH_X86_DT_H
#define KERNEL_ARCH_X86_DT_H

#include <types.h>
#include <attrib.h>

typedef struct SEGMENT_DESCRIPTOR
{
  UINT16 Limit;
  UINT16 BaseLow;
  UINT8 BaseMid;
  UINT8 Access;
  UINT8 Flags;
  UINT8 BaseHigh;
} __packed SEGMENT_DESCRIPTOR;

typedef struct INTERRUPT_DESCRIPTOR
{
  UINT16 OffsetLow;
  UINT16 Selector;
  UINT8 Reserved;
  UINT8 Access;
  UINT16 OffsetHigh;
} __packed INTERRUPT_DESCRIPTOR;

typedef struct DESCRIPTOR_TABLE_POINTER
{
  UINT16 Limit;
  UINTPTR Offset;
} __packed DESCRIPTOR_TABLE_POINTER;

typedef struct TASK_STATE_SEGMENT
{
  UINT32 LinkSelector;
  UINT32 ESP0;
  UINT32 SS0;
  UINT32 ESP1;
  UINT32 SS1;
  UINT32 ESP2;
  UINT32 SS2;
  UINT32 CR3;
  UINT32 EIP;
  UINT32 EFLAGS;
  UINT32 EAX;
  UINT32 ECX;
  UINT32 EDX;
  UINT32 EBX;
  UINT32 ESP;
  UINT32 EBP;
  UINT32 ESI;
  UINT32 EDI;
  UINT32 ES;
  UINT32 CS;
  UINT32 SS;
  UINT32 DS;
  UINT32 FS;
  UINT32 GS;
  UINT32 LDTR;
  UINT16 DebugTrapFlag;
  UINT16 IOPBOffset;
  UINT32 SSP;
} TASK_STATE_SEGMENT;

#define GDT_ENTRY(nBase, nLimit, bAccess, bFlags)               \
(SEGMENT_DESCRIPTOR) {                                          \
  .BaseLow = (UINT16)(nBase),                                   \
  .BaseMid = (UINT8)((nBase) >> 16),                            \
  .BaseHigh = (UINT8)((nBase) >> 24),                           \
  .Limit = (UINT16)(nLimit),                                    \
  .Access = (UINT8)(bAccess),                                   \
  .Flags = (UINT8)(((bFlags) << 4) | (((nLimit) >> 16) & 0xF))  \
}

#define IDT_ENTRY(nOffset, wSelector, bAccess)  \
(INTERRUPT_DESCRIPTOR) {                        \
  .OffsetLow = (UINT16)(nOffset),               \
  .OffsetHigh = (UINT16)((nOffset) >> 16),      \
  .Selector = (UINT16)(wSelector),              \
  .Access = (UINT8)(bAccess)                    \
}

void InitGDT(UINT32 APMTable, UINT32 SystemStack);
void InitIDT();

#endif
