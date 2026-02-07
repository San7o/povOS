// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_INPUT_H
#define POVOS_DRIVERS_INPUT_H

//
// Input driver
// ------------
//
// The input driver is an abstraction over keyboard events, providing
// more "high level" events. Here is a concrete example which shows
// why this is useful:
//
// The keyboard events
//
//   SHIFT pressed
//   A pressed
//   A released
//   SHIFT released
//
// Would generate an input event:
//
//   'A'
//
// Very useful. Input also handles escape sequences, like '\n'.
//

#include <drivers/input/keyboard.h>

typedef enum input_type {
  INPUT_EVENT_TYPE_NONE = 0,
  INPUT_EVENT_TYPE_CHAR,
  INPUT_EVENT_TYPE_KEYBOARD,
  _INPUT_TYPE_MAX,
} input_type_t;

typedef struct input_modifiers {
  bool shift;
  bool ctrl;
  bool alt;
} input_modifiers_t;

typedef struct input_event {
  input_type_t   type;
  union {
    char               c;
    keyboard_event_t   key;
  } e;
} input_event_t;

#define INPUT_EVENTS_FB_SIZE   256

typedef struct input_event_fb {
  input_event_t   events[INPUT_EVENTS_FB_SIZE];
  unsigned int    writer_index;
  unsigned int    reader_index;
} input_event_fb_t;

typedef struct input {
  input_event_fb_t    events_fb;
  input_modifiers_t   modifiers;
} input_t;

//
// Functions
//

// TODO

#endif // POVOS_DRIVERS_INPUT_H
