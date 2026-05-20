// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_LIBK_STDIO_H
#define POVOS_LIBK_STDIO_H

#include <stdarg.h>

int printk(const char* fmt, ...);
int vprintk(const char* fmt, va_list list);

#endif // POVOS_LIBK_STDIO_H
