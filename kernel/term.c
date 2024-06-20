#include "term.h"
#include "serial.h"
#include "keys.h"
#include <stdint.h>

static int color = 7;

void kclear() {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    _set_cursor_pos(0);
    int size = _get_screen_width() * _get_screen_height();
    for (int i = 0; i < size; ++i) buffer[i] = color << 8;
  }
}

void kprint(const char *string) {
  cchar_t *buffer = _get_screen_buffer();
  if (buffer) {
    int pos = _get_cursor_pos();
    int width = _get_screen_width();
    int size = width * _get_screen_height();
    while (*string) {
      unsigned char c = *(string++);
      switch (c) {
        case '\b':
          if (!pos) break;
          buffer[--pos] = color << 8;
          while (!(buffer[--pos] & 0xFF) && ((pos + 1) % width));
          ++pos;
          break;
        case '\t':
          pos = (pos & ~7) + 8;
          buffer[pos - 1] = (color << 8) | ' ';
          break;
        case '\n':
          pos += width;
          break;
        case '\r':
          pos -= pos % width;
          break;
        default:
          buffer[pos++] = c | (color << 8);
          break;
      }
      if (pos >= size) {
        pos -= width;
        for (int i = 0; i < pos; ++i) buffer[i] = buffer[i + width];
        for (int i = 0; i < width; ++i) buffer[pos + i] = color << 8;
      }
    }
    buffer[pos] = color << 8;
    _set_cursor_pos(pos);
  }
}

static unsigned char ps2_key(int c) {
  switch (c) {
    case 0x1C: return 'A';
    case 0x32: return 'B';
    case 0x21: return 'C';
    case 0x23: return 'D';
    case 0x24: return 'E';
    case 0x2B: return 'F';
    case 0x34: return 'G';
    case 0x33: return 'H';
    case 0x43: return 'I';
    case 0x3B: return 'J';
    case 0x42: return 'K';
    case 0x4B: return 'L';
    case 0x3A: return 'M';
    case 0x31: return 'N';
    case 0x44: return 'O';
    case 0x4D: return 'P';
    case 0x15: return 'Q';
    case 0x2D: return 'R';
    case 0x1B: return 'S';
    case 0x2C: return 'T';
    case 0x3C: return 'U';
    case 0x2A: return 'V';
    case 0x1D: return 'W';
    case 0x22: return 'X';
    case 0x35: return 'Y';
    case 0x1A: return 'Z';
    case 0x45: return '0';
    case 0x16: return '1';
    case 0x1E: return '2';
    case 0x26: return '3';
    case 0x25: return '4';
    case 0x2E: return '5';
    case 0x36: return '6';
    case 0x3D: return '7';
    case 0x3E: return '8';
    case 0x46: return '9';
    case 0x0E: return KEY_GRAVE_ACCENT;
    case 0x4E: return KEY_MINUS;
    case 0x55: return KEY_EQUAL;
    case 0x5D: return KEY_BACKSLASH;
    case 0x66: return KEY_BACKSPACE;
    case 0x29: return KEY_SPACE;
    case 0x0D: return KEY_TAB;
    case 0x58: return KEY_CAPSLOCK;
    case 0x12: return KEY_LEFT_SHIFT;
    case 0x14: return KEY_LEFT_CONTROL;
    case 0xE01F: return KEY_LEFT_GUI;
    case 0x11: return KEY_LEFT_ALT;
    case 0x59: return KEY_RIGHT_SHIFT;
    case 0xE014: return KEY_RIGHT_CONTROL;
    case 0xE027: return KEY_RIGHT_GUI;
    case 0xE011: return KEY_RIGHT_ALT;
    case 0xE02F: return KEY_APPS;
    case 0x5A: return KEY_ENTER;
    case 0x76: return KEY_ESCAPE;
    case 0x05: return KEY_F1;
    case 0x06: return KEY_F2;
    case 0x04: return KEY_F3;
    case 0x0C: return KEY_F4;
    case 0x03: return KEY_F5;
    case 0x0B: return KEY_F6;
    case 0x83: return KEY_F7;
    case 0x0A: return KEY_F8;
    case 0x01: return KEY_F9;
    case 0x09: return KEY_F10;
    case 0x78: return KEY_F11;
    case 0x07: return KEY_F12;
    case 0x7E: return KEY_SCROLL;
    case 0x54: return KEY_LEFT_BRACKET;
    case 0x5B: return KEY_RIGHT_BRACKET;
    case 0x4C: return KEY_SEMICOLON;
    case 0x52: return KEY_APOSTROPHE;
    case 0x41: return KEY_COMMA;
    case 0x49: return KEY_PERIOD;
    case 0x4A: return KEY_SLASH;
    case 0xE070: return KEY_INSERT;
    case 0xE06C: return KEY_HOME;
    case 0xE07D: return KEY_PRIOR;
    case 0xE071: return KEY_DELETE;
    case 0xE069: return KEY_END;
    case 0xE07A: return KEY_NEXT;
    case 0xE075: return KEY_UP;
    case 0xE06B: return KEY_LEFT;
    case 0xE072: return KEY_DOWN;
    case 0xE074: return KEY_RIGHT;
    case 0x77: return KEY_NUMLOCK;
    case 0xE04A: return KEY_KP_DIVIDE;
    case 0x7C: return KEY_KP_MULTIPLY;
    case 0x7B: return KEY_KP_SUBTRACT;
    case 0x79: return KEY_KP_ADD;
    case 0xE05A: return KEY_KP_ENTER;
    case 0x71: return KEY_KP_DECIMAL;
    case 0x70: return KEY_KP_0;
    case 0x69: return KEY_KP_1;
    case 0x72: return KEY_KP_2;
    case 0x7A: return KEY_KP_3;
    case 0x6B: return KEY_KP_4;
    case 0x73: return KEY_KP_5;
    case 0x74: return KEY_KP_6;
    case 0x6C: return KEY_KP_7;
    case 0x75: return KEY_KP_8;
    case 0x7D: return KEY_KP_9;
    default: return 0;
  }
}

