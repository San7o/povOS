// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_ISR_H
#define POVOS_KERNEL_ISR_H

//
// Interrupt service routines handlers
// -----------------------------------
//
// ISR handlers are called after an interrupt is triggered, the setup
// for this is done in assembly. Normally the CPU will push an error
// code on the stack if the interrupt is an exception and these
// functions have to return with the `iretq` instruction.
//
// The assembly code sets up a common interface for the C handlers,
// disables interrupts temporarly and takes care of saving the
// registers and properly returning from the interrupt. This means
// that the C handlers do not need to worry about anything.
//
// Error code 0 means no error.
//

#include <libk/stddef.h>

// Default interrupt handler, when a more specific one is not
// implemented
void isr_common_handler(u8_t  isr_number,
                        u64_t error_code);
void isr_keyboard_handler(u8_t  isr_number,
                          u64_t error_code);

#endif // POVOS_KERNEL_IDT_H
