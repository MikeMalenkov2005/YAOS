#include <mmu.h>

#include <cpu.h>
#include <idt.h>

#include <panic.h>

#define PAGE_ROUND_UP(pa) (((size_t)(pa) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(pa) ((size_t)(pa) & ~(PAGE_SIZE - 1))

#define PDE_INDEX(va) ((size_t)(va) >> PAGE_SHIFT >> PT_SHIFT)
#define PTE_INDEX(va) ((size_t)(va) >> PAGE_SHIFT)

#define PAGE_FLAGS(pte) ((size_t)(pte) & (PAGE_SIZE - 1))
#define PAGE_COLOR(pte) (((size_t)(pte) >> 9) & 7)

extern char end;

static size_t* current_page_directory = (void*)((~(size_t)0) << (PAGE_SHIFT));
static size_t* current_page_tables = (void*)((~(size_t)0) << (PAGE_SHIFT + PT_SHIFT));

static size_t free_pages = 0;

void page_fault_handler(struct isr_frame* frame) {
  size_t vaddr = rdcr2();
  size_t pdei = PDE_INDEX(vaddr);
  size_t ptei = PTE_INDEX(vaddr);
  if (frame->error & 4) {
    /* terminate the task */
  }
  /* kernel mode fault */
  kpanic("paging");
}

void init_mmu(void* upper_bound) {
  set_cpu_isr(14, page_fault_handler);
  upper_bound = (void*)PAGE_ROUND_DOWN(upper_bound);
  void* lower_bound = (void*)PAGE_ROUND_UP(&end);
  if (upper_bound < lower_bound + (2 << PAGE_SHIFT)) upper_bound = lower_bound + (2 << PAGE_SHIFT); /* SHOULD NEVER HAPEN */
  for (void* p = lower_bound; p < upper_bound; p += PAGE_SIZE) {
    *(size_t*)p = free_pages;
    free_pages = (size_t)p;
  }
  size_t* system_page_directory = (void*)free_pages;
  size_t* system_page_table = (void*)*system_page_directory;
  free_pages = *system_page_table;
  for (size_t i = 0; i < PT_LENGTH; ++i) {
    system_page_directory[i] = 0;
    if (((void*)(i << PAGE_SHIFT)) < lower_bound) {
      system_page_table[i] = (i << PAGE_SHIFT) | PAGE_FLAG_V | PAGE_FLAG_R | PAGE_FLAG_W | PAGE_FLAG_X;
    }
  }
  system_page_directory[0] = ((size_t)system_page_table) | PAGE_FLAG_V | PAGE_FLAG_R | PAGE_FLAG_W | PAGE_FLAG_X;
  system_page_directory[PT_LENGTH - 1] = ((size_t)system_page_directory) | PAGE_FLAG_V | PAGE_FLAG_R | PAGE_FLAG_W | PAGE_FLAG_X;
  mmu_set_page_table((size_t)system_page_directory);
  wrcr0(rdcr0() | UINT32_C(0x80000000));
}

void mmu_set_page_table(size_t page_table) {
  wrcr3(page_table);
}

size_t mmu_get_page_table_entry(void* vaddr) {
  size_t pde = current_page_directory[PDE_INDEX(vaddr)];
  return pde ? current_page_tables[PTE_INDEX(vaddr)] : 0;
}

size_t mmu_translate_address(void* vaddr) {
  size_t pte = mmu_get_page_table_entry(vaddr);
  return pte ? PAGE_ROUND_DOWN(pte) & PAGE_FLAGS(vaddr) : 0;
}

bool mmu_check_block_flags(void* start, void* stop, int flags) {
  for (void* page = PAGE_ROUND_DOWN(start); page <= stop; page += PAGE_SIZE) {
    size_t pte = mmu_get_page_table_entry(page);
    if (pte & flags != flags) return false;
  }
  return true;
}
