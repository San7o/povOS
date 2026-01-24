// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/string.h>

int strcmp(const unsigned char *s1, const unsigned char *s2)
{
  while (*s1 == *s2 && (*s1 != '\0' && *s2 != '\0')) { s1++; s2++; }
  if (*s1 == '\0' && *s2 == '\0') return 0;
  if (*s1 == '\0') return -1;
  if (*s2 == '\0') return 1;
  return ((*s1 > *s2) ? 1 : -1);
}

int strncmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
  while (*s1 == *s2 && (*s1 != '\0' && *s2 != '\0') && n > 0)
  { s1++; s2++; n--; }
  if (n == 0) return 0;
  if (*s1 == '\0' && *s2 == '\0') return 0;
  if (*s1 == '\0') return -1;
  if (*s2 == '\0') return 1;
  return ((*s1 > *s2) ? 1 : -1);
}
