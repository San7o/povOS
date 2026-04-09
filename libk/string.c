// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/string.h>   // implements

size_t strlen(const char *s)
{
  if (!s) return 0;
  
  size_t len = 0;
  while(*s++ != '\0') { len++; };
  return len;
}

size_t strnlen(const char *s, size_t n)
{
  if (!s) return 0;
  
  size_t len = 0;
  while(*s++ != '\0') { len++; };
  return (len < n) ? len : n;
}

int strcmp(const char *s1, const char *s2)
{
  while (*s1 == *s2 && (*s1 != '\0' && *s2 != '\0')) { s1++; s2++; }
  if (*s1 == '\0' && *s2 == '\0') return 0;
  if (*s1 == '\0') return -1;
  if (*s2 == '\0') return 1;
  return ((*s1 > *s2) ? 1 : -1);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  while (*s1 == *s2 && (*s1 != '\0' && *s2 != '\0') && n > 0)
  { s1++; s2++; n--; }
  if (n == 0) return 0;
  if (*s1 == '\0' && *s2 == '\0') return 0;
  if (*s1 == '\0') return -1;
  if (*s2 == '\0') return 1;
  return ((*s1 > *s2) ? 1 : -1);
}

char *strncpy(char *dst, const char *src, size_t size)
{
  size_t  dlen;
  dlen = strnlen(src, size);
  return memset(mempcpy(dst, src, dlen), 0, size - dlen);
}

char *strcpy(char *dst, const char *src)
{
  return mempcpy(dst, src, strlen(src));
}

void *memcpy(void *dest, const void *src, size_t n)
{
  u8_t* dest_byte = dest;
  const u8_t* src_byte  = src;
  for (size_t i = 0; i < n; ++i)
    dest_byte[i] = src_byte[i];

  return dest;
}

void *mempcpy(void *dest, const void *src, size_t n)
{
  memcpy(dest, src, n);
  return dest + n;
}

void *memset(void *buf, u8_t c, size_t n)
{
  u8_t *_buf = buf;
  for (size_t i = 0; i < n; ++i)
    _buf[i] = c;
  return buf;
}
