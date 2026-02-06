// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// UART driver
// ===========
//
//
// You access UART registers via I/O ports in an indexed style, with
// a base port + an offset that specifies the register. Each
// register can allow read, write, or both.
//

#ifndef POVOS_DRIVERS_UART_H
#define POVOS_DRIVERS_UART_H


// 
// I/O Ports
// ----------
// 
#define UART_COM1   0x3F8
#define UART_COM2   0x2F8
// 
// The following addresses are not fully reliable as they depend
// on how the ports are connected to the machine and how the BIOS
// is configured. COM1 and COM2 are mostly reliable.
// 
#define UART_COM3   0x3E8
#define UART_COM4   0x2E8
#define UART_COM5   0x5F8
#define UART_COM6   0x4F9
#define UART_COM7   0x5E8
#define UART_COM8   0x4E8
  
// 
// Register index (offset from port)
// ---------------------------------
// 
#define UART_REGISTER_RECEIVE_BUF_INDEX        0x0       // r
#define UART_REGISTER_TRANSMIT_BUF_INDEX       0x0       // w
#define UART_REGISTER_INTERRUPT_ENABLE_INDEX   0x1       // r/w
// With DLAB set to 1, this is the least significant byte of the
// divisor value for setting the baud rate.
#define UART_REGISTER_DIVISOR_LOW_INDEX        0x0       // r/w, DLAB=1
// With DLAB set to 1, this is the most significant byte of the
// divisor value.
#define UART_REGISTER_DIVISOR_HIGH_INDEX       0x1       // r/w, DLAB=1
#define UART_REGISTER_INTERRUPT_ID_INDEX       0x2       // r
#define UART_REGISTER_FIFO_CONTROL_INDEX       0x2       // w
//  Line Control Register. The most significant bit of this register
// is the DLAB.
#define UART_REGISTER_LINE_CONTROL_INDEX       0x3       // r/w
#define UART_REGISTER_MODEM_CONTROL_INDEX      0x4       // r/w
#define UART_REGISTER_LINE_STATUS_INDEX        0x5       // r
#define UART_REGISTER_MODEM_STATUS_INDEX       0x6       // r
#define UART_REGISTER_SCRATCH_INDEX            0x7       // r/w

//
// Types
//

#include <libk/stdbool.h>

//
// Functions
//

bool uart_init_port(unsigned short port);
bool uart_is_transmit_ready(unsigned short port);
void uart_putc(unsigned short port, unsigned char c);
void uart_write_str(unsigned short port, const char *str);
void uart_write_hex(unsigned short port, unsigned long int num);

#endif // POVOS_DRIVERS_UART_H
