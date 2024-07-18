global _save_task
global _load_task
global _get_task_flags
global _set_task_flags

section .text

_save_task: ; POSITION, STATE, TASK
  push esi
  push edi
  mov eax, cr3 ; call _get_mem_map
  mov edi, [esp + 20]
  test edi, edi
  jnz .not_sys
    mov edi, SYSTEM_TASK
  .not_sys:
  mov al, [edi]
  stosd
  mov esi, [esp + 12]
  mov ecx, 3 ; DWORDS IN POSITION
  rep movsd
  mov esi, [esp + 16]
  mov ecx, 12 ; DWORDS IN STATE
  rep movsd
  pop edi
  pop esi
  ret

_load_task: ; POSITION, STATE, TASK
  push esi
  push edi
  mov esi, [esp + 20]
  test esi, esi
  jnz .not_sys
    mov esi, SYSTEM_TASK
  .not_sys:
  lodsd
  mov al, 0
  mov cr3, eax ; call _set_mem_map
  mov edi, [esp + 12]
  mov ecx, 3 ; DWORDS IN POSITION
  rep movsd
  mov edi, [esp + 16]
  mov ecx, 12 ; DWORDS IN STATE
  rep movsd
  pop edi
  pop esi
  ret

_get_task_flags: ; TASK
  mov ecx, [esp + 4]
  test ecx, ecx
  jnz .not_sys
    mov ecx, SYSTEM_TASK
  .not_sys:
  xor eax, eax
  mov al, [ecx]
  ret

_set_task_flags: ; TASK, FLAGS
  mov ecx, [esp + 4]
  test ecx, ecx
  jnz .not_sys
    mov ecx, SYSTEM_TASK
  .not_sys:
  mov eax, [esp + 8]
  mov [ecx], al
  ret

section .bss

align 16

SYSTEM_TASK:
  resd 16

