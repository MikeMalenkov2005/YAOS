global _init

global _get_user_stack
global _set_user_stack

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

_init_gdt: ; STACK BASE, TSS INDEX
  push 0
  push 0
  push 0
  push 0
  push 0
  call _set_gdt_entry
  add esp, 20
  push 110b
  push 10011010b
  push 0xFFFFF
  push 0
  push 1
  call _set_gdt_entry
  mov al, 10010010b
  mov [esp + 12], al
  pop eax
  inc eax
  push eax
  call _set_gdt_entry
  mov al, 11111010b
  mov [esp + 12], al
  pop eax
  inc eax
  push eax
  call _set_gdt_entry
  mov al, 11110010b
  mov [esp + 12], al
  pop eax
  inc eax
  push eax
  call _set_gdt_entry
  add esp, 20
  
  mov eax, [esp + 8]
  mov ecx, eax
  add ecx, 5
  push ecx
  mov ecx, TSS.SIZE
  mul ecx
  pop ecx
  push eax
  add eax, TSS

  push 0
  push 11101001b
  push (TSS.SIZE - 1)
  push eax
  push ecx
  call _set_gdt_entry
  pop ecx
  pop eax
  add esp, 12
  sub ecx, 5

  push edi
  mov edi, eax
  xor eax, eax
  mov ecx, TSS.SIZE
  rep stosb
  pop edi

  pop ecx ; TSS.SIZE * TSS INDEX
  mov eax, [esp + 4]
  mov [ecx + TSS.esp0], eax
  mov al, 16
  mov [ecx + TSS.ss0], al
  mov al, (8 | 3)
  mov [ecx + TSS.cs], al
  mov al, (16 | 3)
  mov [ecx + TSS.es], al
  mov [ecx + TSS.ss], al
  mov [ecx + TSS.ds], al
  mov [ecx + TSS.fs], al
  mov [ecx + TSS.gs], al

  mov word [GDTR], GDT.SIZE - 1
  mov dword [GDTR + 2], GDT
  lgdt [GDTR]
  mov eax, 16
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 8:.finish
.finish:
  mov eax, [esp + 8]
  add eax, 5
  shl eax, 3
  or al, 3
  ltr ax
  ret

_set_gdt_entry: ; INDEX, BASE, LIMIT, ACCESS[ P |  DPL  | S |     TYPE      ], FLAGS[  G | DB | L  ]
  mov ecx, [esp + 4]
  shl ecx, 3
  add ecx, GDT
  mov eax, [esp + 12]
  mov [ecx], eax
  shr eax, 8
  and ah, 15
  mov al, [esp + 20]
  shl al, 5
  or al, ah
  mov [ecx + 6], al
  mov eax, [esp + 8]
  mov [ecx + 2], eax
  shr eax, 24
  mov [ecx + 7], al
  mov al, [esp + 16]
  mov [ecx + 5], al
  ret

_get_user_stack:
  mov eax, [TSS.esp3]
  ret

_set_user_stack: ; STACK
  mov eax, [esp + 4]
  mov [TSS.esp1], eax
  mov [TSS.esp2], eax
  mov [TSS.esp3], eax
  ret

section .bss

align 0x1000

STACK:
  resb (1 << STACK_POWER)

TSS:
  .link:    resd 1
  .esp0:    resd 1
  .ss0:     resd 1
  .esp1:    resd 1
  .ss1:     resd 1
  .esp2:    resd 1
  .ss2:     resd 1
  .esp3:    resd 1
  .ss3:     resd 1
  .eip:     resd 1
  .eflags:  resd 1
  .eax:     resd 1
  .ecx:     resd 1
  .edx:     resd 1
  .ebx:     resd 1
  .esp:     resd 1
  .ebp:     resd 1
  .esi:     resd 1
  .edi:     resd 1
  .es:      resd 1
  .cs:      resd 1
  .ss:      resd 1
  .ds:      resd 1
  .fs:      resd 1
  .gs:      resd 1
  .ldtr:    resd 1
  .tflag:   resw 1
  .iopb:    resw 1
  .ssp:     resd 1
  .SIZE equ $ - TSS

GDT:
  resq 6
  .SIZE equ ($ - GDT)

GDTR:
  resw 1
  resd 1

