// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_IDT_H
#define POVOS_KERNEL_IDT_H

void isr_common_handler(long unsigned int isr_number,
                        long unsigned int error_code);

void isr33_handler(long unsigned int isr_number,
                   long unsigned int error_code);

#endif // POVOS_KERNEL_IDT_H
