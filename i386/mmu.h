#ifndef MMU_H
#define MMU_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PT_SHIFT    (PAGE_SHIFT - 2)
#define PT_LENGTH   (1 << PT_SHIFT)

#define PAGE_FLAG_V 1
#define PAGE_FLAG_R 0
#define PAGE_FLAG_W 2
#define PAGE_FLAG_X 0
#define PAGE_FLAG_U 4

void init_mmu(void* upper_bound);

void mmu_set_page_table(size_t page_table);

size_t mmu_get_page_table_entry(void* vaddr);
size_t mmu_translate_address(void* vaddr);

bool mmu_check_block_flags(void* start, void* stop, int flags);

#endif

