// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_KEYBOARD_H
#define POVOS_DRIVERS_KEYBOARD_H

#include <stdbool.h>

//
// Keyboard driver
// ===============
//
// The driver updates a bit array of active presses, which are read
// through serial communication and converted to keycodes through
// a lookup. The conversion is needed because each keyboard will
// send different codes depending on the keyboard layout, and we
// want to abstract this behaviour.
//

typedef enum keycode {
  // Reserved
  KEY_NONE        = 0x00,

  // Letters
  KEY_A           = 0x01,
  KEY_B           = 0x02,
  KEY_C           = 0x03,
  KEY_D           = 0x04,
  KEY_E           = 0x05,
  KEY_F           = 0x06,
  KEY_G           = 0x07,
  KEY_H           = 0x08,
  KEY_I           = 0x09,
  KEY_J           = 0x0A,
  KEY_K           = 0x0B,
  KEY_L           = 0x0C,
  KEY_M           = 0x0D,
  KEY_N           = 0x0E,
  KEY_O           = 0x0F,
  KEY_P           = 0x10,
  KEY_Q           = 0x11,
  KEY_R           = 0x12,
  KEY_S           = 0x13,
  KEY_T           = 0x14,
  KEY_U           = 0x15,
  KEY_V           = 0x16,
  KEY_W           = 0x17,
  KEY_X           = 0x18,
  KEY_Y           = 0x19,
  KEY_Z           = 0x1A,

  // Number row
  KEY_1           = 0x1B,
  KEY_2           = 0x1C,
  KEY_3           = 0x1D,
  KEY_4           = 0x1E,
  KEY_5           = 0x1F,
  KEY_6           = 0x20,
  KEY_7           = 0x21,
  KEY_8           = 0x22,
  KEY_9           = 0x23,
  KEY_0           = 0x24,

  // Symbols (US layout)
  KEY_MINUS       = 0x25,   // -
  KEY_EQUAL       = 0x26,   // =
  KEY_LBRACKET    = 0x27,   // [
  KEY_RBRACKET    = 0x28,   // ]
  KEY_BACKSLASH   = 0x29,   // `\`
  KEY_SEMICOLON   = 0x2A,   // ;
  KEY_APOSTROPHE  = 0x2B,   // '
  KEY_GRAVE       = 0x2C,   // `
  KEY_COMMA       = 0x2D,   // ,
  KEY_PERIOD      = 0x2E,   // .
  KEY_SLASH       = 0x2F,   // /
  KEY_QMARK       = KEY_SLASH,

  // Whitespace / control
  KEY_SPACE       = 0x30,
  KEY_TAB         = 0x31,
  KEY_ENTER       = 0x32,
  KEY_BACKSPACE   = 0x33,
  KEY_ESCAPE      = 0x34,

  // Modifiers
  KEY_LSHIFT      = 0x35,
  KEY_RSHIFT      = 0x36,
  KEY_LCTRL       = 0x37,
  KEY_RCTRL       = 0x38,
  KEY_LALT        = 0x39,
  KEY_RALT        = 0x3A,
  KEY_LMETA       = 0x3B,   // Win / Cmd
  KEY_RMETA       = 0x3C,

  // Navigation
  KEY_INSERT      = 0x3D,
  KEY_DELETE      = 0x3E,
  KEY_HOME        = 0x3F,
  KEY_END         = 0x40,
  KEY_PAGEUP      = 0x41,
  KEY_PAGEDOWN    = 0x42,

  // Arrows
  KEY_UP          = 0x43,
  KEY_DOWN        = 0x44,
  KEY_LEFT        = 0x45,
  KEY_RIGHT       = 0x46,

  // Function keys
  KEY_F1          = 0x47,
  KEY_F2          = 0x48,
  KEY_F3          = 0x49,
  KEY_F4          = 0x4A,
  KEY_F5          = 0x4B,
  KEY_F6          = 0x4C,
  KEY_F7          = 0x4D,
  KEY_F8          = 0x4E,
  KEY_F9          = 0x4F,
  KEY_F10         = 0x50,
  KEY_F11         = 0x51,
  KEY_F12         = 0x52,

  // Numeric keypad
  KEY_KP_0        = 0x53,
  KEY_KP_1        = 0x54,
  KEY_KP_2        = 0x55,
  KEY_KP_3        = 0x56,
  KEY_KP_4        = 0x57,
  KEY_KP_5        = 0x58,
  KEY_KP_6        = 0x59,
  KEY_KP_7        = 0x5A,
  KEY_KP_8        = 0x5B,
  KEY_KP_9        = 0x5C,

  KEY_KP_PLUS     = 0x5D,
  KEY_KP_MINUS    = 0x5E,
  KEY_KP_MULT     = 0x5F,
  KEY_KP_DIV      = 0x60,
  KEY_KP_ENTER    = 0x61,
  KEY_KP_DOT      = 0x62,

  __KEY_MAX,
} keycode_t;

typedef unsigned char keycode_raw_t;

struct keyboard {
  keycode_t state[__KEY_MAX];
};
typedef struct keyboard keyboard_t;

extern bool keyboard_active[__KEY_MAX];

// This should be called to update the global keyboard_active state
// when a PS/2 scancode is received.
void keyboard_register_ps2_scancode(keyboard_t *keyboard,
                                    unsigned int ps2_scancode);

#endif // POVOS_DRIVERS_KEYBOARD_H
