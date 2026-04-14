// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_IOAPIC_H
#define POVOS_DRIVERS_IOAPIC_H

#include <drivers/acpi/acpi.h>

#define IOAPIC_ACPI_SIGNATURE "APIC"

typedef struct ioapic_record_header {
  u8_t entry_type;
  u8_t length;
} ioapic_record_header_t;

// Record type 0
//typedef struct ioapic_local_apic_record {
  // TODO
//} ioapic_local_apic_record_t;

// Record type 1
// TODO

// Record type 2
// TODO

// Record type 3
// TODO

// Record type 4
// TODO

// Record type 5
// TODO

// Record type 9
// TODO

typedef struct ioapic_acpi_sdt {
  acpi_sdt_header_t header;
  u32_t local_addr;
  u32_t flags;
  ioapic_record_header_t records[];
} _packed ioapic_acpi_sdt_t;

#endif // POVOS_DRIVERS_IOAPIC_H
