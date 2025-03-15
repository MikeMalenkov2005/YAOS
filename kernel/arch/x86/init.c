#include <kernel/arch/x86/init.h>
#include <kernel/arch/x86/mmu.h>
#include <kernel/arch/x86/fpu.h>
#include <kernel/arch/x86/dt.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <kernel/task.h>

extern void __end;

BOOL IsPageFree(UINTPTR Page, MODULE_INFO *Modules, UINT32 ModulesCount, UINTPTR Regions, UINT32 RegionsLength)
{
  if (Modules && ModulesCount)
  {
    for (UINT32 i = 0; i < ModulesCount; ++i)
    {
      if (Page >= PAGE_ROUND_DOWN(Modules[i].StartAddress) && Page < PAGE_ROUND_UP(Modules[i].EndAddress)) return FALSE;
    }
  }
  if (Regions && RegionsLength)
  {
    UINTPTR Offset = 0;
    while (Offset < RegionsLength)
    {
      UINT32 Size = *(UINT32*)(void*)(Regions + Offset - 4);
      MEMORY_REGION_INFO *Region = (void*)(Regions + Offset);
      if (Page >= PAGE_ROUND_DOWN(Region->BaseAddress) && Page < PAGE_ROUND_UP(Region->BaseAddress + Region->Length) && Region->Type != MEMORY_REGION_AVAILABLE_TYPE) return FALSE;
      Offset += Size + sizeof(UINT32);
    }
  }
  return TRUE;
}

void __naked InitEnd()
{
}

void InitArch(UINT32 BootMagic, BOOT_INFO *pBootInfo, UINT32 SystemStack)
{
  if (BootMagic != BOOT_MAGIC || !(pBootInfo->Flags & BOOT_INFO_MEMORY_INFO_FLAG)) return;
  InitGDT(pBootInfo->Flags & BOOT_INFO_APM_TABLE_FLAG ? pBootInfo->APMTable : 0, SystemStack);
  InitIDT();
  /* Get Loaded Moduler (if any) */
  UINT32 ModulesCount = pBootInfo->Flags & BOOT_INFO_MODULES_FLAG ? pBootInfo->ModulesCount : 0;
  MODULE_INFO *Modules = pBootInfo->Flags & BOOT_INFO_MODULES_FLAG ? (void*)pBootInfo->ModulesAddress : NULL;
  /* Create a Linked List of Free Pages */
  UINTPTR FreePageList = 0;
  UINTPTR MemorySize = PAGE_ROUND_DOWN(0x100000 + (pBootInfo->UpperMemory << 10));
  for (UINTPTR Page = PAGE_ROUND_DOWN(MemorySize); Page >= PAGE_ROUND_UP(&__end); Page -= PAGE_SIZE)
  {
    /* Get Memory Regions List (if present) */
    UINT32 RegionsLength = pBootInfo->Flags & BOOT_INFO_MEMORY_MAP_FLAG ? pBootInfo->MemoryRegionsLength : 0;
    UINTPTR Regions = pBootInfo->Flags & BOOT_INFO_MEMORY_MAP_FLAG ? pBootInfo->MemoryRegionsAddress : 0;
    /* Add the Page to The List (if available) */
    if (IsPageFree(Page, Modules, ModulesCount, Regions, RegionsLength))
    {
      UINTPTR* p = (void*)Page;
      *p = FreePageList;
      FreePageList = Page;
    }
  }
  InitMMU(FreePageList);
  InitFPU();
  InitTimer();
  InitTaskSlots(1024);
  KernelPanic("INIT END");
  /* TODO: Laod Modules and jump to User Mode */
  InitEnd();
}
