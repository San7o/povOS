// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_INPUT_KEYMAP_H
#define POVOS_KERNEL_INPUT_KEYMAP_H

//
// Keyboard layouts
// ----------------
//

#include <drivers/input/keyboard.h>

typedef char input_keymap_t[_KEY_MAX][2]; // 0 = unshifted, 1  = shifted

// Available layouts

extern input_keymap_t input_keymap_us;

#endif // POVOS_KERNEL_INPUT_KEYMAP_H
