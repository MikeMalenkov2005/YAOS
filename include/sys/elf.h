#ifndef SYS_ELF_H
#define SYS_ELF_H

#include <types.h>

#define EI_MAG0       0
#define EI_MAG1       1
#define EI_MAG2       2
#define EI_MAG3       3
#define EI_CLASS      4
#define EI_DATA       5
#define EI_VERSION    6
#define EI_OSABI      7
#define EI_ABIVERSION 8
#define EI_PAD        9
#define EI_NIDENT     16

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFCLASSNONE  0
#define ELFCLASS32    1
#define ELFCLASS64    2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_CURRENT  1

#define ELFOSABI_NONE   0
#define ELFOSABI_HPUX   1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_LINUX  3

#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOOS   0xFE00
#define ET_HIOS   0xFEFF
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define EM_NONE   0
#define EM_386    3
#define EM_X86_64 62

typedef struct ELF32_FILE_HEADER
{
  UINT8 Ident[EI_NIDENT];
  UINT16 Type;
  UINT16 Machine;
  UINT32 Version;
  UINT32 EntryPoint;
  UINT32 ProgramHeaderTableOffset;
  UINT32 SectionHeaderTableOffset;
  UINT32 Flags;
  UINT16 FileHeaderSize;
  UINT16 ProgramHeaderSize;
  UINT16 ProgramHeaderCount;
  UINT16 SectionHeaderSize;
  UINT16 SectionHeaderCount;
  UINT16 StringTableSectionIndex;
} ELF32_FILE_HEADER;

typedef struct ELF64_FILE_HEADER
{
  UINT8 Ident[EI_NIDENT];
  UINT16 Type;
  UINT16 Machine;
  UINT32 Version;
  UINT64 EntryPoint;
  UINT64 ProgramHeaderTableOffset;
  UINT64 SectionHeaderTableOffset;
  UINT32 Flags;
  UINT16 FileHeaderSize;
  UINT16 ProgramHeaderSize;
  UINT16 ProgramHeaderCount;
  UINT16 SectionHeaderSize;
  UINT16 SectionHeaderCount;
  UINT16 StringTableSectionIndex;
} ELF64_FILE_HEADER;

#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC  2
#define PT_INTERP   3
#define PT_NOTE     4
#define PT_SHLIB    5
#define PT_PHDR     6
#define PT_TLS      7
#define PT_LOOS     0x60000000
#define PT_HIOS     0x6FFFFFFF
#define PT_LOPROC   0x70000000
#define PT_HIPROC   0x7FFFFFFF

#define PF_X        0x1
#define PF_W        0x2
#define PF_R        0x4
#define PF_MASKOS   0x0FF00000
#define PF_MASKPROC 0xF0000000

typedef struct ELF32_PROGRAM_HEADER
{
  UINT32 Type;
  UINT32 Offset;
  UINT32 VirtualAddress;
  UINT32 PhysicalAddress;
  UINT32 SizeInFile;
  UINT32 SizeInMemory;
  UINT32 Flags;
  UINT32 Alignment;
} ELF32_PROGRAM_HEADER;

typedef struct ELF64_PROGRAM_HEADER
{
  UINT32 Type;
  UINT32 Flags;
  UINT64 Offset;
  UINT64 VirtualAddress;
  UINT64 PhysicalAddress;
  UINT64 SizeInFile;
  UINT64 SizeInMemory;
  UINT64 Alignment;
} ELF64_PROGRAM_HEADER;

#define SHT_NULL          0
#define SHT_PROGBITS      1
#define SHT_SYMTAB        2
#define SHT_STRTAB        3
#define SHT_RELA          4
#define SHT_HASH          5
#define SHT_DYNAMIC       6
#define SHT_NOTE          7
#define SHT_NOBITS        8
#define SHT_REL           9
#define SHT_SHLIB         10
#define SHT_DYNSYM        11
#define SHT_INIT_ARRAY    14
#define SHT_FINI_ARRAY    15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP         17
#define SHT_SYMTAB_SHNDX  18
#define SHT_LOOS          0x60000000
#define SHT_HIOS          0x6FFFFFFF
#define SHT_LOPROC        0x70000000
#define SHT_HIPROC        0x7FFFFFFF
#define SHT_LOUSER        0x80000000
#define SHT_HIUSER        0x8FFFFFFF

#define SHF_WRITE             0x001
#define SHF_ALLOC             0x002
#define SHF_EXECINSTR         0x004
#define SHF_MERGE             0x010
#define SHF_STRINGS           0x020
#define SHF_INFO_LINK         0x040
#define SHF_LINK_ORDER        0x080
#define SHF_OS_NONCONFORMING  0x100
#define SHF_GROUP             0x200
#define SHF_TLS               0x400
#define SHF_COMPRESSED        0x800
#define SHF_MASKOS            0x0FF00000
#define SHF_MASKPROC          0xF0000000

