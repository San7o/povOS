// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_TTY_H
#define POVOS_DRIVERS_TTY_H

//
// TTY driver
// ----------
//
// The tty sits between input (keyboard events, bytes written to it)
// and an output (a textbuffer or framebuffer). It manages the
// framebuffer based on it's inputs, it essentially implements the
// logic of the "terminal interface" or a simple text editor.
//

#include <drivers/input/keyboard.h>

void tty_input(tty_t *, keyboard_event_t event);
void tty_write(tty_t *, const char *buf, size_t len);

// TODO

#endif // POVOS_KERNEL_TTY_H
