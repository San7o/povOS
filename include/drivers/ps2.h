// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_PS2_H
#define POVOS_DRIVERS_PS2_H

//
// PS/2
// ====
//
// PS/2 is a type of serial communication, typically used for user
// input devices (keyboard, mouse, bar code scanner, etc).  The ps/2
// controller is also known as Intel 8042 or I8042 for short.
//
//
// I/O ports and registers
// -----------------------
// 
// The PS/2 Controller itself uses 2 IO ports, 0x60 and 0x64. Like
// many IO ports, reads and writes may access different internal
// registers.
//
// The command register is used to send commands to the PS/2
// controller (not the PS/2 devices).
#define PS2_COMMAND_REG   0x64

// The Status Register contains various flags that show the state of
// the PS/2 controller. The meanings for each bit are:
//
//   0    Output buffer status
//        Must be set before attempting to read data from IO port 0x60
//           0 = empty
//           1 = full
//   1    Input buffer status
//           0 = empty
//           1 = full
//        Must be clear before attempting to write data to IO port
//        0x60 or IO port 0x64
//   2    System Flag. Meant to be cleared on reset and set by firmware
//   3    Command/data
//           0 = data written to input buffer is data for PS/2 device
//           1 = data written to input buffer is data for PS/2 controller
//               command
//   4    Unknown
//   5    Unknown
//   6    Time-out error
//           0 = no error
//           1 = time-out error
//   7    Parity error
//           0 = no error
//           1 = parity error
#define PS2_STATUS_REG   0x64

// The Data Port (IO Port 0x60) is used for reading data that was
// received from a PS/2 device or from the PS/2 controller itself
// and writing data to a PS/2 device or to the PS/2 controller
// itself.
#define PS2_DATA_REG   0x60

//
// Commands
// --------
//
// To send a command to the controller, write the command byte to IO
// port 0x64. If the command is 2 bytes long, then the next byte
// needs to be written to IO Port 0x60 after making sure that the
// controller is ready for it (by making sure bit 1 of the Status
// Register is clear). If there is a response byte, then the
// response byte needs to be read from IO Port 0x60 after making
// sure it has arrived (by making sure bit 0 of the Status Register
// is set).
// 
// Read "byte 0" from internal RAM. Response is a Controller
// Configuration Byte.
#define PS2_COMMAND_READ_BYTE_0   0x20
// Read "byte N" from internal RAM (where 'N' is the command byte &
// 0x1F). The response is non standard.
#define PS2_COMMAND_READ_BYTE_N   0x21
// Other commands can be found in the OS Dev wiki
//    https://wiki.osdev.org/I8042_PS/2_Controller
//

//
// Functions
//

unsigned char ps2_read_data(void);
                                
#endif // POVOS_DRIVERS_PS2_H
