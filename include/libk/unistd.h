// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_LINK_UNISTD_H
#define POVOS_LINK_UNISTD_H

#include <libk/stddef.h>

unsigned int sleep(u64_t seconds);
unsigned int sleep_ms(u64_t milliseconds);
// Busy wait a certain amount of CPU clock ticks
void sleep_ticks(u64_t clocks);

#endif // POVOS_LINK_UNISTD_H