typedef struct ELF32_SECTION_HEADER
{
  UINT32 Name;
  UINT32 Type;
  UINT32 Flags;
  UINT32 Address;
  UINT32 Offset;
  UINT32 Size;
  UINT32 Link;
  UINT32 Info;
  UINT32 Alignment;
  UINT32 EntrySize;
} ELF32_SECTION_HEADER;

typedef struct ELF64_SECTION_HEADER
{
  UINT32 Name;
  UINT32 Type;
  UINT64 Flags;
  UINT64 Address;
  UINT64 Offset;
  UINT64 Size;
  UINT32 Link;
UINT32 Info;
  UINT64 Alignment;
  UINT64 EntrySize;
} ELF64_SECTION_HEADER;

#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_NUM     3
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6
#define STT_NUM     7
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

#define ST_BIND(Info)       ((UINT8)(Info) >> 4)
#define ST_TYPE(Info)       ((Info) & 0xF)
#define ST_INFO(Bind, Type) (((Bind) << 4) | ((Type) & 0xF))

#define STV_DEFAULT   0
#define STV_PROTECTED 1
#define STV_HIDDEN    2
#define STV_INTERNAL  3

typedef struct ELF32_SYMBOL
{
  UINT32 Name;
  UINT32 Value;
  UINT32 Size;
  UINT8 Info;
  UINT8 Other;
  UINT16 Section;
} ELF32_SYMBOL;

typedef struct ELF64_SYMBOL
{
  UINT32 Name;
  UINT8 Info;
  UINT8 Other;
  UINT16 Section;
  UINT64 Value;
  UINT64 Size;
} ELF64_SYMBOL;

#define DT_NULL             0
#define DT_NEEDED           1
#define DT_PLTRELSZ         2
#define DT_PLTGOT           3
#define DT_HASH             4
#define DT_STRTAB           5
#define DT_SYMTAB           6
#define DT_RELA             7
#define DT_RELASZ           8
#define DT_RELAENT          9
#define DT_STRSZ            10
#define DT_SYMENT           11
#define DT_INIT             12
#define DT_FINI             13
#define DT_SONAME           14
#define DT_RPATH            15
#define DT_SYMBOLIC         16
#define DT_REL              17
#define DT_RELSZ            18
#define DT_RELENT           19
#define DT_PLTREL           20
#define DT_DEBUG            21
#define DT_TEXTREL          22
#define DT_JMPREL           23
#define	DT_BIND_NOW         24
#define	DT_INIT_ARRAY       25
#define	DT_FINI_ARRAY       26
#define	DT_INIT_ARRAYSZ     27
#define	DT_FINI_ARRAYSZ     28
#define DT_RUNPATH          29
#define DT_FLAGS            30
#define DT_ENCODING         32
#define DT_PREINIT_ARRAY    32
#define DT_PREINIT_ARRAYSZ  33
#define DT_SYMTAB_SHNDX     34
#define	DT_NUM              35
#define DT_LOOS             0x6000000d
#define DT_HIOS             0x6ffff000
#define DT_LOPROC           0x70000000
#define DT_HIPROC           0x7fffffff

typedef struct ELF32_DYNAMIC
{
  INT32 Tag;
  UINT32 Value;
} ELF32_DYNAMIC;

typedef struct ELF64_DYNAMIC
{
  INT64 Tag;
  UINT64 Value;
} ELF64_DYNAMIC;

#define R_386_NONE      0
#define R_386_32        1
#define R_386_PC32      2
#define R_386_GOT32     3
#define R_386_PLT32     4
#define R_386_COPY      5 
#define R_386_GLOB_DAT  6
#define R_386_JMP_SLOT  7
#define R_386_RELATIVE  8
#define R_386_GOTOFF    9
#define R_386_GOTPC     10

 typedef struct ELF32_RELOCATION
{
  UINT32 Offset;
  UINT32 Info;
} ELF32_RELOCATION;

typedef struct ELF64_RELOCATION
{
  UINT64 Offset;
  UINT64 Info;
} ELF64_RELOCATION;

typedef struct ELF32_RELOCATION_ADDEND
{
  UINT32 Offset;
  UINT32 Info;
  UINT32 Addend;
} ELF32_RELOCATION_ADDEND;

typedef struct ELF64_RELOCATION_ADDEND
{
  UINT64 Offset;
  UINT64 Info;
  UINT64 Addend;
} ELF64_RELOCATION_ADDEND;

#endif

