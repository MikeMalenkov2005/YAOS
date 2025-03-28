#include <kernel/image.h>

#include <kernel/memory.h>
#include <sys/elf.h>

static UINT16 ELF32Machine;
static UINT16 ELF64Machine;
static UINT16 COFF32Machine;
static UINT16 COFF64Machine;
static UINT8 ELFData;

BOOL LoadSegmentELF32(const ELF32_PROGRAM_HEADER *pHeader, const void *pImageData)
{
  if (pHeader->Type != PT_LOAD) return TRUE;
  if (pHeader->Alignment & PAGE_FLAGS_MASK) return FALSE;
  UINTPTR FirstPage = pHeader->VirtualAddress & PAGE_ADDRESS_MASK;
  SIZE_T PageCount = (PAGE_ROUND_UP(pHeader->VirtualAddress + pHeader->SizeInMemory) - FirstPage) / PAGE_SIZE;
  UINT MappingFlags = MAPPING_USER_MODE_BIT;
  if (pHeader->Flags & PF_R) MappingFlags |= MAPPING_READABLE_BIT;
  if (pHeader->Flags & PF_W) MappingFlags |= MAPPING_WRITABLE_BIT;
  if (pHeader->Flags & PF_X) MappingFlags |= MAPPING_EXECUTABLE_BIT;
  void *pStart = MapFreePages(FirstPage, PageCount, MappingFlags);
  if (!pStart) return FALSE;
  const UINT8 *pSource = pImageData + pHeader->Offset;
  UINT8 *pData = pStart + (pHeader->VirtualAddress & PAGE_FLAGS_MASK);
  SIZE_T Index = 0;
  for (;Index < pHeader->SizeInFile; ++Index) pData[Index] = pSource[Index];
  for (;Index < pHeader->SizeInMemory; ++Index) pData[Index] = 0;
  return TRUE;
}

BOOL FreeSegmentELF32(const ELF32_PROGRAM_HEADER *pHeader)
{
  if (pHeader->Type != PT_LOAD) return TRUE;
  if (pHeader->Alignment & PAGE_FLAGS_MASK) return FALSE;
  UINTPTR FirstPage = pHeader->VirtualAddress & PAGE_ADDRESS_MASK;
  SIZE_T PageCount = (PAGE_ROUND_UP(pHeader->VirtualAddress + pHeader->SizeInMemory) - FirstPage) / PAGE_SIZE;
  (void)FreeMappedPages(FirstPage, PageCount);
  return TRUE;
}

BOOL LoadSegmentELF64(const ELF64_PROGRAM_HEADER *pHeader, const void *pImageData)
{
  if (pHeader->Type != PT_LOAD) return TRUE;
  if (pHeader->Alignment & PAGE_FLAGS_MASK) return FALSE;
  UINTPTR FirstPage = pHeader->VirtualAddress & PAGE_ADDRESS_MASK;
  SIZE_T PageCount = PAGE_ROUND_UP(pHeader->VirtualAddress + pHeader->SizeInMemory) / PAGE_SIZE;
  UINT MappingFlags = MAPPING_USER_MODE_BIT;
  if (pHeader->Flags & PF_R) MappingFlags |= MAPPING_READABLE_BIT;
  if (pHeader->Flags & PF_W) MappingFlags |= MAPPING_WRITABLE_BIT;
  if (pHeader->Flags & PF_X) MappingFlags |= MAPPING_EXECUTABLE_BIT;
  void *pStart = MapFreePages(FirstPage, PageCount, MappingFlags);
  if (!pStart) return FALSE;
  const UINT8 *pSource = pImageData + pHeader->Offset;
  UINT8 *pData = pStart + (pHeader->VirtualAddress & PAGE_FLAGS_MASK);
  SIZE_T Index = 0;
  for (;Index < pHeader->SizeInFile; ++Index) pData[Index] = pSource[Index];
  for (;Index < pHeader->SizeInMemory; ++Index) pData[Index] = 0;
  return TRUE;
}

BOOL FreeSegmentELF64(const ELF64_PROGRAM_HEADER *pHeader)
{
  if (pHeader->Type != PT_LOAD) return TRUE;
  if (pHeader->Alignment & PAGE_FLAGS_MASK) return FALSE;
  UINTPTR FirstPage = pHeader->VirtualAddress & PAGE_ADDRESS_MASK;
  UINTPTR TopAddress = pHeader->VirtualAddress + pHeader->SizeInMemory;
  for (UINTPTR Page = FirstPage; Page < TopAddress; Page += PAGE_SIZE) (void)FreeMappedPage(Page);
  return TRUE;
}

