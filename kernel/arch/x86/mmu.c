#include <kernel/arch/x86/mmu.h>
#include <kernel/panic.h>
#include <attrib.h>

extern void __end;

static UINTPTR NextFreePage;

static UINTPTR *const pPageDirectory = (void*)PAGE_ADDRESS_MASK;
static UINTPTR *const pPageTable = (void*)(~(UINTPTR)(PAGE_SIZE * PAGE_TABLE_SIZE - 1));

UINTPTR GetMemoryMap()
{
  UINTPTR MemoryMap;
  asm volatile ("mov %%cr3, %0" : "=a"(MemoryMap));
  return MemoryMap;
}

void SetMemoryMap(UINTPTR MemoryMap)
{
  asm volatile ("mov %0, %%cr3" : : "a"(MemoryMap) : "memory");
}

UINTPTR CreateMemoryMap(BOOL bShare)
{
  /* TODO: Implement Memory Map Creation */
  return bShare ? 0 : 0;
}

UINTPTR GetPageMapping(UINTPTR VirtualPage)
{
  UINTPTR PageIndex = VirtualPage >> PAGE_SHIFT;
  UINTPTR TableIndex = PageIndex / PAGE_TABLE_SIZE;
  if (!(pPageDirectory[TableIndex] & PAGE_PRESENT_FLAG)) return 0; /* There is no Page if there is no Page Table */
  UINTPTR Page = pPageTable[PageIndex];
  /* Convert x86 Page Table Entry to Mapping */
  UINTPTR Mapping = Page & PAGE_ADDRESS_MASK;
  if (Page & PAGE_PRESENT_FLAG) Mapping |= MAPPING_PRESENT_BIT | MAPPING_READABLE_BIT | MAPPING_EXECUTABLE_BIT; /* Every Page is Readable and Executable on x86 */
  if (Page & PAGE_WRITABLE_FLAG) Mapping |= MAPPING_WRITABLE_BIT;
  if (Page & PAGE_USER_FLAG) Mapping |= MAPPING_USER_MODE_BIT;
  if (Page & PAGE_GLOBAL_FLAG) Mapping |= MAPPING_GLOBAL_BIT;
  if (Page & PAGE_LOCAL_FLAG) Mapping |= MAPPING_LOCAL_BIT;
  return Mapping;
}

BOOL SetPageMapping(UINTPTR VirtualPage, UINTPTR Mapping)
{
  UINTPTR PageIndex = VirtualPage >> PAGE_SHIFT;
  UINTPTR TableIndex = PageIndex / PAGE_TABLE_SIZE;
  if (TableIndex == PAGE_TABLE_SIZE - 1) return FALSE; /* Refuse access to the Page Table Region */
  /* Convert Mapping to x86 Page Table Entry */
  UINTPTR Page = Mapping & PAGE_ADDRESS_MASK;
  if (Mapping & MAPPING_PRESENT_BIT) Page |= PAGE_PRESENT_FLAG;
  if (Mapping & MAPPING_WRITABLE_BIT) Page |= PAGE_WRITABLE_FLAG;
  if (Mapping & MAPPING_USER_MODE_BIT) Page |= PAGE_USER_FLAG;
  if (Mapping & MAPPING_GLOBAL_BIT) Page |= PAGE_GLOBAL_FLAG;
  if (Mapping & MAPPING_LOCAL_BIT) Page |= PAGE_LOCAL_FLAG;
  if (!(pPageDirectory[TableIndex] & PAGE_PRESENT_FLAG))
  {
    /* Allocate an empty Page Table */
    if (!NextFreePage) return FALSE; /* Out of Free Memory */
    pPageDirectory[TableIndex] = NextFreePage | PAGE_PRESENT_FLAG | PAGE_WRITABLE_FLAG | PAGE_USER_FLAG | (Mapping & MAPPING_LOCAL_BIT ? PAGE_LOCAL_FLAG : 0);
    NextFreePage = pPageTable[TableIndex * PAGE_TABLE_SIZE];
    for (UINTPTR i = 0; i < PAGE_TABLE_SIZE; ++i) pPageTable[TableIndex * PAGE_TABLE_SIZE + i] = 0;
  }
  pPageTable[PageIndex] = Page;
  /* Check if the Page Table is unused */
  for (UINTPTR i = 0; i < PAGE_TABLE_SIZE; ++i)
  {
    if (pPageTable[TableIndex * PAGE_TABLE_SIZE + i]) return TRUE;
  }
  /* Free the unused Page Table */
  pPageTable[TableIndex * PAGE_TABLE_SIZE] = NextFreePage;
  NextFreePage = pPageDirectory[TableIndex] & PAGE_ADDRESS_MASK;
  pPageDirectory[TableIndex] = 0;
  return TRUE;
}

