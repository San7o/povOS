// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef LIBK_STDLIB_H
#define LIBK_STDLIB_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

unsigned int rand(void);
void srand(unsigned int seed);

char *itoa(int value, char *str, int base);

#endif // LIBK_STDLIB_H
