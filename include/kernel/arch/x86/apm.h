#ifndef KERNEL_ARCH_X86_APM_H
#define KERNEL_ARCH_X86_APM_H

#include <types.h>
#include <attrib.h>

typedef struct APM_TABLE
{
  UINT16 Version;
  UINT16 CodeSegment;
  UINT32 EntryOffset;
  UINT16 CodeSegment16;
  UINT16 DataSegment;
  UINT16 Flags;
  UINT16 CodeSegmentLength;
  UINT16 CodeSegment16Length;
  UINT16 DataSegmentLength;
} __packed APM_TABLE;

#endif
