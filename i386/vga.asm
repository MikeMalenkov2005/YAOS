global _init_vga

global _get_screen_buffer
global _get_screen_pitch
global _get_screen_width
global _get_screen_height
global _get_screen_depth
global _get_screen_mode

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
  mov eax, [ecx + 16]
  mov [VGA_DEPTH], eax
  mov eax, [ecx + 20]
  mov [VESA_MODE], eax
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

_get_screen_depth:
  mov eax, [VGA_DEPTH]
  ret

_get_screen_mode:
  mov eax, [VESA_MODE]
  ret

section .bss

VGA_BUFFER: resd 1
VGA_PITCH:  resd 1
VGA_WIDTH:  resd 1
VGA_HEIGHT: resd 1
VGA_DEPTH:  resd 1
VESA_MODE:  resd 1