void print_byte(int byte) {
  char string[5];
  int i = sizeof(string);
  string[--i] = 0;
  string[--i] = ' ';
  string[--i] = byte & 15;
  string[--i] = (byte >> 4) & 15;
  string[--i] = ' ';
  string[1] += string[1] < 10 ? '0' : 'A' - 10;
  string[2] += string[2] < 10 ? '0' : 'A' - 10;
  kprint(string);
}

int kscan() {
  int extended = 0;
  int released = 0;
  int c = _read_first_ps2();
  if (c < 0) return 0;
  if (c == 0xE1) {
    int code_a = 0;
    for (int i = 0; i < 4; ++i) {
      for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
      code_a = (code_a << 8) | c;
    }
    int code_b = 0;
    for (int i = 0; i < 3; ++i) {
      for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
      code_b = (code_b << 8) | c;
    }
    return code_a == 0x1477E1F0 && code_b == 0x14F077 ? KEY_PAUSE : 0;
  }
  if (c == 0xE0) {
    extended = 1;
    for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
  }
  if (c == 0xF0) {
    released = 1;
    for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
    if (c == 0x7C) {
      int code = 0;
      for (int i = 0; i < 3; ++i) {
        for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
        code = (code << 8) | c;
      }
      return code == 0xE0F012 ? KEY_PRINT_SCREEN | KEY_REL_BIT : -1;
    }
  }
  else if (extended && c == 0x12) {
    int code = 0;
    for (int i = 0; i < 2; ++i) {
      for (c = _read_first_ps2(); c < 0; c = _read_first_ps2());
      code = (code << 8) | c;
    }
    return code == 0xE07C ? KEY_PRINT_SCREEN : -1;
  }
  return ps2_key((extended ? 0xE000 : 0) | c) | (released ? KEY_REL_BIT : 0);
}

