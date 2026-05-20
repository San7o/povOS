// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_RANGE_H
#define POVOS_KERNEL_RANGE_H

#include <libk/stddef.h>

struct range {
  size_t start;
  size_t end;
};

#define RANGE_MAKE(_start, _end) \
  (struct range) { .start = _start, .end = _end }

#endif // POVOS_KERNEL_RANGE_H
