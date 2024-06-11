ENTRY(_init)

SECTIONS
{
  . = 1M;

  .text :
  {
    KEEP(*(.init))
    *(.text)
  }

  .rodata :
  {
    *(.rodata)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(COMMON)
    *(.bss)
  }
}

