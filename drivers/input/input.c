// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/input/input.h>   // implements
#include <libk/string.h>

void input_init(input_t *input, input_keymap_t *keymap)
{
  if (!input) return;
  memset((void*)input, 0, sizeof(input_t));
  input->keymap = keymap;
  return;
}

void input_events_add(input_t *input, input_event_t event)
{
  if (!input) return;

  input->events_rb.events[input->events_rb.writer_index] = event;
  input->events_rb.writer_index =
    (input->events_rb.writer_index + 1) % INPUT_EVENTS_RB_SIZE;
  
  return;
}

input_event_t input_events_get(input_t *input)
{
  if (!input) goto exit;

  if (input->events_rb.reader_index ==
      input->events_rb.writer_index) goto exit;

  input_event_t event =
    input->events_rb.events[input->events_rb.reader_index];
  input->events_rb.reader_index =
    (input->events_rb.reader_index + 1) % INPUT_EVENTS_RB_SIZE;

  return event;
  
 exit:
  return (input_event_t) {
    .type = INPUT_EVENT_TYPE_NONE,
  };
}

void input_update(input_t *input, keyboard_event_t event)
{
  if (!input) return;

  switch (event.key)
  {
  case KEY_NONE:
    return;
  case KEY_LEFT_SHIFT:
  case KEY_RIGHT_SHIFT:
    input->modifiers.shift = event.pressed;
    return;
  case KEY_LEFT_CTRL:
  case KEY_RIGHT_CTRL:
    input->modifiers.ctrl = event.pressed;
    return;
  case KEY_LEFT_ALT:
  case KEY_RIGHT_ALT:
    input->modifiers.alt = event.pressed;
    return;
  case KEY_LEFT_META:
  case KEY_RIGHT_META:
    input->modifiers.meta = event.pressed;
    return;
  default:
    break;
  }

  if (!event.pressed) return;

  char c = (*input->keymap)[event.key][input->modifiers.shift];

  if (c == 0)  // key not present in layout
  {
    input_events_add(input, (input_event_t) {
        .type = INPUT_EVENT_TYPE_KEYBOARD,
        .e.key = event,
      });
  }
  else
  {
    input_events_add(input, (input_event_t) {
        .type = INPUT_EVENT_TYPE_CHAR,
        .e.c = c,
      });
  }

  return;
}
