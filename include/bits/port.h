// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_PORT_H
#define POVOS_KERNEL_PORT_H

#include <kernel/macros.h>
#include <libk/stddef.h>

typedef u16_t port_t;

// These functions are implemented in assembly. Do not use them
extern void _port_outb(port_t port, u8_t value);
extern void _port_outw(port_t port, u8_t value);
extern void _port_outdw(port_t port, u8_t value);
extern u8_t  _port_inb(port_t port);
extern u16_t _port_inw(port_t port);
extern u32_t _port_indw(port_t port);

// Write [value] to [port]

static inline void port_outb(port_t port, u8_t val)  { _port_outb(port, val); SW_FENCE; }
static inline void port_outw(port_t port, u8_t val)  { _port_outw(port, val); SW_FENCE; }
static inline void port_outdw(port_t port, u8_t val) { _port_outdw(port, val); SW_FENCE; }

// Return content read from [port]

static inline u8_t  port_inb(port_t port)  { SW_FENCE; return _port_inb(port); }
static inline u16_t port_inw(port_t port)  { SW_FENCE; return _port_inw(port); }
static inline u32_t port_indw(port_t port) { SW_FENCE; return _port_indw(port); }

#endif // POVOS_KERNEL_PORT_H
