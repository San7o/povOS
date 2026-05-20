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

struct tty {
  struct textbuffer *textbuffer;
  struct textbuffer_style style;
  struct console *console;
};

void tty_init(struct tty *tty, struct textbuffer *textbuffer,
              struct textbuffer_style style, struct console *console);

void tty_write_input(struct tty *tty, struct input_event event);
void tty_write(struct tty *tty, const char *buf, size_t len);

void tty_flush(struct tty *tty);

#endif // POVOS_KERNEL_TTY_H
