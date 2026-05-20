// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_CONSOLE_H
#define POVOS_KERNEL_CONSOLE_H

//
// Console
// -------
//
// The console allows a textbuffer to be rendered via a backend. The
// functions of the console are implemented for each backend (like
// VGA).
//

#include <kernel/textbuffer.h>

struct console {
  void (*draw)(const struct textbuffer *textbuffer);
  void (*set_cursor)(unsigned int x, unsigned int y);
  void (*clear)(void);
};

#endif // POVOS_KERNEL_CONSOLE_H
