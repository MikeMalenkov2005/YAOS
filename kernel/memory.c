#include <kernel/memory.h>
#include <kernel/panic.h>

extern void __end;

UINTPTR FindFirstFreeVirtualPages(SIZE_T PageCount)
{
  BOOL bFound = FALSE;
  UINTPTR FirstPage = PAGE_ROUND_UP((UINTPTR)&__end);
  while (!bFound && FirstPage)
  {
    bFound = TRUE;
    for (SIZE_T i = 0; i < PageCount && bFound; ++i)
    {
      if (GetPageMapping(FirstPage + i * PAGE_SIZE))
      {
        bFound = FALSE;
        FirstPage += (i + 1) * PAGE_SIZE;
      }
    }
  }
  return FirstPage;
}

void *MapFirstFreePages(SIZE_T PageCount, int MappingFlags)
{
  UINTPTR FirstPage = FindFirstFreeVirtualPages(PageCount);
  if (FirstPage)
  {
    for (SIZE_T i = 0; i < PageCount; ++i)
    {
      if (!MapFreePage(FirstPage + i * PAGE_SIZE, MappingFlags))
      {
        for (SIZE_T j = 0; j < i; ++j) (void)FreeMappedPage(FirstPage + j * PAGE_SIZE);
        return FALSE;
      }
    }
  }
  return (void*)FirstPage;
}
