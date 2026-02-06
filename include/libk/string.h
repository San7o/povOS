// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef LIBK_STRING_H
#define LIBK_STRING_H

#include <libk/stddef.h>

int strcmp(const unsigned char *s1, const unsigned char *s2);
int strncmp(const unsigned char *s1, const unsigned char *s2, size_t n);

#endif // LIBK_STRING_H
