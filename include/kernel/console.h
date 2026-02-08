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

typedef struct console {
  void (*draw)(const textbuffer_t *textbuffer);
  void (*set_cursor)(unsigned int x, unsigned int y);
  void (*clear)(void);
} console_t;

#endif // POVOS_KERNEL_CONSOLE_H
