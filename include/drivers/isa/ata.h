// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_DRIVERS_ATA_H
#define POVOS_DRIVERS_ATA_H

//
// Advanced Technology Attachment
// ==============================
//
// ATA aka IDE is an interface for storage devices. It was replaced by
// SATA (Serial ATA), and it is commonly known as PATA (Parallel ATA)
// to differentiate it from the new one. ATA's interface is still
// supported in modern PCs.
//
// There exists also ATAPI, which allows sending SCSI commands to ATA
// devices.
//
// AT Attachment Interface with Extensions (ATA-2) revision 4c (1996):
//  
//             https://www.karry.cz/files/ata2.pdf
//

#include <kernel/macros.h>
#include <libk/stddef.h>
#include <libk/stdbool.h>
#include <bits/port.h>

#define ATA_SECTOR_SIZE 512

// Logical Block Addressing (LBA) mode is a simple linear addressing
// scheme: blocks are located by an integer index, with the first
// block being LBA 0, the second LBA 1, and so on.
#define ATA_HPC 16  // maximum number of heads per cylinder
#define ATA_SPT 63  // maximum number of sectors per track
#define ATA_LBA(cylinder, head, sector) \
  (cynlinder × ATA_HPC + head) × ATA_SPT + (sector − 1)

//
// Ports
//

// There are usually two buses supported, primary (BUS0) and secondary
// (BUS1). Each bus can have two drives, called respectively a
// "master" and a "slave" (even if they do not have a master-slave
// relationship). So there can be a total of 4 ATA drivers in the
// system.
#define ATA_BUS0_BASE_PORT        0x1F0   // uses IRQ14
#define ATA_BUS1_BASE_PORT        0x170   // uses IRQ15

#define ATA_BUS0_CTRL_BASE_PORT   0x3F6
#define ATA_BUS1_CTRL_BASE_PORT   0x376

// These also may exist
#define ATA_BUS3_BASE_PORT        0x1E8
#define ATA_BUS4_BASE_PORT        0x168

//
// Registers
//

// Offsets from base ports
// '*' mapping of registers when in LBA mode
#define ATA_DATA_REG_OFFSET          0   // r/w
#define ATA_ERR_REG_OFFSET           1   // r
#define ATA_FEATURES_REG_OFFSET      1   // w
#define ATA_SECTOR_COUNT_REG_OFFSET  2   // r/w
#define ATA_SECTOR_NUM_REG_OFFSET    3   // r/w, * LBA bits 0-7
#define ATA_CYLINDER_LOW_REG_OFFSET  4   // r/w, * LBA bits 8-15
#define ATA_CYLINDER_HIGH_REG_OFFSET 5   // r/w, * LBA bits 16-23
#define ATA_HEAD_REG_OFFSET          6   // r/w, * LBA bits 24-27
#define ATA_STATUS_REG_OFFSET        7   // r
#define ATA_CMD_REG_OFFSET           7   // w

// Offsets from control base port
#define ATA_CTRL_ALT_STATUS_REG_OFFSET   0   // r
#define ATA_CTRL_DEVICE_REG_OFFSET       0   // w
#define ATA_CTRL_DRIVE_ADDR_REG_OFFSET   1   // r

typedef union ata_err_reg {
  struct _packed {
    bool amnf   : 1;  // address mark not found
    bool tknonf : 1;  // track zero not found
    bool abrt   : 1;  // aborted command
    bool mcr    : 1;  // media change request
    bool idnf   : 1;  // id not found
    bool mc     : 1;  // media changed
    bool unc    : 1;  // uncorrectable data error
    bool bbk    : 1;  // bad block detected
  } flags;
  u8_t raw;
} _packed ata_err_reg_t;

typedef union ata_head_reg {
  struct _packed {
    u8_t head : 4;   // In CHS addressing, bits 0 to 3 of the head. In
                     // LBA addressing, bits 24 to 27 of the block number
    bool drv : 1;    // selects the drive number
    bool set : 1;    // always set
    bool lba : 1;    // uses CHS addressing if clear or LBA addressing if
                     // set
    bool set2: 1;    // always set
  } flags;
  u8_t raw;
} _packed ata_head_reg_t;

typedef union ata_status_reg {
  struct _packed {
    bool err  : 1;  // Indicated an error occurred. Send a new command to
                    // clear it
    bool idx  : 1;  // index, always set to zero
    bool corr : 1;  // Corrected data, always set to 0
    bool drq  : 1;  // Set when the drive has PIO data to transfer, or is
                    // ready to accept PIO data
    bool dsc  : 1;  // Device seek complete
    bool df   : 1;  // Drive Fault Error
    bool rdy  : 1;  // Bit is clear when drive is spun down, or after an
                    // error. Set otherwise.
    bool bsy  : 1;  // Indicated the drive is preparing to send / receive
                    // data (wait for it to clear).
  } flags;
  u8_t raw;
} _packed ata_status_reg_t;

//
// Commands
// Refer to the specification chapter 8 for more information
//

// Pio data in
#define ATA_CMD_PIO_IDENTIFY_DEVICE      0xEC   // mandatory
#define ATA_CMD_PIO_READ_BUFFER          0xE4   // optional
#define ATA_CMD_PIO_READ_LONG_RETRY      0x22   // optional
#define ATA_CMD_PIO_READ_LONG            0x23   // optional
#define ATA_CMD_PIO_READ_MULTIPLE        0xC4   // optional
#define ATA_CMD_PIO_READ_SECTORS_RETRY   0x20   // mandatory
#define ATA_CMD_PIO_READ_SECTORS         0x21   // mandatory
// Pio data out
#define ATA_CMD_PIO_DOWNLOAD_MICROCODE   0x92   // optional
#define ATA_CMD_PIO_WRITE_LONG_RETRY     0x32   // optional
#define ATA_CMD_PIO_WRITE_LONG           0x33   // optional
#define ATA_CMD_PIO_WRITE_MULTIPLE       0xC5   // optional
#define ATA_CMD_PIO_WRITE_SAME           0xE9   // optional
#define ATA_CMD_PIO_WRITE_SECTORS_RETRY  0x30   // mandatory
#define ATA_CMD_PIO_WRITE_SECTORS        0x31   // mandatory
#define ATA_CMD_PIO_WRITE_VERIFY         0x3C   // optional

//
// Functions
//

bool ata_enabled(port_t port);

// Blocks until status has done sending / receiving data
// Retrun status
ata_status_reg_t ata_wait_status(port_t port);

// [dest] must be large enough to contain [secotrs] * ATA_SECTOR_SIZE bytes
bool ata_read(port_t port, u8_t *dest, u32_t from, u32_t sectors);
// Writes [sectors] * ATA_SECTOR_SIZE from [src] to [to]
bool ata_write(port_t port, u8_t *src, u32_t to, u32_t sectors);

#endif // POVOS_DRIVERS_ATA_H
