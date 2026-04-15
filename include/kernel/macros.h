// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_MACROS_H
#define POVOS_KERNEL_MACROS_H

#define _packed __attribute__((packed))

#define NOP __asm__ volatile ("nop")

// X86_PAUSE
//
// Improves the performance of spin-wait loops, should be used instead
// of NOP.
//
// When executing a “spin-wait loop,” processors will suffer a severe
// performance penalty when exiting the loop because it detects a
// possible memory order violation. The PAUSE instruction provides a
// hint to the processor that the code sequence is a spin-wait
// loop. The processor uses this hint to avoid the memory order
// violation in most situations, which greatly improves processor
// performance. For this reason, it is recommended that a PAUSE
// instruction be placed in all spin-wait loops.
#define X86_PAUSE __asm__ volatile("pause")

#define UNUSED(value) (void)(value)

#endif // POVOS_KERNEL_MACROS_H
