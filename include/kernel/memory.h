#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <types.h>

#define PAGE_SHIFT        12
#define PAGE_SIZE         (1 << PAGE_SHIFT)
#define PAGE_FLAGS_MASK   (PAGE_SIZE - 1)
#define PAGE_ADDRESS_MASK (~(UINTPTR)PAGE_FLAGS_MASK)

#define PAGE_ROUND_DOWN(Address)  ((UINTPTR)(Address) & PAGE_ADDRESS_MASK)
#define PAGE_ROUND_UP(Address)    (((UINTPTR)(Address) + PAGE_FLAGS_MASK) & PAGE_ADDRESS_MASK)

UINTPTR GetMemoryMap();

void SetMemoryMap(UINTPTR MemoryMap);

UINTPTR CreateMemoryMap();

BOOL DeleteMemoryMap(UINTPTR MemoryMap);

void InvalidatePage(UINTPTR VirtualPage);

#define MAPPING_PRESENT_BIT       (1 << 0)
#define MAPPING_READABLE_BIT      (1 << 1)
#define MAPPING_WRITABLE_BIT      (1 << 2)
#define MAPPING_EXECUTABLE_BIT    (1 << 3)
#define MAPPING_USER_MODE_BIT     (1 << 4)
#define MAPPING_GLOBAL_BIT        (1 << 5)
#define MAPPING_EXTERNAL_BIT      (1 << 6)
#define MAPPING_CACHE_DISABLE_BIT (1 << 7)

#define MAPPING_COMMITED_MASK     (MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT | MAPPING_EXECUTABLE_BIT)

UINTPTR GetPageMapping(UINTPTR VirtualPage);

BOOL SetPageMapping(UINTPTR VirtualPage, UINTPTR Mapping);

BOOL MapFreePage(UINTPTR VirtualPage, UINT MappingFlags);

BOOL FreeMappedPage(UINTPTR VirtualPage);

BOOL RemapPage(UINTPTR VirtualPage, UINT MappingFlags);

BOOL IsUserPage(UINTPTR VirtualPage);

UINTPTR FindFirstFreeVirtualPages(SIZE_T PageCount);

UINTPTR FindLastFreeVirtualPages(SIZE_T PageCount);

UINTPTR FindBestFreeVirtualPages(SIZE_T PageCount);

void *MapFreePages(UINTPTR FirstPage, SIZE_T PageCount, UINT MappingFlags);

void *MapFirstFreePages(SIZE_T PageCount, UINT MappingFlags);

void *MapLastFreePages(SIZE_T PageCount, UINT MappingFlags);

void *MapBestFreePages(SIZE_T PageCount, UINT MappingFlags);

SIZE_T FreeMappedPages(UINTPTR FirstPage, SIZE_T PageCount);

BOOL RemapPages(UINTPTR FirstPage, SIZE_T PageCount, UINT MappingFlags);

BOOL CheckUserAccess(const void *pBlock, SIZE_T Size);

/* Forces MAPPING_PRESENT_BIT to 1, returnes 0 on failure */
UINT GetRangeMappingFlags(UINTPTR FirstPage, SIZE_T PageCount);

#endif
