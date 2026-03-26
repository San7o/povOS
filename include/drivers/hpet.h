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

#include <drivers/acpi.h>

typedef struct hpet_acpi_sdt {
    acpi_sdt_header_t header;
    u32_t event_timer_block_id;
    u32_t reserved;
    u64_t address;
    u8_t  id;
    u16_t min_ticks;
    u8_t  page_protection;
} __attribute__((packed)) hpet_acpi_sdt_t;

#endif // POVOS_DRIVERS_HPET_H
