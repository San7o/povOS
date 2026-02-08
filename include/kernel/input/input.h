// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_INPUT_H
#define POVOS_KERNEL_INPUT_H

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
#include <kernel/input/keymap.h>

//
// Types
//

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
  bool meta;
} input_modifiers_t;

typedef struct input_event {
  input_type_t   type;
  union {
    char               c;
    keyboard_event_t   key;
  } e;
} input_event_t;

#define INPUT_EVENTS_RB_SIZE   256

typedef struct input_event_rb {
  input_event_t   events[INPUT_EVENTS_RB_SIZE];
  unsigned int    writer_index;
  unsigned int    reader_index;
} input_event_rb_t;

typedef struct input {
  void*               tty;   // pointer to a tty
  input_event_rb_t    events_rb;
  input_modifiers_t   modifiers;
  input_keymap_t*     keymap;
} input_t;

//
// Functions
//

void input_init(input_t *input, input_keymap_t *keymap, void *tty);
void input_update(input_t *input, keyboard_event_t event);

void          input_events_add(input_t *input, input_event_t event);
input_event_t input_events_get(input_t *input);

#endif // POVOS_KERNEL_INPUT_H
