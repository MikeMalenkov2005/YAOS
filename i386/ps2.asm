global _init_ps2

global _read_first_ps2
global _read_second_ps2

extern _set_irq_handler

PS2_POWER equ 10

section .text

_init_ps2:
  xor eax, eax
  mov [FIRST_PS2_BOT], eax
  mov [FIRST_PS2_TOP], eax
  mov [SECOND_PS2_BOT], eax
  mov [SECOND_PS2_TOP], eax
  push _first_ps2_handler
  push 1
  call _set_irq_handler
  add esp, 8
  push _second_ps2_handler
  push 12
  call _set_irq_handler
  add esp, 8
  ret

_first_ps2_handler: ; STATE(IGNORED), POSITION(IGNORED)
  in al, 0x60
  mov ecx, [FIRST_PS2_TOP]
  mov [ecx + FIRST_PS2_BUFFER], al
  inc ecx
  and ecx, ((1 << PS2_POWER) - 1)
  mov [FIRST_PS2_TOP], ecx
  cmp ecx, [FIRST_PS2_BOT]
  jne .end
    inc ecx
    mov [FIRST_PS2_BOT], ecx
  .end:
  ret

_second_ps2_handler: ; STATE(IGNORED), POSITION(IGNORED)
  in al, 0x60
  mov ecx, [SECOND_PS2_TOP]
  mov [ecx + SECOND_PS2_BUFFER], al
  inc ecx
  and ecx, ((1 << PS2_POWER) - 1)
  mov [SECOND_PS2_TOP], ecx
  cmp ecx, [SECOND_PS2_BOT]
  jne .end
    inc ecx
    mov [SECOND_PS2_BOT], ecx
  .end:
  ret

_read_first_ps2:
  xor eax, eax
  not eax
  mov ecx, [FIRST_PS2_BOT]
  cmp ecx, [FIRST_PS2_TOP]
  je .end
    not eax
    mov al, [ecx + FIRST_PS2_BUFFER]
    inc ecx
    and ecx, ((1 << PS2_POWER) - 1)
    mov [FIRST_PS2_BOT], ecx
  .end:
  ret

_read_second_ps2:
  xor eax, eax
  not eax
  mov ecx, [SECOND_PS2_BOT]
  cmp ecx, [SECOND_PS2_TOP]
  je .end
    not eax
    mov al, [ecx + SECOND_PS2_BUFFER]
    inc ecx
    and ecx, ((1 << PS2_POWER) - 1)
    mov [SECOND_PS2_BOT], ecx
  .end:
  ret

section .bss

align 8

FIRST_PS2_BOT:  resd 1
FIRST_PS2_TOP:  resd 1
SECOND_PS2_BOT: resd 1
SECOND_PS2_TOP: resd 1

FIRST_PS2_BUFFER:
  resb (1 << PS2_POWER)

SECOND_PS2_BUFFER:
  resb (1 << PS2_POWER)

