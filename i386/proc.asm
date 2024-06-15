global _init_process_system
global _get_process
global _save_process
global _load_process

extern _get_core_index

section .text

_init_process_system:
  xor eax, eax
  mov [CURRENT_PROCESS_TABLE], eax
  ret

_get_process:
  mov eax, [CURRENT_PROCESS_TABLE]
  ret

_save_process:
  ret

_load_process:
  ret

section .bss

align 8

CURRENT_PROCESS_TABLE: resd 1

