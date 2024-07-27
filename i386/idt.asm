global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

global _sys

extern _isr_handler
extern _irq_handler

extern kcall

section .text

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
  mov eax, 16
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, [esp + 52]
  push eax
  mov eax, [esp + 52]
  push eax
  lea eax, [esp + 8]
  push eax
  add eax, 56
  push eax
  call _isr_handler
  add esp, 8
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret

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
  mov eax, [esp + 48]
  push eax
  lea eax, [esp + 4]
  push eax
  add eax, 52
  push eax
  call _irq_handler
  add esp, 12
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 4
  iret

_sys:
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
  ;mov eax, esp
  ;add eax, 28
  ;push eax ; STATE
  ;add eax, 48
  ;push eax ; POSITION
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

