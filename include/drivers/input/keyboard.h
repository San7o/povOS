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
// The keyboard keeps an array of active presses and a ring buffer
// of the latest events.
//

#include <libk/stdbool.h>
#include <libk/stddef.h>

#include <drivers/input/keycodes.h>

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

#define KEYBOARD_EVENTS_RB_SIZE   256

// A ring buffer
typedef struct keyboard_events_rb {
  keyboard_event_t events[KEYBOARD_EVENTS_RB_SIZE];
  unsigned int     writer_index;
  unsigned int     reader_index;
} keyboard_events_rb_t;

//
// The keyboard
// ------------
//
typedef struct keyboard {
  // Public
  keyboard_type_t         type;
  bool                    state[_KEY_MAX];
  keyboard_events_rb_t    events_rb;   // ring buffer
  void*                   input;    // pointer to an input_t device
  
  // Private
  unsigned int _internal;   // For internal use, do not modify
} keyboard_t;

//
// Functions
//

// Use this function when creating a new keyboard
void keyboard_init(keyboard_t *keyboard, keyboard_type_t type,
                   void* input_dev);
// Register a new event
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
// Keyboard state query
// --------------------
//
// Register a new event to the ring buffer
// This is called in `keyboard_update`
void keyboard_events_add(keyboard_t *keyboard,
                         keyboard_event_t event);
// Returns event.key = KEY_NONE when no events are found
keyboard_event_t keyboard_events_get(keyboard_t *keyboard);
bool keyboard_is_key_pressed(keyboard_t *keyboard, keycode_t key);

//
// Conversion functions
// --------------------
//
keyboard_event_t keyboard_event_from_scancode(keyboard_t *keyboard,
                                              u8_t        scancode);

#endif // POVOS_DRIVERS_KEYBOARD_H
