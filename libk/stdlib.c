// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>   // implements

char *itoa(int val, char *buf, int base)
{
  char* p = buf;
  char* p1, *p2;
  unsigned long uval = (val < 0 && base == 10) ? -val : val;

  do {
    *p++ = "0123456789abcdef"[uval % base];
  } while (uval /= base);

  if (val < 0 && base == 10) *p++ = '-';
  *p = '\0';

  p1 = buf; p2 = p - 1;
  while (p1 < p2)
  {
    char tmp = *p1; *p1++ = *p2; *p2-- = tmp;
  }
  return buf;
}
