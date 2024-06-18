global _init_pit

global _play_sound

extern _set_irq_handler

extern ktimer

section .text

_init_pit: ; FREQUENCY
  push ktimer
  push 0
  call _set_irq_handler
  add esp, 8
  mov ecx, [esp + 4]
  test ecx, ecx
  jz .end
    xor edx, edx
    mov eax, 1193180
    div ecx
    mov ecx, eax
    mov al, 0x36
    out 0x43, al
    mov al, cl
    out 0x40, al
    mov al, ch
    out 0x40, al
  .end:
  ret

_play_sound: ; FREQUENCY
  mov ecx, [esp + 4]
  test ecx, ecx
  jz .no_sound
  xor edx, edx
  mov eax, 1193180 ; MAGIC THING
  div ecx
  mov ecx, eax
  mov al, 0xB6
  out 0x43, al
  mov al, cl
  out 0x42, al
  mov al, ch
  out 0x42, al
  in al, 0x61
  mov ah, al
  or al, 3
  cmp ah, al
  je .end
  out 0x61, al
  .end:
  ret
.no_sound:
  in al, 0x61
  and al, 0xFC
  out 0x61, al
  ret

