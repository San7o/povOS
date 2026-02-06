// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_PORT_H
#define POVOS_KERNEL_PORT_H

#include <libk/stddef.h>

typedef u16_t port_t;

// Write [value] to [port]
void port_outb(port_t  port,
               u8_t   value);
void port_outw(port_t  port,
               u16_t  value);

// Return content read from [port]
unsigned char  port_inb(port_t port);
unsigned short port_inw(u16_t port);


#endif // POVOS_KERNEL_PORT_H
