OUTPUT_ARCH(i386)
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

SECTIONS {
  . = 1M;

  .text : {
    *(.text)
    PROVIDE(etext = .);
  }

  .rodata : {
    *(.rodata)
  }

  .data : {
    *(.data)
    PROVIDE(edata = .);
  }

  .bss : {
    *(.bss)
    *(COMMON)
    PROVIDE(end = .);
  }
}

