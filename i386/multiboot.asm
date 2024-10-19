global _start

extern kinit

default rel

MB_ALIGN_BIT  equ (1 << 0)
MB_MEMORY_BIT equ (1 << 1)
MB_VIDEO_BIT  equ (1 << 2)
MB_LAYOUT_BIT equ (1 << 16)

MB_OSMAG  equ 0x1BADB002
MB_BLMAG  equ 0x2BADB002
MB_FLAGS  equ (MB_ALIGN_BIT | MB_VIDEO_BIT)

section .text

_start:
  cli
  cld
  cmp eax, MB_BLMAG
  jne .halt
  lea eax, [STACK.TOP]
  mov esp, eax
  push eax ; STACK TOP POINTER
  push ebx ; BOOT INFO POINTER
  call kinit
  pop ebx
  pop eax
.halt:
  cli
  hlt
  jmp .halt

align 4
MB_HEADER:
  dd MB_OSMAG
  dd MB_FLAGS
  dd -(MB_OSMAG + MB_FLAGS)
  times 5 dd 0
  dd 1
  dd 0
  dd 0
  dd 0

section .bss

align 0x1000

STACK:
  resb 0x1000
  .TOP:
