#include <kernel/memory.h>
#include <kernel/panic.h>

extern void __end;

UINTPTR FindFirstFreeVirtualPages(SIZE_T PageCount)
{
  if (!PageCount) return 0;
  BOOL bFound = FALSE;
  UINTPTR FirstPage = PAGE_ROUND_UP((UINTPTR)&__end); /* PAGE_SIZE ??? */
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

UINTPTR FindLastFreeVirtualPages(SIZE_T PageCount)
{
  if (!PageCount) return 0;
  BOOL bFound = FALSE;
  UINTPTR FirstPage = PAGE_ROUND_DOWN(~(UINTPTR)0) - (PageCount - 1) * PAGE_SIZE;
  while (!bFound && FirstPage)
  {
    bFound = TRUE;
    for (SIZE_T i = 0; i < PageCount && bFound; ++i)
    {
      if (GetPageMapping(FirstPage + i * PAGE_SIZE))
      {
        bFound = FALSE;
        FirstPage -= (i + 1) * PAGE_SIZE;
      }
    }
  }
  return FirstPage;
}

UINTPTR FindBestFreeVirtualPages(SIZE_T PageCount)
{
  /* TODO: Implement best fit search */
  return PageCount ? 0 : 0;
}

void *MapFirstFreePages(SIZE_T PageCount, UINT MappingFlags)
{
  UINTPTR FirstPage = FindFirstFreeVirtualPages(PageCount);
  if (!FirstPage) return NULL;
  for (SIZE_T i = 0; i < PageCount; ++i)
  {
    if (!MapFreePage(FirstPage + i * PAGE_SIZE, MappingFlags))
    {
      for (SIZE_T j = 0; j < i; ++j)
      {
        (void)FreeMappedPage(FirstPage + j * PAGE_SIZE);
      }
      return NULL;
    }
  }
  return (void*)FirstPage;
}

void *MapLastFreePages(SIZE_T PageCount, UINT MappingFlags)
{
  UINTPTR FirstPage = FindLastFreeVirtualPages(PageCount);
  if (!FirstPage) return NULL;
  for (SIZE_T i = 0; i < PageCount; ++i)
  {
    if (!MapFreePage(FirstPage + i * PAGE_SIZE, MappingFlags))
    {
      for (SIZE_T j = 0; j < i; ++j)
      {
        (void)FreeMappedPage(FirstPage + j * PAGE_SIZE);
      }
      return NULL;
    }
  }
  return (void*)FirstPage;
}

void *MapBestFreePages(SIZE_T PageCount, UINT MappingFlags)
{
  UINTPTR FirstPage = FindBestFreeVirtualPages(PageCount);
  if (!FirstPage) return NULL;
  for (SIZE_T i = 0; i < PageCount; ++i)
  {
    if (!MapFreePage(FirstPage + i * PAGE_SIZE, MappingFlags))
    {
      for (SIZE_T j = 0; j < i; ++j)
      {
        (void)FreeMappedPage(FirstPage + j * PAGE_SIZE);
      }
      return NULL;
    }
  }
  return (void*)FirstPage;
}