BOOL MapFreePage(UINTPTR VirtualPage, UINT MappingFlags)
{
  UINTPTR PreviousMapping = GetPageMapping(VirtualPage);
  if (PreviousMapping || !SetPageMapping(VirtualPage, MAPPING_PRESENT_BIT)) return FALSE; /* The Page is already Mapped or is unable to be Mapped */
  if (!NextFreePage) /* Out of Free Memory */
  {
    (void)SetPageMapping(VirtualPage, PreviousMapping); /* Can NOT fail */
    return FALSE;
  }
  (void)SetPageMapping(VirtualPage, NextFreePage | (MappingFlags & PAGE_FLAGS_MASK) | MAPPING_PRESENT_BIT); /* Can NOT fail */
  NextFreePage = *(UINTPTR*)(void*)VirtualPage;
  return TRUE;
}

BOOL FreeMappedPage(UINTPTR VirtualPage)
{
  if (VirtualPage >= (UINTPTR)pPageTable) return FALSE; /* The Page Table can NOT be freed with this function */
  UINTPTR Mapping = GetPageMapping(VirtualPage);
  if (!(Mapping & MAPPING_PRESENT_BIT)) return FALSE; /* The Page is NOT a Memory Page */
  *(UINTPTR*)(void*)VirtualPage = NextFreePage;
  NextFreePage = Mapping & PAGE_ADDRESS_MASK;
  (void)SetPageMapping(VirtualPage, 0); /* Can NOT fail */
  return TRUE;
}

void InitMMU(UINTPTR FreePageList)
{
  /* Allocate a System Page Directory */
  UINTPTR *pSystemPageDirectory = (void*)FreePageList;
  if (!pSystemPageDirectory) KernelPanic("NO MEMORY");
  NextFreePage = *pSystemPageDirectory;
  for (UINTPTR i = 0; i < PAGE_TABLE_SIZE - 1; ++i) pSystemPageDirectory[i] = PAGE_USER_FLAG;
  pSystemPageDirectory[PAGE_TABLE_SIZE - 1] = (UINTPTR)pSystemPageDirectory | PAGE_PRESENT_FLAG | PAGE_WRITABLE_FLAG;
  /* Fill the System Page Directory and Tables */
  UINTPTR *pSystemPageTable;
  for (UINTPTR SystemPage = PAGE_PRESENT_FLAG | PAGE_WRITABLE_FLAG | PAGE_GLOBAL_FLAG; SystemPage < (UINTPTR)&__end; SystemPage += PAGE_SIZE)
  {
    UINTPTR PageIndex = SystemPage >> PAGE_SHIFT;
    UINTPTR PageTableIndex = PageIndex % PAGE_TABLE_SIZE;
    UINTPTR PageDirectoryIndex = PageIndex / PAGE_TABLE_SIZE;
    if (PageTableIndex == 0)
    {
      /* Allocate a System Page Table */
      pSystemPageTable = (void*)NextFreePage;
      if (!pSystemPageTable) KernelPanic("NO MEMORY");
      NextFreePage = *pSystemPageTable;
      pSystemPageDirectory[PageDirectoryIndex] = (UINTPTR)pSystemPageTable | PAGE_PRESENT_FLAG | PAGE_WRITABLE_FLAG | PAGE_USER_FLAG;
    }
    pSystemPageTable[PageTableIndex] = SystemPage;
  }
  /* Enable Paging */
  UINT32 CR0;
  asm volatile ("mov %0, %%cr3" : : "a"(pSystemPageDirectory) : "memory");
  asm volatile ("mov %%cr0, %0" : "=a"(CR0));
  asm volatile ("mov %0, %%cr0" : : "a"(CR0 | 0x80000001U));
}
