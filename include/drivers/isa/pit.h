// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_PIT_H
#define POVOS_DRIVERS_PIT_H

//
// Programmable Interrupt Timer
// ----------------------------
//
// The Programmable Interval Timer (PIT) chip (Intel 8253/8254)
// basically consists of an oscillator, a prescaler and 3 independent
// frequency dividers. Each frequency divider has an output, which is
// used to allow the timer to control external circuitry (for example,
// IRQ 0). The oscillator used by the PIT chip runs at (roughly)
// 1.193182 MHz.
//
// 8253 Datasheet:
//
// https://www.alldatasheet.com/datasheet-pdf/pdf/66098/INTEL/8253.html
//
// PIIX3 Datasheet:
//
// https://www.alldatasheet.com/datasheet-pdf/pdf/66093/INTEL/PIIX3.html
//
// The PIT contains a pair of registers per channel: the count and
// reload count. When the PIT is started the count register is set to
// value of the reload count, and then every time the main clock ticks
// (at 1.19318MHz) the count is decremented by 1. When the count
// register reaches 0 the PIT sends an interrupt. Depending on the
// mode the PIT may then set the count register to the reload register
// again (in mode 2 - periodic operation), or simple stay idle (mode 0
// - one shot operation).
//
#define PIT_CHANNEL_0_DATA_PORT    0x40   // r/w  generates IRQ 0
#define PIT_CHANNEL_1_DATA_PORT    0x41   // r/w  unusable
#define PIT_CHANNEL_2_DATA_PORT    0x42   // r/w  connected to speaker
//
// PIT_MODE_REGISTER
//
// Bits         Usage
// -------------------------------------------------------------------
// 7 and 6      Select channel:
//                 0 0 = Channel 0
//                 0 1 = Channel 1
//                 1 0 = Channel 2
//                 1 1 = Read-back command (8254 only)
// 5 and 4      Access mode:
//                 0 0 = Latch count value command
//                 0 1 = Access mode: lobyte only
//                 1 0 = Access mode: hibyte only
//                 1 1 = Access mode: lobyte/hibyte
// 3 to 1       Operating mode:
//                 0 0 0 = Mode 0 (interrupt on terminal count)
//                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
//                 0 1 0 = Mode 2 (rate generator)
//                 0 1 1 = Mode 3 (square wave generator)
//                 1 0 0 = Mode 4 (software triggered strobe)
//                 1 0 1 = Mode 5 (hardware triggered strobe)
//                 1 1 0 = Mode 2 (rate generator, same as 010b)
//                 1 1 1 = Mode 3 (square wave generator, same as 011b)
// 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
#define PIT_MODE_REGISTER          0x43   // w

#include <libk/stddef.h>

u16_t pit_read_count(void);
void  pit_set_count(u16_t count);

#endif // POVOS_DRIVERS_PIT_H
