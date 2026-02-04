// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_PORT_H
#define POVOS_KERNEL_PORT_H

// Write byte [value] to [port]
void port_outb(const unsigned char port,
               const unsigned char value);

// TODO: port_inb

#endif // POVOS_KERNEL_PORT_H
