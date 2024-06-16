global _init_proc
global _get_process
global _save_process
global _load_process

section .text

_init_proc:
  xor eax, eax
  mov [CURRENT_PROCESS], eax
  ret

_get_process:
  mov eax, [CURRENT_PROCESS]
  ret

_save_process:
  ret

_load_process:
  ret

section .bss

align 8

CURRENT_PROCESS: resd 1

