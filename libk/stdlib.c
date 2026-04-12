// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <libk/stdlib.h>   // implements

// Magic numbers for LCG pseudo number generator
#define RAND_MAGIC1 1664525    // a
#define RAND_MAGIC2 1013904223 // c
#define RAND_MAGIC3 (1<<31)    // m

static unsigned int rand_seed = 0x505;

unsigned int rand(void)
{
  rand_seed = (RAND_MAGIC1 * rand_seed + RAND_MAGIC2) % RAND_MAGIC3;
  return rand_seed;
}

void srand(unsigned int seed)
{
  rand_seed = seed;
}

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
