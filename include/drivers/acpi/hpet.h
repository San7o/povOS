// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_HPET_H
#define POVOS_DRIVERS_HPET_H

//
// High Precision Event Timer
// ==========================
//
// HPET is a piece of hardware designed by Intel and Microsoft to
// replace older PIT and RTC. It consists of (usually 64-bit) main
// counter (which counts up), as well as from 3 to 32 32-bit or 64-bit
// wide comparators. HPET is programmed using memory mapped IO, and
// the base address of HPET can be found using ACPI.
//
// Datasheet: IA-PC HPET (High Precision Event Timers) Specification
//

#include <drivers/acpi/acpi.h>
#include <libk/stdbool.h>

#define HPET_ACPI_SIGNATURE "HPET"

typedef struct hpet_acpi_sdt {
  acpi_sdt_header_t header;
  u32_t event_timer_block_id;
  u32_t reserved;
  u64_t address;   // base the address of the registers
  u8_t  id;
  u16_t min_ticks;
  u8_t  page_protection;
} _packed hpet_acpi_sdt_t;

// Offset from hpet_acpi_sdt_t->addresss
#define HPET_GENERAL_CAPABILITIES_REGISTER_OFFSET   0x0    // r
#define HPET_GENERAL_CONFIG_REGISTER_OFFSET         0x10   // r/w
#define HPET_GENERAL_INTERRUPT_STATIS_REGISTER      0x20   // r/w
// The counter counts ticks, which can be converted in realtime by
// multiplying them with the timer period in the capabilities register
#define HPET_MAIN_COUNTER_REGISTER_OFFSET           0xF0   // r/w

#define HPET_TIMER_N_CONFIG_CAP_REGISTER_OFFSET(N) \
  (0x100 + 0x20 * (N))  // r/w
#define HPET_TIMER_N_COMPARATOR_VALUE_REGISTER_OFFSET(N) \
  (0x108 + 0x20 * (N))  // r/w
#define HPET_TIMER_N_FSB_INTERRUP_ROUTE_REGISTER_OFFSET(N) \
  (0x110 + 0x20 * (N))  // r/w

// Returns femtoseconds per tick
// This can be called only once during initialization
u32_t hpet_get_period_fs(void* hpet_base_reg);

void hpet_enable(void* hpet_base_reg, bool legacy_replacement);
void hpet_disable(void* hpet_base_reg);

// Set the period in femtoseconds (10^-15 seconds)
void  hpet_set_period(void* hpet_base_reg, u32_t period_fs);
u64_t hpet_poll(void* hpet_base_reg);

#endif // POVOS_DRIVERS_HPET_H
