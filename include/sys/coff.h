#ifndef SYS_COFF_H
#define SYS_COFF_H

#include <types.h>

/* Machine Types */
#define COFF_MACHINE_I386   0x14C
#define COFF_MACHINE_AMD64  0x8664

/* COFF File Flags */
#define COFF_FILE_RELOCATIONS_STRIPPED  0x0001
#define COFF_FILE_EXECUTABLE_IMAGE      0x0002
#define COFF_FILE_LINE_NUMBERS_STRIPPED 0x0004
#define COFF_FILE_SYMBOLS_STRIPPED      0x0008
#define COFF_FILE_32BIT_MACHINE         0x0100

typedef struct COFF_FILE_HEADER
{
  UINT16 Machine;
  UINT16 SectionCount;
  UINT32 TimeDateStamp;
  UINT32 SymbolTableOffset;
  UINT32 SymbolCount;
  UINT16 OptionalHeaderSize;
  UINT16 Flags;
} COFF_FILE_HEADER;

/* COFF Optional Header Magic Numbers */
#define COFF_32BIT_ROM_MAGIC    0x107
#define COFF_32BIT_IMAGE_MAGIC  0x10B
#define COFF_64BIT_IMAGE_MAGIC  0x20B

typedef struct COFF_OPTIONAL_HEADER
{
  UINT16 Magic;
  UINT16 VersionStamp;
  UINT32 TextSize;
  UINT32 DataSize;
  UINT32 BssSize;
  UINT32 EntryPoint;
  UINT32 TextStart;
  UINT32 DataStart;
} COFF_OPTIONAL_HEADER;

#define COFF_SCN_TEXT 0x0020
#define COFF_SCN_DATA 0x0040
#define COFF_SCN_BSS  0x0080

typedef struct COFF_SECTION_HEADER
{
  UINT8 Name[8];
  UINT32 VirtualSize;
  UINT32 VirtualAddress;
  UINT32 SectionDataSize;
  UINT32 SectionDataOffset;
  UINT32 RelocationTableOffset;
  UINT32 LineNumberTableOffset;
  UINT16 RelocationCount;
  UINT16 LineNumberCount;
  UINT32 Flags;
} COFF_SECTION_HEADER;

#define COFF_REL_I386_ABSOLUTE  0
#define COFF_REL_I386_DIR16     1
#define COFF_REL_I386_REL16     2
#define COFF_REL_I386_DIR32     6
#define COFF_REL_I386_DIR32NB   7
#define COFF_REL_I386_SECTION   10
#define COFF_REL_I386_SECREL    11
#define COFF_REL_I386_TOKEN     12
#define COFF_REL_I386_SECREL7   13
#define COFF_REL_I386_REL32     20

typedef struct COFF_RELOCATION
{
  UINT32 VirtualAddress;
  UINT32 SymbolIndex;
  UINT16 Type;
} COFF_RELOCATION;

typedef struct COFF_LINE_NUMBER
{
  union
  {
    UINT32 SymbolIndex;
    UINT32 VirtualAddress;
  };
  UINT16 LineNumber;
} COFF_LINE_NUMBER;

#define COFF_SYM_UNDEFINED  0
#define COFF_SYM_ABSOLUTE   -1
#define COFF_SYM_DEBUG      -2

#define COFF_SYM_TYPE_NULL    0
#define COFF_SYM_TYPE_VOID    1
#define COFF_SYM_TYPE_CHAR    2
#define COFF_SYM_TYPE_SHORT   3
#define COFF_SYM_TYPE_INT     4
#define COFF_SYM_TYPE_LONG    5
#define COFF_SYM_TYPE_FLOAT   6
#define COFF_SYM_TYPE_DOUBLE  7
#define COFF_SYM_TYPE_STRUCT  8
#define COFF_SYM_TYPE_UNION   9
#define COFF_SYM_TYPE_ENUM    10
#define COFF_SYM_TYPE_MOE     11
#define COFF_SYM_TYPE_BYTE    12
#define COFF_SYM_TYPE_WORD    13
#define COFF_SYM_TYPE_UINT    14
#define COFF_SYM_TYPE_DWORD   15

#define COFF_SYM_DTYPE_NULL     0
#define COFF_SYM_DTYPE_POINTER  1
#define COFF_SYM_DTYPE_FUNCTION 2
#define COFF_SYM_DTYPE_ARRAY    3

#define COFF_SYM_CLASS_NULL             0
#define COFF_SYM_CLASS_AUTOMATIC        1
#define COFF_SYM_CLASS_EXTERNAL         2
#define COFF_SYM_CLASS_STATIC           3
#define COFF_SYM_CLASS_REGISTER         4
#define COFF_SYM_CLASS_EXTERNAL_DEF     5
#define COFF_SYM_CLASS_LABEL            6
#define COFF_SYM_CLASS_UNDEFINED_LABEL  7
#define COFF_SYM_CLASS_MEMBER_OF_STRUCT 8
#define COFF_SYM_CLASS_ARGUMENT         9
#define COFF_SYM_CLASS_STRUCT_TAG       10
#define COFF_SYM_CLASS_MEMBER_OF_UNION  11
#define COFF_SYM_CLASS_UNION_TAG        12
#define COFF_SYM_CLASS_TYPE_DEFINITION  13
#define COFF_SYM_CLASS_UNDEFINED_STATIC 14
#define COFF_SYM_CLASS_ENUM_TAG         15
#define COFF_SYM_CLASS_MEMBER_OF_ENUM   16
#define COFF_SYM_CLASS_REGISTER_PARAM   17
#define COFF_SYM_CLASS_BIT_FIELD        18
#define COFF_SYM_CLASS_BLOCK            100
#define COFF_SYM_CLASS_FUNCTION         101
#define COFF_SYM_CLASS_END_OF_STRUCT    102
#define COFF_SYM_CLASS_FILE             103
#define COFF_SYM_CLASS_SECTION          104
#define COFF_SYM_CLASS_WEAK_EXTERNAL    105
#define COFF_SYM_CLASS_CLR_TOKEN        107
#define COFF_SYM_CLASS_END_OF_FUNCTION  255

typedef struct COFF_SYMBOL
{
  union 
  {
    UINT8 ShortName[8];
    struct
    {
      UINT32 Zeroes;
      UINT32 Offset;
    };
  } Name;
  UINT32 Value;
  INT16 Section; /* One Based Index or a Special Value */
  UINT16 Type;
  UINT8 Class;
  UINT8 AuxiliaryCount;
} COFF_SYMBOL;

#define COFF_AUXILIARY_SIZE 16

#endif

