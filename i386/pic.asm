global _init_pic
global _irq_handler
global _set_pic_mask
global _set_irq_handler

section .text

_init_pic: ; MPIC REMAP, SPIC REMAP
  mov al, 0x11
  out 0x20, al
  out 0xA0, al
  mov al, [esp + 4]
  out 0x21, al
  mov al, [esp + 8]
  out 0xA1, al
  mov al, 4
  out 0x21, al
  mov al, 2
  out 0xA1, al
  mov al, 1
  out 0x21, al
  out 0xA1, al
  mov al, 0xFF
  out 0x21, al
  out 0xA1, al
  ret

_irq_handler: ; REGS(48 bytes), IRQ INDEX, EIP, CS, EFLAGS
  mov ecx, [esp + 52]
  mov eax, [IRQ_HANDLER_TABLE + ecx * 4]
  test eax, eax
  jz .no_handler
    mov ecx, esp
    add ecx, 56
    push ecx ; POSITION
    sub ecx, 52
    push ecx ; STATE
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

_set_pic_mask: ; MPIC MASK, SPIC MASK
  mov al, [esp + 4]
  out 0x21, al
  mov al, [esp + 8]
  out 0xA1, al
  ret

_set_irq_handler: ; INDEX, FUNCTION POINTER
  mov ecx, [esp + 4]
  mov eax, [esp + 8]
  mov [IRQ_HANDLER_TABLE + ecx * 4], eax
  ret

section .bss

align 8

IRQ_HANDLER_TABLE:
  resd 16

