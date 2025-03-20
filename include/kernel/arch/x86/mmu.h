#ifndef KERNEL_ARCH_X86_MMU_H
#define KERNEL_ARCH_X86_MMU_H

#include <kernel/memory.h>

#define PAGE_TABLE_SIZE   (PAGE_SIZE / sizeof(UINTPTR))

#define PAGE_PRESENT_FLAG         (1 << 0)
#define PAGE_WRITABLE_FLAG        (1 << 1)
#define PAGE_USER_FLAG            (1 << 2)
#define PAGE_WRITE_THROUGH_FLAG   (1 << 3)
#define PAGE_CACHE_DISABLE_FLAG   (1 << 4)
#define PAGE_ACCESSED_FLAG        (1 << 5)
#define PAGE_DIRTY_FLAG           (1 << 6)
#define PAGE_ATTRIBUTE_TABLE_FLAG (1 << 7)
#define PAGE_GLOBAL_FLAG          (1 << 8)
#define PAGE_EXTERNAL_FLAG        (1 << 9)

void InitMMU(UINTPTR FreePageList);

#endif
