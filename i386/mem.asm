global _init_mem

global _get_sys_mem_map
global _set_mem_map
global _get_mem_map

global _get_page_count
global _get_page_size

global _get_kernel_file_size
global _get_kernel_memory_size

KERNEL_START equ 0x100000

SYS_MEM_MAP equ 0x401000

section .text

_init_mem:
  push edi
  mov edi, 0x400000
  mov ecx, 0x100000
  mov eax, 3
  .l0:
    stosd
    add eax, 0x1000
    loop .l0
  mov eax, SYS_MEM_MAP
  mov cr3, eax
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax
  pop edi
  ret

_get_sys_mem_map:
  mov eax, SYS_MEM_MAP
  ret

_set_mem_map: ; MEM MAP
  mov eax, [esp + 4]
  mov al, 0x00
  and ah, 0xF0
  mov cr3, eax
  ret

_get_mem_map:
  mov eax, cr3
  ret

_get_page_count:
  mov eax, 0x100000
  ret

_get_page_size:
  mov eax, 0x1000

_get_kernel_file_size:
  mov eax, EOF
  sub eax, KERNEL_START
  ret

_get_kernel_memory_size:
  mov eax, EOM
  sub eax, KERNEL_START
  ret

section .eof

EOF:

section .eom

EOM:

