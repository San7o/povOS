// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_INPUT_KEYBOARD_SCANMAP_H
#define POVOS_DRIVERS_INPUT_KEYBOARD_SCANMAP_H

#include <drivers/input/keyboard.h>

//
// Varous scanmaps
// ---------------
//
// Convert scancode to keyboard events
//

extern const keyboard_event_t keyboard_scanmap_ps2_set1[];
extern const keyboard_event_t keyboard_scanmap_ps2_set1_escaped[];

#endif // POVOS_DRIVERS_INPUT_KEYBOARD_SCANMAP_H
