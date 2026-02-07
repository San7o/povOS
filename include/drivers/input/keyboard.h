// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_KEYBOARD_H
#define POVOS_DRIVERS_KEYBOARD_H

//
// Keyboard driver
// ===============
//
// The driver exposes three important types: the `keycode`, the
// `keyboard` and the `keyboard_event`. When a scancode is received,
// the scancode is converted a keycode, and generates an event which
// is used to update the keyboard. While scancodes depend on the type
// of keyboard, the keycodes are a general abstraction for all
// keyboards.
//
// The keyboard keeps an array of active presses.
//

#include <libk/stdbool.h>
#include <libk/stddef.h>

//
// Keycode
// -------
//
// The keycode type is a general abstraction over the scancodes.
//
typedef enum keycode {
    // Reserved
    KEY_NONE            = 0x00,

    // Letters
    KEY_A               = 0x01,
    KEY_B               = 0x02,
    KEY_C               = 0x03,
    KEY_D               = 0x04,
    KEY_E               = 0x05,
    KEY_F               = 0x06,
    KEY_G               = 0x07,
    KEY_H               = 0x08,
    KEY_I               = 0x09,
    KEY_J               = 0x0A,
    KEY_K               = 0x0B,
    KEY_L               = 0x0C,
    KEY_M               = 0x0D,
    KEY_N               = 0x0E,
    KEY_O               = 0x0F,
    KEY_P               = 0x10,
    KEY_Q               = 0x11,
    KEY_R               = 0x12,
    KEY_S               = 0x13,
    KEY_T               = 0x14,
    KEY_U               = 0x15,
    KEY_V               = 0x16,
    KEY_W               = 0x17,
    KEY_X               = 0x18,
    KEY_Y               = 0x19,
    KEY_Z               = 0x1A,

    // Number row
    KEY_1               = 0x1B,
    KEY_2               = 0x1C,
    KEY_3               = 0x1D,
    KEY_4               = 0x1E,
    KEY_5               = 0x1F,
    KEY_6               = 0x20,
    KEY_7               = 0x21,
    KEY_8               = 0x22,
    KEY_9               = 0x23,
    KEY_0               = 0x24,

    // Symbols (US layout)
    KEY_MINUS           = 0x25,   // -
    KEY_EQUAL           = 0x26,   // =
    KEY_OPEN_BRACKET    = 0x27,   // [
    KEY_CLOSE_BRACKET   = 0x28,   // ]
    KEY_BACKSLASH       = 0x29,   // `\` 
    KEY_SEMICOLON       = 0x2A,   // ;
    KEY_APOSTROPHE      = 0x2B,   // '
    KEY_GRAVE           = 0x2C,   // `
    KEY_COMMA           = 0x2D,   // ,
    KEY_PERIOD          = 0x2E,   // .
    KEY_SLASH           = 0x2F,   // /
    KEY_QMARK           = KEY_SLASH,

    // Whitespace / control
    KEY_SPACE           = 0x30,
    KEY_TAB             = 0x31,
    KEY_ENTER           = 0x32,
    KEY_BACKSPACE       = 0x33,
    KEY_ESCAPE          = 0x34,

    // Modifiers
    KEY_LEFT_SHIFT      = 0x35,
    KEY_RIGHT_SHIFT     = 0x36,
    KEY_LEFT_CTRL       = 0x37,
    KEY_RIGHT_CTRL      = 0x38,
    KEY_LEFT_ALT        = 0x39,
    KEY_RIGHT_ALT       = 0x3A,
    KEY_LEFT_META       = 0x3B,
    KEY_RIGHT_META      = 0x3C,

    // Navigation
    KEY_INSERT          = 0x3D,
    KEY_DELETE          = 0x3E,
    KEY_HOME            = 0x3F,
    KEY_END             = 0x40,
    KEY_PAGE_UP         = 0x41,
    KEY_PAGE_DOWN       = 0x42,

    // Arrows
    KEY_UP_ARROW        = 0x43,
    KEY_DOWN_ARROW      = 0x44,
    KEY_LEFT_ARROW      = 0x45,
    KEY_RIGHT_ARROW     = 0x46,

    // Function keys
    KEY_F1              = 0x47,
    KEY_F2              = 0x48,
    KEY_F3              = 0x49,
    KEY_F4              = 0x4A,
    KEY_F5              = 0x4B,
    KEY_F6              = 0x4C,
    KEY_F7              = 0x4D,
    KEY_F8              = 0x4E,
    KEY_F9              = 0x4F,
    KEY_F10             = 0x50,
    KEY_F11             = 0x51,
    KEY_F12             = 0x52,

    // Numeric keypad
    KEY_KP_0            = 0x53,
    KEY_KP_1            = 0x54,
    KEY_KP_2            = 0x55,
    KEY_KP_3            = 0x56,
    KEY_KP_4            = 0x57,
    KEY_KP_5            = 0x58,
    KEY_KP_6            = 0x59,
    KEY_KP_7            = 0x5A,
    KEY_KP_8            = 0x5B,
    KEY_KP_9            = 0x5C,
    KEY_KP_PLUS         = 0x5D,
    KEY_KP_MINUS        = 0x5E,
    KEY_KP_MULT         = 0x5F,
    KEY_KP_DIV          = 0x60,
    KEY_KP_ENTER        = 0x61,
    KEY_KP_DOT          = 0x62,
    KEY_KP_SLASH        = 0x63, // Mapping 0x35

    // Locks
    KEY_CAPS_LOCK       = 0x64,
    KEY_NUM_LOCK        = 0x65,
    KEY_SCROLL_LOCK     = 0x66,

    // Multimedia / System (Extended)
    KEY_PRINT_SCREEN    = 0x67,
    KEY_PAUSE           = 0x68,
    KEY_APPLICATION     = 0x69,
    
    // Multimedia Controls
    KEY_MUTE            = 0x6A,
    KEY_VOLUME_UP       = 0x6B,
    KEY_VOLUME_DOWN     = 0x6C,
    KEY_PLAY_PAUSE      = 0x6D,
    KEY_STOP            = 0x6E,
    KEY_NEXT_TRACK      = 0x6F,
    KEY_PREV_TRACK      = 0x70,
    
    // Application Launchers
    KEY_CALCULATOR      = 0x71,
    KEY_MAIL            = 0x72,
    KEY_BROWSER_HOME    = 0x73,
    KEY_MEDIA_SELECT    = 0x74,
    KEY_LOCAL_BROWSER   = 0x75,

    // Browser/AC Controls
    KEY_AC_SEARCH       = 0x76,
    KEY_AC_BOOKMARKS    = 0x77,
    KEY_AC_REFRESH      = 0x78,
    KEY_AC_STOP         = 0x79,
    KEY_AC_FORWARD      = 0x7A,
    KEY_AC_BACK         = 0x7B,

    // System Power
    KEY_SYSTEM_POWER    = 0x7C,
    KEY_SYSTEM_SLEEP    = 0x7D,
    KEY_SYSTEM_WAKE     = 0x7E,

    _KEY_MAX,
} keycode_t;

