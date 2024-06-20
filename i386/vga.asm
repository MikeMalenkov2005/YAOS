global _enable_cursor
global _disable_cursor
global _get_cursor_pos
global _set_cursor_pos
global _get_screen_buffer
global _get_screen_width
global _get_screen_height

section .text

_enable_cursor:
  mov edx, 0x3D4
  mov al, 0x0A
  out dx, al
  xor dl, 1
  in al, dx
  and al, 0xDF
  out dx, al
  ret

_disable_curosr:
  mov edx, 0x3D4
  mov al, 0x0A
  out dx, al
  xor dl, 1
  in al, dx
  or al, 0x20
  out dx, al
  ret

_get_cursor_pos:
  mov edx, 0x3D4
  xor eax, eax
  mov al, 0x0E
  out dx, al
  xor dl, 1
  in al, dx
  mov ah, al
  xor dl, 1
  mov al, 0x0F
  out dx, al
  xor dl, 1
  in al, dx
  ret

_set_cursor_pos: ; POSITION
  mov edx, 0x3D4
  mov al, 0x0F
  out dx, al
  xor dl, 1
  mov eax, [esp + 4]
  out dx, al
  xor dl, 1
  mov al, 0x0E
  out dx, al
  xor dl, 1
  mov al, ah
  out dx, al
  ret

_get_screen_buffer:
  mov eax, 0xB8000
  ret

_get_screen_width:
  mov eax, [0x44A]
  and eax, 0xFFFF
  ret

_get_screen_height:
  mov eax, 25
  ret

