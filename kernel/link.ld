ENTRY(entry)

SECTIONS
{
  . = 1M;
  .entry  : { PROVIDE(__entry_start = .);   *(.entry)   }
  .text   : { PROVIDE(__text_start = .);    *(.text)    }
  .data   : { PROVIDE(__data_start = .);    *(.data)    }
  .rodata : { PROVIDE(__rodata_start = .);  *(.rodata)  }
  .bss    : { PROVIDE(__bss_start = .);     *(.bss)     }
  PROVIDE(__end = .);
}
