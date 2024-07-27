global _init_ps2

global _clear_first_ps2
global _clear_second_ps2

global _read_first_ps2
global _read_second_ps2

global _write_first_ps2
global _write_second_ps2

extern _set_irq_handler

PS2_POWER equ 6

section .text

_init_ps2:
  mov al, 0x20
  out 0x64, al
  in al, 0x60
  and al, 0xBF
  or al, 3
  mov ah, al
  mov al, 0x60
  out 0x64, al
  mov al, ah
  out 0x60, al
  call _clear_first_ps2
  call _clear_second_ps2
  push _first_ps2_handler
  push 1
  call _set_irq_handler
  add esp, 8
  push _second_ps2_handler
  push 12
  call _set_irq_handler
  add esp, 8
  ret

_first_ps2_handler: ; POSITION(IGNORED), STATE(IGNORED)
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

_second_ps2_handler: ; POSITION(IGNORED), STATE(IGNORED)
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

_clear_first_ps2:
  xor eax, eax
  mov [FIRST_PS2_BOT], eax
  mov [FIRST_PS2_TOP], eax
  ret

_clear_second_ps2:
  xor eax, eax
  mov [SECOND_PS2_BOT], eax
  mov [SECOND_PS2_TOP], eax
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

_write_first_ps2: ; BYTE
  xor ecx, ecx
  mov cl, 0x10
  .wait:
    in al, 0x64
    test al, 2
    jz .good
    loop .wait
    xor eax, eax
    not eax
    ret
  .good:
  mov al, [esp + 4]
  out 0x60, al
  xor eax, eax
  ret

_write_second_ps2: ; BYTE
  mov al, 0xD4
  out 0x64, al
  xor ecx, ecx
  mov cl, 0x10
  .wait:
    in al, 0x64
    test al, 2
    jz .good
    loop .wait
    xor eax, eax
    not eax
    ret
  .good:
  mov al, [esp + 4]
  out 0x60, al
  xor eax, eax
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

