#include "kernel/memory.h"
#include "types.h"
#include <kernel/arch/x86/mmu.h>
#include <kernel/panic.h>
#include <kernel/task.h>
#include <attrib.h>

extern void __end;

static UINTPTR NextFreePage;

static UINTPTR *const pPageDirectory = (void*)PAGE_ADDRESS_MASK;
static UINTPTR *const pPageTable = (void*)(~(UINTPTR)(PAGE_SIZE * PAGE_TABLE_SIZE - 1));

/* INVLPG is supported only after i486 */
#define InvalidatePage(VirtualPage) SetMemoryMap(GetMemoryMap());

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
  KernelPanic("NO IMPL");
  return bShare ? 0 : 0;
}

BOOL DeleteMemoryMap(UINTPTR MemoryMap, BOOL bFreeShared)
{
  UINTPTR CurrentMap = GetMemoryMap();
  if (MemoryMap == CurrentMap) return FALSE;
  UINTPTR VirtualPages = FindLastFreeVirtualPages(2);
  if (!VirtualPages) return FALSE;
  (void)SetPageMapping(VirtualPages + PAGE_SIZE, MemoryMap | MAPPING_READABLE_BIT | MAPPING_WRITABLE_BIT | MAPPING_PRESENT_BIT);
  UINTPTR *pDirectoryToDelete = (void*)(VirtualPages + PAGE_SIZE);
  if ((pDirectoryToDelete[PAGE_TABLE_SIZE - 1] & PAGE_ADDRESS_MASK) != MemoryMap)
  {
    SetPageMapping(VirtualPages + PAGE_SIZE, 0);
    return FALSE;
  }
  for (UINTPTR TableIndex = 0; TableIndex < PAGE_TABLE_SIZE - 1; ++TableIndex)
  {
    (void)SetPageMapping(VirtualPages, pDirectoryToDelete[TableIndex]);
    UINTPTR *pTableToDelete = (void*)VirtualPages;
    for (UINTPTR PageIndex = 0; PageIndex < PAGE_TABLE_SIZE; ++PageIndex)
    {
      UINTPTR Page = pTableToDelete[PageIndex];
      if (!(Page & (PAGE_GLOBAL_FLAG | PAGE_EXTERNAL_FLAG)) && ((Page & (PAGE_LOCAL_FLAG)) || bFreeShared))
      {
        if (Page & PAGE_PRESENT_FLAG)
        {
          (void)SetPageMapping(VirtualPages, (Page & PAGE_ADDRESS_MASK) | MAPPING_WRITABLE_BIT | MAPPING_PRESENT_BIT);
          *(UINTPTR*)(void*)VirtualPages = NextFreePage;
          NextFreePage = Page & PAGE_ADDRESS_MASK;
          (void)SetPageMapping(VirtualPages, pDirectoryToDelete[TableIndex]);
        }
      }
    }
    *pTableToDelete = NextFreePage;
    NextFreePage = pDirectoryToDelete[TableIndex];
  }
  *pDirectoryToDelete = NextFreePage;
  NextFreePage = MemoryMap;
  (void)SetPageMapping(VirtualPages + PAGE_SIZE, 0);
  (void)SetPageMapping(VirtualPages, 0);
  return TRUE;
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
  if (Page & PAGE_GRANT_FLAG) Mapping |= MAPPING_GRANT_BIT;
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
  if (Mapping & MAPPING_GRANT_BIT) Page |= PAGE_GRANT_FLAG;
  if (!(pPageDirectory[TableIndex] & PAGE_PRESENT_FLAG))
  {
    if (!Page) return TRUE;
    /* Allocate an empty Page Table */
    if (!NextFreePage) return FALSE; /* Out of Free Memory */
    pPageDirectory[TableIndex] = NextFreePage | PAGE_PRESENT_FLAG | PAGE_WRITABLE_FLAG | PAGE_USER_FLAG;
    InvalidatePage((UINTPTR)(void*)&pPageTable[TableIndex * PAGE_TABLE_SIZE]);
    NextFreePage = pPageTable[TableIndex * PAGE_TABLE_SIZE];
    for (UINTPTR i = 0; i < PAGE_TABLE_SIZE; ++i) pPageTable[TableIndex * PAGE_TABLE_SIZE + i] = 0;
  }
  pPageTable[PageIndex] = Page;
  InvalidatePage(VirtualPage);
  /* Check if the Page Table is unused */
  for (UINTPTR i = 0; i < PAGE_TABLE_SIZE; ++i)
  {
    if (pPageTable[TableIndex * PAGE_TABLE_SIZE + i]) return TRUE;
  }
  /* Free the unused Page Table */
  pPageTable[TableIndex * PAGE_TABLE_SIZE] = NextFreePage;
  NextFreePage = pPageDirectory[TableIndex] & PAGE_ADDRESS_MASK;
  pPageDirectory[TableIndex] = 0;
  InvalidatePage((UINTPTR)(void*)&pPageTable[TableIndex * PAGE_TABLE_SIZE]);
  return TRUE;
}

BOOL MapFreePage(UINTPTR VirtualPage, UINT MappingFlags)
{
  UINTPTR Mapping = NextFreePage | (MappingFlags & PAGE_FLAGS_MASK) | MAPPING_PRESENT_BIT;
  if (!NextFreePage /* Out of Free Pages */
      || GetPageMapping(VirtualPage) /* The Page is already Mapped */
      || !SetPageMapping(VirtualPage, Mapping)) return FALSE; /* The Page cannot be Mapped */
  NextFreePage = *(UINTPTR*)(void*)VirtualPage;
  return TRUE;
}

BOOL FreeMappedPage(UINTPTR VirtualPage)
{
  if (VirtualPage >= (UINTPTR)pPageTable) return FALSE; /* The Page Table can NOT be freed with this function */
  UINTPTR Mapping = GetPageMapping(VirtualPage);
  if (!(Mapping & MAPPING_PRESENT_BIT)) return FALSE;
  *(UINTPTR*)(void*)VirtualPage = NextFreePage;
  NextFreePage = Mapping & PAGE_ADDRESS_MASK;
  return SetPageMapping(VirtualPage, 0);
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
