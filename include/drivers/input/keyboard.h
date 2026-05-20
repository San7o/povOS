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

#include <kernel/input/keycodes.h>

typedef unsigned char keycode_raw_t;

// Maps from keycode to char*
extern const char* keycode_str[_KEY_MAX];

enum keyboard_type {
  KEYBOARD_NONE      = 0,
  KEYBOARD_TYPE_PS2_SET1,
  KEYBOARD_TYPE_PS2_SET2,
  KEYBOARD_TYPE_PS2_SET3,
  __KEYBOARD_TYPE,
};

//
// The event
// ---------
//
struct keyboard_event {
  bool  pressed;   // false = released
  enum keycode  key;
};
#define KEYBOARD_EVENT_MAKE(_pressed, _key) \
  (struct keyboard_event) { .pressed = _pressed, .key = _key }

#define KEYBOARD_EVENTS_RB_SIZE   256

// A ring buffer
struct keyboard_events_rb {
  struct keyboard_event events[KEYBOARD_EVENTS_RB_SIZE];
  unsigned int     writer_index;
  unsigned int     reader_index;
};

//
// The keyboard
// ------------
//
struct keyboard {
  // Public
  enum keyboard_type         type;
  bool                         state[_KEY_MAX];
  struct keyboard_events_rb    events_rb;   // ring buffer
  void*                        input;       // pointer to an input device
  
  // Private
  unsigned int _internal;   // For internal use, do not modify
};

//
// Functions
//

// Use this function when creating a new keyboard
void keyboard_init(struct keyboard *keyboard, enum keyboard_type type,
                   void* input_dev);
// Register a new event
void keyboard_update(struct keyboard *keyboard, struct keyboard_event event);

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
void keyboard_set_active(struct keyboard *keyboard);
// Set the currently active keyboard (may be NULL if none was set)
struct keyboard *keyboard_get_active(void);

//
// Keyboard state query
// --------------------
//
// Register a new event to the ring buffer
// This is called in `keyboard_update`
void keyboard_events_add(struct keyboard *keyboard,
                         struct keyboard_event event);
// Returns event.key = KEY_NONE when no events are found
struct keyboard_event keyboard_events_get(struct keyboard *keyboard);
bool keyboard_is_key_pressed(struct keyboard *keyboard, enum keycode key);

//
// Conversion functions
// --------------------
//
struct keyboard_event keyboard_event_from_scancode(struct keyboard *keyboard,
                                                   u8_t scancode);

#endif // POVOS_DRIVERS_KEYBOARD_H
