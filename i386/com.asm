global _init_com

global _init_com_port
global _read_com_port
global _write_com_port

COM_COUNT equ 4
COM_POWER equ 9

section .text

_init_com:
  push esi
  push edi
  mov esi, 0x400
  mov edi, COM_PORTS
  mov ecx, COM_COUNT
  .l0:
    xor eax, eax
    lodsb
    mov ah, al
    lodsb
    xchg al, ah
    stosd
    test eax, eax
    jz .fi
      inc eax
      mov edx, eax
      xor eax, eax
      out dx, al
    .fi:
    loop .l0
  pop edi
  pop esi
  ret

_init_com_port: ; COM INDEX, BAUD RATE, DATA BITS, PARITY MODE, STOP BITS
  mov ecx, [esp + 8]
  test ecx, ecx
  jz .err
  xor edx, edx
  mov eax, 115200
  div ecx
  mov ecx, [esp + 4]
  cmp ecx, COM_COUNT
  jae .err
  mov edx, [COM_PORTS + ecx * 4]
  test edx, edx
  jz .err
  mov ecx, eax
  add edx, 1
  mov al, 0x00
  out dx, al
  add edx, 2
  mov al, 0x80
  out dx, al
  sub edx, 3
  mov al, cl
  out dx, al
  add edx, 1
  mov al, ch
  out dx, al
  add edx, 2
  mov al, [esp + 12]
  sub al, 5
  and al, 3
  mov ah, [esp + 16]
  and ah, 7
  shl ah, 3
  or al, ah
  mov ah, [esp + 20]
  dec ah
  and ah, 1
  shl ah, 2
  or al, ah
  out dx, al
  sub edx, 1
  mov al, 0xC7
  out dx, al
  add edx, 2
  mov al, 0x1E
  out dx, al
  sub edx, 4
  mov al, 0xAE
  out dx, al
  in al, dx
  cmp al, 0xAE
  jne .err
  add edx, 4
  mov al, 0x0F
  out dx, al
  xor eax, eax
  ret
  .err:
  xor eax, eax
  not eax
  ret

_read_com_port: ; COM INDEX
  xor eax, eax
  mov ecx, [esp + 4]
  cmp ecx, COM_COUNT
  jae .err
  mov edx, [COM_PORTS + ecx * 4]
  test edx, edx
  jz .err
  add edx, 5
  in al, dx
  and al, 1
  jz .err
  sub edx, 5
  in al, dx
  ret
  .err:
  not eax
  ret

_write_com_port: ; COM INDEX, CHAR(can be 5, 6, 7 or 8 bits)
  xor eax, eax
  mov ecx, [esp + 4]
  cmp ecx, COM_COUNT
  jae .err
  mov edx, [COM_PORTS + ecx * 4]
  test edx, edx
  jz .err
  add edx, 5
  mov ecx, 0x10
  .wait:
    in al, dx
    and al, 0x20
    jnz .good
    loop .wait
  .err:
  not eax
  ret
  .good:
  sub edx, 5
  mov al, [esp + 8]
  out dx, al
  xor eax, eax
  ret

section .bss

COM_PORTS: resd COM_COUNT

; COM_INPUT_TOP:  resd COM_COUNT
; COM_INPUT_BOT:  resd COM_COUNT

; COM_OUTPUT_TOP: resd COM_COUNT
; COM_OUTPUT_BOT: resd COM_COUNT

; COM_INPUT_BUFFER:   resb (COM_COUNT << COM_POWER)
; COM_OUTPUT_BUFFER:  resb (COM_COUNT << COM_POWER)

