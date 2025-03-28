global entry

extern __entry_start
extern __bss_start
extern __end
extern InitArch

MBOOT_KERNEL_MAGIC equ 0x1BADB002
MBOOT_LOADER_MAGIC equ 0x2BADB002
MBOOT_KERNEL_FLAGS equ 3

section .entry

MBOOT_HEADER:
  dd MBOOT_KERNEL_MAGIC
  dd MBOOT_KERNEL_FLAGS
  dd -(MBOOT_KERNEL_MAGIC + MBOOT_KERNEL_FLAGS)
  dd MBOOT_HEADER
  dd __entry_start
  dd __bss_start
  dd __end
  dd entry
  dd 1
  dd 0
  dd 0
  dd 0

entry:
  mov ebp, STACK.TOP
  mov esp, ebp
  sub esp, 64
  push ebp
  push ebx
  push eax
  call InitArch
  add esp, 12
  pop eax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ds, ax
  popa
  add esp, 8
  iret

section .bss

align 0x1000

STACK:
  resb 0x1000
.TOP:
