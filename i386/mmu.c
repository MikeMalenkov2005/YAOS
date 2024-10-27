#include <mmu.h>

#include <cpu.h>
#include <idt.h>

#include <panic.h>

#define PAGE_ROUND_UP(pa) (((pa) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_ROUND_DOWN(pa) ((pa) & ~(PAGE_SIZE - 1))

#define PDE_INDEX(va) ((va) >> PAGE_SHIFT >> PT_SHIFT)
#define PTE_INDEX(va) ((va) >> PAGE_SHIFT)

#define PAGE_FLAGS(pte) ((pte) & (PAGE_SIZE - 1))
#define PAGE_COLOR(pte) (((pte) >> 9) & 7)

extern char end;

size_t* current_page_directory = (void*)((~(size_t)0) << (PAGE_SHIFT));
size_t* current_page_tables = (void*)((~(size_t)0) << (PAGE_SHIFT + PT_SHIFT));

size_t free_pages = 0;

void* mmu_allocate(void* vaddr, int flags) {
  vaddr = (void*)PAGE_ROUND_DOWN((size_t)vaddr);
  size_t ptei = PTE_INDEX((size_t)vaddr);
  current_page_tables[ptei] = free_pages | flags | 1; /* force present flag */
  free_pages = *(size_t*)vaddr;
  return vaddr;
}

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
  upper_bound = (void*)PAGE_ROUND_DOWN((size_t)upper_bound);
  void* lower_bound = (void*)PAGE_ROUND_UP((size_t)&end);
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
      system_page_table[i] = (i << 12) | 3;
    }
  }
  system_page_directory[0] = ((size_t)system_page_table) | 3;
  system_page_directory[PT_LENGTH - 1] = ((size_t)system_page_directory) | 3;
  wrcr3((size_t)system_page_directory);
  wrcr0(rdcr0() | UINT32_C(0x80000000));
}

size_t mmu_translate(void* vaddr) {
  size_t pde = current_page_directory[PDE_INDEX((size_t)vaddr)];
  if (!pde) return 0;
  size_t pte = current_page_tables[PTE_INDEX((size_t)vaddr)];
  if (!pte) return 0;
  return PAGE_ROUND_DOWN(pte) & PAGE_FLAGS((size_t)vaddr);
}

