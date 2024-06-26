global _init_vga

global _get_screen_buffer
global _get_screen_pitch
global _get_screen_width
global _get_screen_height

section .text

_init_vga: ; SCREEN DATA PTR
  mov ecx, [esp + 4]
  mov eax, [ecx]
  mov [VGA_BUFFER], eax
  mov eax, [ecx + 4]
  mov [VGA_PITCH], eax
  mov eax, [ecx + 8]
  mov [VGA_WIDTH], eax
  mov eax, [ecx + 12]
  mov [VGA_HEIGHT], eax
  ret

_get_screen_buffer:
  mov eax, [VGA_BUFFER]
  ret

_get_screen_pitch:
  mov eax, [VGA_PITCH]
  ret

_get_screen_width:
  mov eax, [VGA_WIDTH]
  ret

_get_screen_height:
  mov eax, [VGA_HEIGHT]
  ret

section .bss

VGA_BUFFER: resd 1
VGA_PITCH:  resd 1
VGA_WIDTH:  resd 1
VGA_HEIGHT: resd 1