typedef unsigned char keycode_raw_t;

// Maps from keycode to char*
extern const char* keycode_str[_KEY_MAX];

typedef enum keyboard_type {
  KEYBOARD_NONE      = 0,
  KEYBOARD_TYPE_PS2_SET1,
  KEYBOARD_TYPE_PS2_SET2,
  KEYBOARD_TYPE_PS2_SET3,
  __KEYBOARD_TYPE,
} keyboard_type_t;

//
// The event
// ---------
//
typedef struct keyboard_event {
  bool        pressed;   // false = released
  keycode_t   key;
} keyboard_event_t;

//
// The keyboard
// ------------
//
typedef struct keyboard {
  // Public
  keyboard_type_t    type;
  bool               state[_KEY_MAX];
  keyboard_event_t   events_rb[256];   // ring buffer

  // Private
  unsigned int _internal;   // For internal use, do not modify
} keyboard_t;

//
// Functions
//

void keyboard_init(keyboard_t *keyboard, keyboard_type_t type);
void keyboard_update(keyboard_t *keyboard, keyboard_event_t event);

//
// Active keyboard
// ---------------
//
// At any point in time, there can be at most one currently active
// keyboard. The interrupts handlers will use this keyboard to
// register new events. Use the following functions to interact with
// the active keyboard:
//
// Get the currently active keyboard
void keyboard_set_active(keyboard_t *keyboard);
// Set the currently active keyboard (may be NULL if none was set)
keyboard_t *keyboard_get_active(void);

//
// Conversion functions
// --------------------
//
keyboard_event_t keyboard_event_from_scancode(keyboard_t *keyboard,
                                              u8_t        scancode);
const char* keyboard_string_from_keycode(keycode_t key);

#endif // POVOS_DRIVERS_KEYBOARD_H
