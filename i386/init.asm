global _init

global _set_segs

global lcs

global STACK

extern _init_gdt
extern _init_idt
extern _init_vga
extern _init_ps2
extern _init_com
extern _init_pit

extern _set_pic_mask

extern kmain

STACK_POWER equ 12

section .init

_init:
  cli
  cld
  jmp _start

section .text

_start:
  xor eax, eax ; TODO: MAYBE DO MULTICORE IN THE FUTURE ?
  mov ecx, eax
  inc eax
  shl eax, STACK_POWER
  add eax, STACK
  mov esp, eax
  push ecx
  push eax
  call _init_gdt
  pop ebp
  call _init_idt
  push ebx
  call _init_vga
  pop ebx
  call _init_ps2
  call _init_com
  push 1000
  call _init_pit
  add esp, 4
  push 11101111b ; SPIC
  push 11111100b ; MPIC
  call _set_pic_mask
  add esp, 8
  call kmain
  pop ecx
  .wait:
   in al, 0x64
   test al, 2
   jnz .wait
  mov al, 0xFE
  out 0x64, al ; RESETS THE CPU I GUESS ?
  jmp $ ; TODO: IMPLEMENT PROPER SHUTDOWN ?

lcs: ; SEGMENT SELECTOR
  pop ecx
  mov eax, [esp]
  push eax
  push ecx
  retf

section .bss

align 0x1000

STACK:
  resb (1 << STACK_POWER)

TMP_VAR:
  resw 1
  resd 1

