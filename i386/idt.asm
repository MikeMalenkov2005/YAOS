global _init_idt

extern _init_pic
extern _irq_handler

extern kcall
extern kerror

section .text

_init_idt:
  push edi
  mov edi, IDT
  mov ecx, IDT.SIZE - 1
  mov [IDTR], ecx
  mov [IDTR + 2], edi
  inc ecx
  xor eax, eax
  rep stosb
  pop edi
  push 40
  push 32
  call _init_pic
  add esp, 8
  
  push 1
  push 8
  %assign i 0
  %rep 32
    push _isr %+ i
    push i
    call _set_idt_entry
    add esp, 8
    %assign i (i + 1)
  %endrep
  %assign j 0
  %rep 16
    push _irq %+ j
    push i
    call _set_idt_entry
    add esp, 8
    %assign i i + 1
    %assign j j + 1
  %endrep
  add esp, 8

  push 0
  push 8
  push _syscall
  push 0x80
  call _set_idt_entry
  add esp, 16

  lidt [IDTR]
  sti
  ret

_set_idt_entry: ; INDEX, OFFSET, SEGMENT, CRITICAL FLAG (PREVENTS SUBSEQUENT INTERRUPTS)
  mov ecx, [esp + 4]
  shl ecx, 3
  add ecx, IDT
  mov eax, [esp + 8]
  mov [ecx], eax
  mov [ecx + 4], eax
  mov eax, [esp + 12]
  mov [ecx + 2], eax
  mov al, [esp + 16]
  and al, 1
  xor al, 0xEF
  shl eax, 8
  mov [ecx + 4], al
  mov [ecx + 5], ah
  ret

%macro ISR_NOERRORCODE 1
_isr%1:
  push 0
  push %1
  jmp _isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
_isr%1:
  push %1
  jmp _isr_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE 8
ISR_NOERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31

_isr_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs
  mov eax, cr2
  push eax
  mov eax, 16
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  call _isr_handler
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret

_isr_handler: ; CR2, REGS(48 bytes), ISR INDEX, ERROR CODE, EIP, CS, EFLAGS
  mov eax, [esp + 56]
  shl eax, 5
  add eax, ERROR_TABLE
  push eax
  mov eax, esp
  add eax, 8
  push eax
  add eax, 56
  push eax
  call kerror
  add esp, 12
  ret

%macro IRQ 1
_irq%1:
  push %1
  jmp _irq_common_stub
%endmacro

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

_irq_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs
  mov eax, 16
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  call _irq_handler
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 4
  iret

_syscall:
  cli ; SHOULD PROBABLY BE REMOVED LATER
  pusha
  push ds
  push es
  push fs
  push gs
  push eax
  mov eax, 16
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  pop eax
  ; MAYBE ADD STI HERE OR POPFD WITH PUSHFD IN THE BEGINNING
  push ebp
  push edi
  push esi
  push edx
  push ecx
  push ebx
  push eax
  call kcall
  add esp, 28
  cli
  mov [esp + 44], eax ; RETURN VALUE
  pop gs
  pop fs
  pop es
  pop ds
  popa
  iret

section .data

align 32, db 0

ERROR_TABLE:
  db "Devision by zero"
  align 32, db 0
  db "Debug exception"
  align 32, db 0
  db "Non maskable interrupt"
  align 32, db 0
  db "Breakpoint"
  align 32, db 0
  db "Overflow"
  align 32, db 0
  db "Bounds check"
  align 32, db 0
  db "Invalid opcode"
  align 32, db 0
  db "Coprocessor not available"
  align 32, db 0
  db "Double fault"
  align 32, db 0
  db "Coprocessor segment overrun"
  align 32, db 0
  db "Invalid TSS"
  align 32, db 0
  db "Segment not present"
  align 32, db 0
  db "Stack exception"
  align 32, db 0
  db "General protection exception"
  align 32, db 0
  db "Page fault"
  align 32, db 0
  db "Coprocessor exception"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0
  db "Reserved"
  align 32, db 0

section .bss

align 0x1000

IDT:
  resq 256
  .SIZE equ $ - IDT

IDTR:
  resw 1
  resd 1

