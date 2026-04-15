// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_ISA_H
#define POVOS_DRIVERS_ISA_H

//
// Industry Standard Architecture
// ==============================
//
// The Industry Standard Architecture was a bus for the original IBM
// PC from 1981. It was designed to connect peripheral cards to the
// motherboard and allows for bus mastering. Nowdays ISA is obsolete
// and not present in moder computers, however motherboards still
// support its interface, such as the PIIX Southbridge through the LPC
// bus and its successor PCH through eSPI bus (which still supports
// ATA, PIC, PIT...).
//
// ISA allowed a plug-and-play mechanism, but required manual
// enumeration and resource allocation of each device which is quite
// tedious to do. Modern busses like PCIe use enumeration to do this
// automatically. This driver does not support configuration, hence
// this is not a complete driver.
//
// You use I/O ports to send commands to the 8-bit configuration
// registers, which are connected to the motherboard via these really
// wide cables that you would find in very old PCs. ISA does not
// specify a "protocol" for communication, so you simply write
// directly to the ports.
//
// The specification:
//
//     http://www.osdever.net/documents/PNP-ISA-v1.0a.pdf 
//
// The Plug and Play registers are accessed by first writing the
// address of the desired register to the ADDRESS port, followed by a
// read of data from the READ_DATA port or a write of data to the
// WRITE_DATA port. A write to the ADDRESS port may be followed by any
// number of WRITE_DATA or READ_DATA accesses to the same register
// location without the need to write to the ADDRESS port before each
// access.
//

#include <bits/port.h>

// These can be used to configure the Plug and Play mechanism
#define ISA_PNP_ADDRESS_PORT           0x279   // w
#define ISA_PNP_WRITE_DATA_PORT        0xA79   // w
#define ISA_PNP_READ_DATA_START_PORT   0x203   // r, up to 0x3FF

void isa_pnp_set_config_mode(void);

#define isa_writeb(port, value) \
  port_outb(port, value)

#define isa_writew(port, value) \
  port_outw(port, value)

#define isa_readb(port) \
  port_inb(port)

#define isa_readw(port) \
  port_inw(port)

#endif // POVOS_DRIVERS_ISA_H
