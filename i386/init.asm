global _init

extern kmain

STACK_POWER equ 12

section .init

_init:
  cli
  cld
  jmp _start

section .text

_start:
  %if __SMP__ > 1
    xor eax, eax
  %else
    xor eax, eax
  %endif
  mov ecx, eax
  inc eax
  shl eax, STACK_POWER
  add eax, STACK_TABLE
  mov esp, eax
  push ecx
  push eax
  call _init_gdt
  pop ebp
  call kmain
  pop ecx
  jmp $

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
  mov ecx, TSS0.SIZE
  mul ecx
  pop ecx
  push eax
  add eax, TSS0

  push 0
  push 11101001b
  push (TSS0.SIZE - 1)
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
  mov ecx, TSS0.SIZE
  rep stosb
  pop edi

  pop ecx ; TSS0.SIZE * TSS INDEX
  mov eax, [esp + 4]
  mov [ecx + TSS0.esp0], eax
  mov al, 16
  mov [ecx + TSS0.ss0], al
  mov al, (8 | 3)
  mov [ecx + TSS0.cs], al
  mov al, (16 | 3)
  mov [ecx + TSS0.es], al
  mov [ecx + TSS0.ss], al
  mov [ecx + TSS0.ds], al
  mov [ecx + TSS0.fs], al
  mov [ecx + TSS0.gs], al

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
  mov eax, [esp + 8]
  mov [ecx + 2], al
  mov [ecx + 3], ah
  shr eax, 16
  mov [ecx + 4], al
  mov [ecx + 7], ah
  mov eax, [esp + 12]
  mov [ecx + 0], al
  mov [ecx + 1], ah
  shr eax, 8
  and ah, 15
  mov al, [esp + 20]
  shl al, 5
  or ah, al
  mov al, [esp + 16]
  mov [ecx + 5], al
  mov [ecx + 6], ah
  ret

section .bss

align 0x1000

STACK_TABLE:
  resb ((1 << STACK_POWER) * __SMP__)

GDT:
  resq (5 + __SMP__)
  .SIZE equ ($ - GDT)

%assign i 0
%rep __SMP__
TSS %+ i:
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
  .SIZE equ $ - (TSS %+ i)
  %assign i i + 1
%endrep

GDTR:
  resw 1
  resd 1

