global _get_screen_buffer
global _get_screen_width
global _get_screen_height

section .text

_get_screen_buffer:
  mov eax, 0xA0000
  ret

_get_screen_width:
  mov eax, 320
  ret

_get_screen_height:
  mov eax, 200
  ret

