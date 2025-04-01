#include "kernel/memory.h"
#include <kernel/arch/x86/init.h>
#include <kernel/arch/x86/task.h>
#include <kernel/arch/x86/mmu.h>
#include <kernel/arch/x86/fpu.h>
#include <kernel/arch/x86/dt.h>
#include <kernel/image.h>
#include <kernel/timer.h>
#include <kernel/panic.h>
#include <sys/coff.h>
#include <sys/elf.h>

extern void __end;

BOOL IsPageFree(UINTPTR Page, MODULE_INFO *pModules, UINT32 ModulesCount, UINTPTR Regions, UINT32 RegionsLength)
{
  if (pModules && ModulesCount)
  {
    for (UINT32 i = 0; i < ModulesCount; ++i)
    {
      if (Page >= PAGE_ROUND_DOWN(pModules[i].StartAddress) && Page < PAGE_ROUND_UP(pModules[i].EndAddress)) return FALSE;
    }
  }
  if (Regions && RegionsLength)
  {
    UINTPTR Offset = 0;
    while (Offset < RegionsLength)
    {
      UINT32 Size = *(UINT32*)(void*)(Regions + Offset - 4);
      MEMORY_REGION_INFO *Region = (void*)(Regions + Offset);
      if (Page >= PAGE_ROUND_DOWN(Region->BaseAddress) &&
          Page < PAGE_ROUND_UP(Region->BaseAddress + Region->Length) &&
          Region->Type != MEMORY_REGION_AVAILABLE_TYPE)
        return FALSE;
      Offset += Size + sizeof(UINT32);
    }
  }
  return TRUE;
}

void InitArch(UINT32 BootMagic, BOOT_INFO *pBootInfo, UINT32 SystemStack, INTERRUPT_FRAME Frame)
{
  if (BootMagic != BOOT_MAGIC || !(pBootInfo->Flags & BOOT_INFO_MEMORY_INFO_FLAG)) KernelPanic("NO MULTIBOOT");
  InitGDT(pBootInfo->Flags & BOOT_INFO_APM_TABLE_FLAG ? pBootInfo->APMTable : 0, SystemStack);
  InitIDT();
  /* Get Loaded Modules (panic of none) */
  UINT32 ModulesCount = pBootInfo->Flags & BOOT_INFO_MODULES_FLAG ? pBootInfo->ModulesCount : 0;
  MODULE_INFO *pModules = pBootInfo->Flags & BOOT_INFO_MODULES_FLAG ? (void*)(UINTPTR)pBootInfo->ModulesAddress : NULL;
  if (!ModulesCount) KernelPanic("NO MODULES");
  /* Create a Linked List of Free Pages */
  UINTPTR FreePageList = 0;
  UINTPTR MemorySize = PAGE_ROUND_UP(0x100000 + (pBootInfo->UpperMemory << 10));
  for (UINTPTR Page = MemorySize - PAGE_SIZE; Page >= PAGE_ROUND_UP(&__end); Page -= PAGE_SIZE)
  {
    /* Get Memory Regions List (if present) */
    UINT32 RegionsLength = pBootInfo->Flags & BOOT_INFO_MEMORY_MAP_FLAG ? pBootInfo->MemoryRegionsLength : 0;
    UINTPTR Regions = pBootInfo->Flags & BOOT_INFO_MEMORY_MAP_FLAG ? pBootInfo->MemoryRegionsAddress : 0;
    /* Add the Page to The List (if available) */
    if (IsPageFree(Page, pModules, ModulesCount, Regions, RegionsLength))
    {
      UINTPTR* p = (void*)Page;
      *p = FreePageList;
      FreePageList = Page;
    }
  }
  InitMMU(FreePageList);
  InitFPU();
  InitTimer();
  InitTasks();
  SetTaskFrame(&Frame);
  SetArchInfo(EM_386, COFF_MACHINE_I386, FALSE);
  /* Create a Task for Each Module */
  for (UINT32 i = 0; i < ModulesCount; ++i)
  {
    const TASK *pTask = CreateTask(TASK_STACK_SIZE, TASK_MODULE_BIT);
    if (!pTask) KernelPanic("CREATE TASK");
    
    SIZE_T ModuleSize = pModules[i].EndAddress - pModules[i].StartAddress;
    SIZE_T ModulePageCount = PAGE_ROUND_UP(ModuleSize + (pModules[i].StartAddress & PAGE_FLAGS_MASK)) >> PAGE_SHIFT;
    UINTPTR FirstModulePage = FindLastFreeVirtualPages(ModulePageCount);
    for (SIZE_T j = 0; j < ModulePageCount; ++j) SetPageMapping(FirstModulePage + j * PAGE_SIZE,
        (pModules[i].StartAddress & PAGE_ADDRESS_MASK) | MAPPING_PRESENT_BIT | MAPPING_READABLE_BIT);
    void *pModuleData = (void*)(FirstModulePage | (pModules[i].StartAddress & PAGE_FLAGS_MASK));
    UINTPTR EntryPoint = 0;

    if (!LoadImage(&EntryPoint, pModuleData, ModuleSize)) KernelPanic("LOAD IMAGE");
    if (!EntryPoint) KernelPanic("NO ENTRY POINT");
    SetCurrentUserIP(EntryPoint);
    if (!FreeMappedPages(FirstModulePage, ModulePageCount)) KernelPanic("FREE MAPPED PAGES");
    SwitchTask(TRUE); /* Switch to the Previous Task */
  }
}
