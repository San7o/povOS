// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_ISR_STRING_H
#define POVOS_KERNEL_ISR_STRING_H

#define ISR_EXCEPTION_COUNT 32

// Give string values for each exception
extern const char *isr_exception_string[ISR_EXCEPTION_COUNT];

#endif // POVOS_KERNEL_ISR_STRING_H
