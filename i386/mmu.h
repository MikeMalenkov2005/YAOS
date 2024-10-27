#ifndef MMU_H
#define MMU_H

#include <stddef.h>
#include <stdint.h>

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PT_SHIFT    (PAGE_SHIFT - 2)
#define PT_LENGTH   (1 << PT_SHIFT)

void init_mmu(void* upper_bound);

size_t mmu_translate(void* vaddr);

#endif

