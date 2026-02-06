// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/input/keyboard.h>

// TODO: reference minix codes
typedef enum ps2_keycode_1 {
  PS2_KEY1_ESCAPE_PRESSED = 0x01,
  PS2_KEY1_1_PRESSED      = 0x02,
  PS2_KEY1_2_PRESSED      = 0x03,
  PS2_KEY1_3_PRESSED      = 0x04,
  PS2_KEY1_4_PRESSED      = 0x05,
  PS2_KEY1_5_PRESSED      = 0x06,
  PS2_KEY1_6_PRESSED      = 0x07,
  PS2_KEY1_7_PRESSED      = 0x08,
  PS2_KEY1_8_PRESSED      = 0x09,
  PS2_KEY1_9_PRESSED      = 0x0A,
  PS2_KEY1_0_PRESSED      = 0x0B,
} ps2_keycode_1_t;

// Implements keyboard_register_ps2_scancode for keyboards of
// type KEYBOARD_TYPE_PS2_1 which use the first scancode set
void keyboard_register_ps2_scancode_1(keyboard_dev_t *keyboard,
                                      keyboard_event_t event)
{
  if (!keyboard) return;
  
  (void) keyboard;
  (void) event;

  // TODO
  
  return;
}

void keyboard_register_event(keyboard_dev_t *keyboard,
                             keyboard_event_t event)
{
  if (!keyboard) return;

  switch (keyboard->type)
  {
  case KEYBOARD_TYPE_PS2_1:
    keyboard_register_ps2_scancode_1(keyboard, event);
    break;
  case KEYBOARD_TYPE_PS2_2:
  case KEYBOARD_TYPE_PS2_3:
  default:
    // Do nothing
    break;
  }
  
  return;
}
