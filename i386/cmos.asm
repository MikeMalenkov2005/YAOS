global _read_cmos
global _write_cmos

section .text

_read_cmos: ; REGISTER
  mov al, [esp + 4]
  out 0x70, al
  xor eax, eax
  in al, 0x71
  ret

_write_cmos: ; REGISTER, BYTE
  mov al, [esp + 4]
  out 0x70, al
  mov al, [esp + 8]
  out 0x71, al
  ret

