// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_UART_H
#define POVOS_DRIVERS_UART_H

//
// UART driver
// ===========
//
// A universal asynchronous receiver-transmitter (UART) is a hardware
// device for asynchronous *serial* communication which is very
// popular in personal computers and embedded systems. It is usually
// the simplest way to communicate with a device.
//
// 
// I/O Ports
// ----------
//
// You access UART registers via I/O ports in an indexed style, with a
// base port + an offset that specifies the register. Each register
// may allow read, write, or both.
#define UART_COM1   0x3F8
#define UART_COM2   0x2F8
// 
// The following addresses are not fully reliable as they depend on
// how the ports are connected to the machine and how the BIOS is
// configured. COM1 and COM2 are mostly reliable.
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
#include <bits/port.h>

//
// Functions
//

bool uart_init_port(port_t port);
bool uart_is_transmit_ready(port_t port);
void uart_putc(port_t port, u8_t c);
void uart_write_str(port_t port, const char *str);
void uart_write_hex(port_t port, u64_t num);

#endif // POVOS_DRIVERS_UART_H
