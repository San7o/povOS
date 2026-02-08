// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o


#ifndef POVOS_DRIVERS_VGA_H
#define POVOS_DRIVERS_VGA_H

//
// VGA Driver
// ==========
//
// VGA, or Video Graphics Array, is a video display standard
// introduced by IBM in 1987 that defines how video signals are
// transmitted from a computer to a display. It uses a 15-pin
// connector and supports a maximum resolution of 640x480 pixels with
// 256 colors.
//
// The best resource to learn VGA is the following:
//    http://www.osdever.net/FreeVGA/vga/vga.htm
//
//
// I/O Ports
// ---------
//
// VGA has pleny of registers. To work with them (reading and writing)
// you use two ports which access two specific registers: an address
// register and a data register. In order to access the other
// registers, you usually write an identifier (INDEX) of that register
// and the address register, then read / write the data register.
#define VGA_ADDRESS_REGISTER   0x3CE
#define VGA_DATA_REGISTER      0x3CF
  
// 
// Graphics controller registers
// ------------------------------
//
#define VGA_GRAPHICS_SET_RESET_REGISTER_INDEX          0x00
#define VGA_GRAPHICS_ENABLE_SET_RESET_REGISTER_INDEX   0x01
#define VGA_GRAPHICS_COLOR_COMPARE_REGISTER_INDEX      0x02
#define VGA_GRAPHICS_DATA_ROTATE_REGISTER_INDEX        0x03
#define VGA_GRAPHICS_READ_MAP_SELECT_REGISTER_INDEX    0x04
#define VGA_GRAPHICS_MODE_REGISTER_INDEX               0x05
#define VGA_GRAPHICS_MISC_REGISTER_INDEX               0x06
#define VGA_GRAPHICS_COLOR_DONT_CARE_REGISTER_INDEX    0x07
#define VGA_GRAPHICS_BIT_MASK_REGISTER_INDEX           0x08
// 
// Masks
// -----
// 
// VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK
// 
// This field specifies the range of host memory addresses that is
// decoded by the VGA hardware and mapped into display memory
// accesses.  The values of this field and their corresponding host
// memory ranges are:
// 
//  00b -- A0000h-BFFFFh (128K region)
//  01b -- A0000h-AFFFFh (64K region)
//  10b -- B0000h-B7FFFh (32K region)
//  11b -- B8000h-BFFFFh (32K region)
// 
#define VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK   0b1100
//
// GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK
//
// This bit controls alphanumeric mode addressing. When set to 1,
// this bit selects graphics modes, which also disables the
// character generator latches.
// 
#define VGA_GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK   0b1

#define VGA_START         0x000B8000
#define VGA_WIDTH         80
#define VGA_HEIGHT        25
#define VGA_BUFFER_SIZE   (VGA_WIDTH * VGA_HEIGHT)
#define VGA_EXTENT        (80*25*2)   // bytes

//
// Types
//

#include <kernel/console.h>
#include <libk/stddef.h>
#include <libk/stdbool.h>

#define VGA_COLOR_BLACK        0
#define VGA_COLOR_BLUE         1
#define VGA_COLOR_GREEN        2
#define VGA_COLOR_CYAN         3
#define VGA_COLOR_RED          4
#define VGA_COLOR_PURPLE       5
#define VGA_COLOR_BROWN        6
#define VGA_COLOR_GRAY         7
#define VGA_COLOR_DARK_GRAY    8
#define VGA_COLOR_LIGHT_BLUE   9
#define VGA_COLOR_LIGHT_GREEN  10
#define VGA_COLOR_LIGHT_CYAN   11
#define VGA_COLOR_LIGHT_RED    12
#define VGA_COLOR_LIGHT_PURPLE 13
#define VGA_COLOR_YELLOW       14
#define VGA_COLOR_WHITE        15

typedef struct __attribute__((packed)) vga_style {
  u8_t foreground;
  u8_t background;
} vga_style_t;

// Convert a vga_style_t into bytes for the VGA text buffer
#define VGA_STYLE_BYTES(style) (style.foreground | (style.background << 4))

// Some default colors
#define VGA_STYLE_BLUE ((vga_style_t) { .foreground = VGA_COLOR_WHITE, .background = VGA_COLOR_BLUE })
#define VGA_STYLE_BW   ((vga_style_t) { .foreground = VGA_COLOR_WHITE, .background = VGA_COLOR_BLACK })
#define VGA_STYLE_RED  ((vga_style_t) { .foreground = VGA_COLOR_WHITE, .background = VGA_COLOR_BLACK })

//
// Global VGA buffer
// -----------------
//
// VGA uses an in-memory buffer where each entry is two bytes
// representing a character and its color style. You can write to the
// buffer directly, its address can be queried (use the
// vga_get_memory_map function) and the number of entries is
// VGA_BUFFER_SIZE.

// An entry in the vga text buffer
typedef struct  __attribute__((packed)) vga_entry {
  u8_t value;
  u8_t style;   // use VGA_STYLE_BYTES(style)
} vga_entry_t;

extern vga_entry_t *vga_buffer;

//
// Functions
//

// Returns the range of host memory addresses decoded by the VGA,
// which can be one of the following values:
//
//  0b00 -- A0000h-BFFFFh (128K region)
//  0b01 -- A0000h-AFFFFh (64K region)
//  0b10 -- B0000h-B7FFFh (32K region)
//  0b11 -- B8000h-BFFFFh (32K region)
// 
int vga_get_memory_map(void);
// Returns true if alphanumeric is disabled
bool vga_is_alpha_disabled(void);

void   vga_putc(int offset, u8_t c, vga_style_t style);
size_t vga_print(int offset, const char* str, vga_style_t style);
size_t vga_print_hex(int offset, u64_t num, vga_style_t style);
void   vga_clear(vga_style_t style);

// VGA console
extern console_t vga_console;

#endif // POVOS_DRIVERS_VGA_H
