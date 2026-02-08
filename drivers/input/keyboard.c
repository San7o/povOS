// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/input/keyboard.h>   // implements
#include <drivers/input/input.h>
#include <libk/string.h>

#include "keyboard_scanmap.h"

// Currently active keyboard
static keyboard_t *keyboard_active = NULL;

void keyboard_init(keyboard_t *keyboard, keyboard_type_t type,
                   void* input)
{
  if (!keyboard) return;

  keyboard->type      = type;
  keyboard->input     = input;
  keyboard->_internal = 0;
  memset(keyboard->state, 0, _KEY_MAX * sizeof(keycode_t));
  keyboard->events_rb.writer_index = 0;
  keyboard->events_rb.reader_index = 0;
  memset(&keyboard->events_rb.events, 0,
         KEYBOARD_EVENTS_RB_SIZE * sizeof(keyboard_event_t));
  
  return;
}

void keyboard_update(keyboard_t *keyboard, keyboard_event_t event)
{
  if (!keyboard || event.key == KEY_NONE
      || event.key < 0 || event.key >= _KEY_MAX) return;

  keyboard->state[event.key] = event.pressed;

  // For future me: careful with future deadlock here
  keyboard_events_add(keyboard, event);
  
  input_update((input_t*) keyboard->input, event);

  return;
}

// Implements keyboard_event_from_scancode for keyboards of type
// KEYBOARD_TYPE_PS2_SET1
static keyboard_event_t
keyboard_event_from_scancode_ps2_set1(keyboard_t *keyboard,
                                      u8_t        scancode)
{
  if (!keyboard)
    goto exit;
  
  if (keyboard->_internal == 1)
  {
    keyboard->_internal = 0;
    return keyboard_scanmap_ps2_set1_escaped[scancode];
  }
  else
  {
    if (scancode == 0xE0)
    {
      keyboard->_internal = 1;
      goto exit;
    }
    
    return keyboard_scanmap_ps2_set1[scancode];
  }

 exit:
  return (keyboard_event_t) {
    .pressed = false,
    .key     = KEY_NONE,
  };
}

keyboard_event_t keyboard_event_from_scancode(keyboard_t *keyboard,
                                              u8_t        scancode)
{
  if (!keyboard)
    goto exit;

  switch (keyboard->type)
  {
  case KEYBOARD_TYPE_PS2_SET1:
    return keyboard_event_from_scancode_ps2_set1(keyboard, scancode);
    break;
  case KEYBOARD_TYPE_PS2_SET2:
  case KEYBOARD_TYPE_PS2_SET3:
  default:
    // Do nothing
    break;
  }

 exit:
  return (keyboard_event_t) {
    .pressed = false,
    .key     = KEY_NONE,
  };
}

void keyboard_set_active(keyboard_t *keyboard)
{
  keyboard_active = keyboard;
  return;
}

keyboard_t *keyboard_get_active(void)
{
  return keyboard_active;
}

void keyboard_events_add(keyboard_t *keyboard,
                         keyboard_event_t event)
{
  if (!keyboard) return;

  keyboard->events_rb.events[keyboard->events_rb.writer_index] = event;
  keyboard->events_rb.writer_index =
    (keyboard->events_rb.writer_index + 1) % KEYBOARD_EVENTS_RB_SIZE;
  
  return;
}

keyboard_event_t keyboard_events_get(keyboard_t *keyboard)
{
  if (!keyboard) goto exit;

  if (keyboard->events_rb.reader_index ==
      keyboard->events_rb.writer_index) goto exit;

  keyboard_event_t event =
    keyboard->events_rb.events[keyboard->events_rb.reader_index];
  keyboard->events_rb.reader_index =
    (keyboard->events_rb.reader_index + 1) % KEYBOARD_EVENTS_RB_SIZE;

  return event;
  
 exit:
  return (keyboard_event_t) {
    .pressed = false,
    .key     = KEY_NONE,
  };
} 

bool keyboard_is_key_pressed(keyboard_t *keyboard, keycode_t key)
{
  if (!keyboard) return false;
  return keyboard->state[key];
}
