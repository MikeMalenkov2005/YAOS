#ifndef KEYS_H
#define KEYS_H

#define KEY_REL_BIT 0x100
#define KEY_ALT_BIT 0x80

#define KEY_BACKSPACE     0x08 /*\b */
#define KEY_TAB           0x09 /*\t */
#define KEY_ENTER         0x0D /*\r */
#define KEY_ESCAPE        0x1B /*\e */
#define KEY_SPACE         0x20 /*   */
#define KEY_APOSTROPHE    0x27 /* ' */
#define KEY_COMMA         0x2C /* , */
#define KEY_MINUS         0x2D /* - */
#define KEY_PERIOD        0x2E /* . */
#define KEY_SLASH         0x2F /* / */
#define KEY_SEMICOLON     0x3B /* ; */
#define KEY_EQUAL         0x3D /* = */
#define KEY_LEFT_BRACKET  0x5B /* [ */
#define KEY_BACKSLASH     0x5C /* \ */
#define KEY_RIGHT_BRACKET 0x5D /* ] */
#define KEY_GRAVE_ACCENT  0x60 /* ` */

#define KEY_F1  0x61
#define KEY_F2  0x62
#define KEY_F3  0x63
#define KEY_F4  0x64
#define KEY_F5  0x65
#define KEY_F6  0x66
#define KEY_F7  0x67
#define KEY_F8  0x68
#define KEY_F9  0x69
#define KEY_F10 0x6A
#define KEY_F11 0x6B
#define KEY_F12 0x6C
#define KEY_F13 0x6D
#define KEY_F14 0x6E
#define KEY_F15 0x6F
#define KEY_F16 0x70
#define KEY_F17 0x71
#define KEY_F18 0x72
#define KEY_F19 0x73
#define KEY_F20 0x74
#define KEY_F21 0x75
#define KEY_F22 0x76
#define KEY_F23 0x77
#define KEY_F24 0x78
#define KEY_F25 0x79
#define KEY_F26 0x7A

#define KEY_LEFT_ALT      0x21
#define KEY_LEFT_GUI      0x25
#define KEY_LEFT_SHIFT    0x5E
#define KEY_LEFT_CONTROL  0x5F

#define KEY_RIGHT_ALT     (KEY_ALT_BIT | KEY_LEFT_ALT)
#define KEY_RIGHT_GUI     (KEY_ALT_BIT | KEY_LEFT_GUI)
#define KEY_RIGHT_SHIFT   (KEY_ALT_BIT | KEY_LEFT_SHIFT)
#define KEY_RIGHT_CONTROL (KEY_ALT_BIT | KEY_LEFT_CONTROL)

#define KEY_DELETE  0x7F
#define KEY_INSERT  0x3A
#define KEY_HOME    0x28
#define KEY_END     0x29
#define KEY_PRIOR   0x3C
#define KEY_NEXT    0x3E

#define KEY_LEFT  0x7B
#define KEY_UP    0x7C
#define KEY_RIGHT 0x7D
#define KEY_DOWN  0x7E

#define KEY_KP_0  (KEY_ALT_BIT | KEY_INSERT)
#define KEY_KP_1  (KEY_ALT_BIT | KEY_END)
#define KEY_KP_2  (KEY_ALT_BIT | KEY_DOWN)
#define KEY_KP_3  (KEY_ALT_BIT | KEY_NEXT)
#define KEY_KP_4  (KEY_ALT_BIT | KEY_LEFT)
#define KEY_KP_5  (KEY_ALT_BIT | KEY_SPACE)
#define KEY_KP_6  (KEY_ALT_BIT | KEY_RIGHT)
#define KEY_KP_7  (KEY_ALT_BIT | KEY_HOME)
#define KEY_KP_8  (KEY_ALT_BIT | KEY_UP)
#define KEY_KP_9  (KEY_ALT_BIT | KEY_PRIOR)

#define KEY_KP_DECIMAL  (KEY_ALT_BIT | KEY_DELETE)
#define KEY_KP_DIVIDE   (KEY_ALT_BIT | KEY_SLASH)
#define KEY_KP_MULTIPLY (KEY_ALT_BIT | 0x2A)
#define KEY_KP_SUBTRACT (KEY_ALT_BIT | KEY_MINUS)
#define KEY_KP_ADD      (KEY_ALT_BIT | 0x2B)
#define KEY_KP_ENTER    (KEY_ALT_BIT | KEY_ENTER)

#define KEY_CAPSLOCK  0x22
#define KEY_NUMLOCK   0x23
#define KEY_SCROLL    0x24
#define KEY_APPS      0x26

#define KEY_PRINT_SCREEN  0x19
#define KEY_PAUSE         0x16

#endif

