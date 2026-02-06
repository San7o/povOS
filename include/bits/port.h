// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_PORT_H
#define POVOS_KERNEL_PORT_H

// Write [value] to [port]
void port_outb(const unsigned short  port,
               const unsigned char  value);
void port_outw(const unsigned short port,
               const unsigned short value);

// Return content read from [port]
unsigned char  port_inb(const unsigned short port);
unsigned short port_inw(const unsigned short port);


#endif // POVOS_KERNEL_PORT_H
