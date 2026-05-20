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

enum input_type {
  INPUT_EVENT_TYPE_NONE = 0,
  INPUT_EVENT_TYPE_CHAR,
  INPUT_EVENT_TYPE_KEYBOARD,
  _INPUT_TYPE_MAX,
};

struct input_modifiers {
  bool shift;
  bool ctrl;
  bool alt;
  bool meta;
};

struct input_event {
  enum input_type type;
  union {
    char c;
    struct keyboard_event key;
  } e;
};

#define INPUT_EVENTS_RB_SIZE   256

struct input_event_rb {
  struct input_event  events[INPUT_EVENTS_RB_SIZE];
  unsigned int  writer_index;
  unsigned int  reader_index;
};

struct input {
  void*  tty;   // pointer to a tty
  struct input_event_rb   events_rb;
  struct input_modifiers  modifiers;
  input_keymap_t*    keymap;
};

//
// Functions
//

void input_init(struct input *input, input_keymap_t *keymap, void *tty);
void input_update(struct input *input, struct keyboard_event event);

void input_events_add(struct input *input, struct input_event event);
struct input_event input_events_get(struct input *input);

#endif // POVOS_KERNEL_INPUT_H
