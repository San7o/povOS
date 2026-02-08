// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_TTY_H
#define POVOS_KERNEL_TTY_H

//
// TTY driver
// ----------
//
// The tty sits between input (keyboard events, bytes written to it)
// and an output (a textbuffer or framebuffer). It manages the
// framebuffer based on it's inputs, it essentially implements the
// logic of the "terminal interface" or a simple text editor.
//

#include <kernel/input/input.h>
#include <kernel/textbuffer.h>
#include <kernel/console.h>

typedef struct tty {
  textbuffer_t *textbuffer;
  console_t    *console;
  bool          escaped;
} tty_t;

void tty_init(tty_t *tty, textbuffer_t *textbuffer, console_t *console);

void tty_write_input(tty_t *tty, input_event_t event);
void tty_write(tty_t *tty, const char *buf, size_t len);

void tty_flush(tty_t *tty);

#endif // POVOS_KERNEL_TTY_H