BOOL LoadImage(UINTPTR *pEntryPoint, void *pImageData, SIZE_T ImageSize)
{
  if (!pImageData || ImageSize < 16) return FALSE;
  UINT8 *pIdent = pImageData;
  if (pIdent[EI_MAG0] != 0x7F ||
      pIdent[EI_MAG1] != 'E' ||
      pIdent[EI_MAG2] != 'L' ||
      pIdent[EI_MAG3] != 'F' ||
      !((pIdent[EI_CLASS] == ELFCLASS32 && ELF32Machine) ||
        (pIdent[EI_CLASS] == ELFCLASS64 && ELF64Machine)) ||
      pIdent[EI_DATA] != ELFData ||
      pIdent[EI_VERSION] != EV_CURRENT ||
      pIdent[EI_OSABI] != ELFOSABI_NONE ||
      pIdent[EI_ABIVERSION])
    return FALSE;
  if (pIdent[EI_CLASS] == ELFCLASS32)
  {
    ELF32_FILE_HEADER *pFileHeader = pImageData;
    if (ImageSize < sizeof(ELF32_FILE_HEADER) ||
        pFileHeader->Type != ET_EXEC ||
        pFileHeader->Machine != ELF32Machine ||
        pFileHeader->Version != EV_CURRENT ||
        !pFileHeader->ProgramHeaderTableOffset ||
        pFileHeader->FileHeaderSize != sizeof(ELF32_FILE_HEADER) ||
        pFileHeader->ProgramHeaderSize != sizeof(ELF32_PROGRAM_HEADER) ||
        !pFileHeader->ProgramHeaderCount ||
        pFileHeader->SectionHeaderSize != sizeof(ELF32_SECTION_HEADER) ||
        pFileHeader->ProgramHeaderTableOffset + pFileHeader->ProgramHeaderCount * pFileHeader->ProgramHeaderSize > ImageSize)
      return FALSE;
    ELF32_PROGRAM_HEADER *pProgramHeaderTable = (pImageData + pFileHeader->ProgramHeaderTableOffset);
    for (UINT16 i = 0; i < pFileHeader->ProgramHeaderCount; ++i)
      if (pProgramHeaderTable[i].Offset + pProgramHeaderTable[i].SizeInFile > ImageSize) return FALSE;
    for (UINT16 i = 0; i < pFileHeader->ProgramHeaderCount; ++i)
    {
      if (!LoadSegmentELF32(&pProgramHeaderTable[i], pImageData))
      {
        for (UINT16 j = 0; j < i; ++j) (void)FreeSegmentELF32(&pProgramHeaderTable[j]);
        return FALSE;
      }
    }
    if (pEntryPoint) *pEntryPoint = pFileHeader->EntryPoint;
  }
  else
  {
    ELF64_FILE_HEADER *pFileHeader = pImageData;
    if (ImageSize < sizeof(ELF64_FILE_HEADER) ||
        pFileHeader->Type != ET_EXEC ||
        pFileHeader->Machine != ELF64Machine ||
        pFileHeader->Version != EV_CURRENT ||
        !pFileHeader->ProgramHeaderTableOffset ||
        pFileHeader->FileHeaderSize != sizeof(ELF64_FILE_HEADER) ||
        pFileHeader->ProgramHeaderSize != sizeof(ELF64_PROGRAM_HEADER) ||
        !pFileHeader->ProgramHeaderCount ||
        pFileHeader->SectionHeaderSize != sizeof(ELF64_SECTION_HEADER) ||
        pFileHeader->ProgramHeaderTableOffset + pFileHeader->ProgramHeaderCount * pFileHeader->ProgramHeaderSize > ImageSize)
      return FALSE;
    ELF64_PROGRAM_HEADER *pProgramHeaderTable = (pImageData + pFileHeader->ProgramHeaderTableOffset);
    for (UINT16 i = 0; i < pFileHeader->ProgramHeaderCount; ++i)
      if (pProgramHeaderTable[i].Offset + pProgramHeaderTable[i].SizeInFile > ImageSize) return FALSE;
    for (UINT16 i = 0; i < pFileHeader->ProgramHeaderCount; ++i)
    {
      if (!LoadSegmentELF64(&pProgramHeaderTable[i], pImageData))
      {
        for (UINT16 j = 0; j < i; ++j) (void)FreeSegmentELF64(&pProgramHeaderTable[j]);
        return FALSE;
      }
    }
    if (pEntryPoint) *pEntryPoint = pFileHeader->EntryPoint;
  }
  return TRUE;
}

void SetArchInfo(UINT32 ELFMachinePair, UINT32 COFFMachinePair, BOOL bNetworkOrder)
{
  ELF32Machine = ELFMachinePair;
  ELF64Machine = ELFMachinePair >> 16;
  COFF32Machine = COFFMachinePair;
  COFF64Machine = COFFMachinePair >> 16;
  ELFData = bNetworkOrder ? ELFDATA2MSB : ELFDATA2LSB;
}

