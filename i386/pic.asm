global _irq_handler
global _set_pic_mask
global _set_irq_handler

global IRQ_HANDLER_TABLE

section .text

_irq_handler: ; REGS(48 bytes), IRQ INDEX, EIP, CS, EFLAGS
  mov ecx, [esp + 52]
  mov eax, [IRQ_HANDLER_TABLE + ecx * 4]
  test eax, eax
  jz .no_handler
    mov ecx, esp
    add ecx, 4
    push ecx ; STATE
    add ecx, 52
    push ecx ; POSITION
    call eax
    add esp, 8
  .no_handler:
  mov ecx, [esp + 52]
  mov al, 0x20
  cmp ecx, 8
  jb .mpic
    out 0xA0, al
  .mpic:
  out 0x20, al
  ret

section .bss

align 8

IRQ_HANDLER_TABLE:
  resd 16

