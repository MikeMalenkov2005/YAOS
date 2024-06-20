global _init_com

section .text

_init_com:
  push esi
  push edi
  pop edi
  pop esi
  ret

section .bss

COM_PORTS: resd 4

