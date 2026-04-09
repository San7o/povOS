// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef LIBK_STRING_H
#define LIBK_STRING_H

#include <libk/stddef.h>

size_t strlen(const char *s);
size_t strnlen(const char *s, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t size);

void *memset(void *buf, u8_t c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *mempcpy(void *dest, const void *src, size_t n);

#endif // LIBK_STRING_H
