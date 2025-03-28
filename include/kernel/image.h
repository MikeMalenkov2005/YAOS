#ifndef KERNEL_IMAGE_H
#define KERNEL_IMAGE_H

#include <types.h>

BOOL LoadImage(UINTPTR *pEntryPoint, void *pImageData, SIZE_T ImageSize);

void SetArchInfo(UINT32 ELFMachinePair, UINT32 COFFMachinePair, BOOL bNetworkOrder);

#endif

