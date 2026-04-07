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
// THE best resource to learn VGA is the following:
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

// 
// Graphics controller registers
// ------------------------------
//
#define VGA_GRAPHICS_ADDRESS_REGISTER   0x3CE
#define VGA_GRAPHICS_DATA_REGISTER      0x3CF

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

//
// CRTC Registers
// --------------
//
// A CRT controller is a device that manages the display parameters of
// a cathode ray tube (CRT), allowing for the control of how images
// are rendered on the screen. It regulates aspects like the timing of
// the electron beams and the refresh rate to ensure smooth image
// display.
//
#define VGA_CRTC_ADDRESS_REGISTER       0x3D4
#define VGA_CRTC_DATA_REGISTER          0x3D5

#define VGA_CRTC_HORIZONTAL_TOTAL_REGISTER_INDEX          0x00
#define VGA_CRTC_END_HORIZONTAL_DISPLAY_REGISTER_INDEX    0x01
#define VGA_CRTC_START_HORIZONTAL_BLANKING_REGISTER_INDEX 0x02
#define VGA_CRTC_END_HORIZONTAL_BLANKING_REGISTER_INDEX   0x03
#define VGA_CRTC_START_HORIZONTAL_RETRACE_REGISTER_INDEX  0x04
#define VGA_CRTC_END_HORIZONTAL_RETRACE_REGISTER_INDEX    0x05
#define VGA_CRTC_VERTICAL_TOTAL_REGISTER_INDEX            0x06
#define VGA_CRTC_OVERFLOW_REGISTER_INDEX                  0x07
#define VGA_CRTC_PRESET_ROW_SCAN_REGISTER_INDEX           0x08
#define VGA_CRTC_MAXIMUM_SCAN_LINE_REGISTER_INDEX         0x09
#define VGA_CRTC_CURSOR_START_REGISTER_INDEX              0x0A
#define VGA_CRTC_CURSOR_END_REGISTER_INDEX                0x0B
#define VGA_CRTC_START_ADDRESS_HIGH_REGISTER_INDEX        0x0C
#define VGA_CRTC_START_ADDRESS_LOW_REGISTER_INDEX         0x0D
#define VGA_CRTC_CURSOR_LOCATION_HIGH_REGISTER_INDEX      0x0E
#define VGA_CRTC_CURSOR_LOCATION_LOW_REGISTER_INDEX       0x0F
#define VGA_CRTC_VERTICAL_RETRACE_START_REGISTER_INDEX    0x10
#define VGA_CRTC_VERTICAL_RETRACE_END_REGISTER_INDEX      0x11
#define VGA_CRTC_VERTICAL_DISPLAY_END_REGISTER_INDEX      0x12
#define VGA_CRTC_OFFSET_REGISTER_INDEX                    0x13
#define VGA_CRTC_UNDERLINE_LOCATION_REGISTER_INDEX        0x14
#define VGA_CRTC_START_VERTICAL_BLANKING_REGISTER_INDEX   0x15
#define VGA_CRTC_END_VERTICAL_REGISTER_BLANKING           0x16
#define VGA_CRTC_CRTC_MODE_CONTROL_REGISTER_INDEX         0x17
#define VGA_CRTC_LINE_COMPARE_REGISTER_INDEX              0x18

//
// Sequencer Registers
// -------------------
//
// The Sequencer is responsible to convert video memory to color
// indexes. It operates in text (alphanumeric) mode or graphics mode.
//
// - in alphanumeric mode the four planes are assigned distinct
//   tasks. Plane 0 contains character data, plane 1 contains
//   attribute data and plane 2 holds the font data.
// - in graphics mode, operations can be divided into two steps:
//   address computation and shift logic. The sequencer computes an
//   address, then reads out the four planes from that address, and
//   generates 8 pixels from these values.
//
#define VGA_SEQUENCER_ADDRESS_REGISTER    0x3C4
#define VGA_SEQUENCER_DATA_REGISTER       0x3C5

#define VGA_SEQUECNER_RESET_REGISTER_INDEX                0x00
#define VGA_SEQUENCER_CLOCKING_MODE_REGISTER_INDEX        0x01
#define VGA_SEQUENCER_MAP_MASK_REGISTER_INDEX             0x02
#define VGA_SEQUENCER_CHARACTER_MAP_SELECT_REGISTER_INDEX 0x03
#define VGA_SEQUENCER_MEMORY_MODE_REGISTER_INDEX          0x04

//
// Attribute Controller Registers
// ------------------------------
//
// The attribute registers are also accessed in an indexed fashion,
// albeit in a more confusing way. The address register is read and
// written via port 0x3C0. The data register is written to port 0x3C0
// and read from port 3C1h. The index and the data are written to the
// same port, one after another.
//
// Because there is no standard method of determining the initial
// state, the ability to reset the state such that the next write will
// be handled as an index is provided. This is accomplished by reading
// the Input Status #1 Register (normally port 0x3DA).
//
#define VGA_ATTRIBUTE_CTRL_ADDRESS_REGISTER      0x3C0
#define VGA_ATTRIBUTE_CTRL_DATA_READ_REGISTER    0x3C1

#define VGA_ATTRIBUTE_CTRL_PALETTE_FIRST_REGISTER_INDEX   0x00 // to 0x0F
#define VGA_ATTRIBUTE_CTRL_MODE_CONTROL_REGISTER_INDEX    0x10
#define VGA_ATTRIBUTE_CTRL_OVERSCAN_CTRL_REGISTER_INDEX   0x11
#define VGA_ATTRIBUTE_CTRL_COLOR_PLANE_ENABLE_REGISTER_INDEX 0x12
#define VGA_ATTRIBUTE_CTRL_HORIZONTAL_PIXEL_PANNING_REGISTER_INDEX 0x13
#define VGA_ATTRIBUTE_CTRL_COLOR_SELECT_REGISTER_INDEX    0x14

//
// Color registers
// ---------------
//
// The standard VGA has 256 palette entries containing six bits each
// of red, green, and blue values. The palette RAM is accessed via a
// pair of address registers and a data register. To write a palette
// entry, output the palette entry's index value to the DAC Address
// Write Mode Register then perform 3 writes to the DAC Data Register,
// loading the red, green, then blue values into the palette RAM. The
// internal write address automatically advances allowing the next
// value's RGB values to be loaded without having to reprogram the DAC
// Address Write Mode Register.
//
#define VGA_DAC_ADDRESS_WRITE_MODE_REGISTER   0x3C8
#define VGA_DAC_ADDRESS_READ_MODE_REGISTER    0x3C7
#define VGA_DAC_DATA_REGISTER_REGISTER        0x3C9
#define VGA_DAC_STATE_REGISTER                0x3C7

//
// External registers
// ------------------
//
// There are easier to access as you can use them directly to read or
// write without indexing.
//
#define VGA_MISC_OUTPUT_READ_REGISTER            0x3CC
#define VGA_MISC_OUTPUT_WRITE_REGISTER           0x3C2
#define VGA_FEATURE_CONTROL_READ_REGISTER        0x3CA
#define VGA_FEATURE_CONTROL_WRITE_REGISTER       0x3BA
#define VGA_INPUT_STATUS_0_MONO_READ_REGISTER    0x3C2
#define VGA_INPUT_STATUS_1_MONO_READ_REGISTER    0x3BA
#define VGA_INPUT_STATUS_1_COLOR_READ_REGISTER   0x3DA

//
// Types
//

#include <kernel/console.h>
#include <kernel/macros.h>
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

typedef u8_t vga_color_t;

typedef struct vga_style {
  vga_color_t foreground;
  vga_color_t background;
} vga_style_t;

// Convert a vga_style_t into bytes for the VGA text buffer
#define VGA_STYLE_BYTES(style) (style.foreground | (style.background << 4))
#define VGA_STYLE_MAKE(fg, bg) \
  (vga_style_t) { .foreground = fg, .background = bg }

// Some default colors
#define VGA_STYLE_BLUE  VGA_STYLE_MAKE(VGA_COLOR_WHITE, VGA_COLOR_BLUE)
#define VGA_STYLE_BW    VGA_STYLE_MAKE(VGA_COLOR_WHITE, VGA_COLOR_BLACK)
#define VGA_STYLE_RED   VGA_STYLE_MAKE(VGA_COLOR_RED, VGA_COLOR_BLACK)

//
// VGA text buffer
// ---------------
//
// VGA uses an in-memory buffer where each entry is two bytes
// representing a character and its color style. You can write to the
// buffer directly, its address can be queried (use the
// vga_get_memory_map function) and the number of entries is
// VGA_TEXT_BUFFER_SIZE.

// An entry in the vga text buffer
typedef struct vga_text_entry {
  u8_t value;
  u8_t style;   // use VGA_STYLE_BYTES(style)
} _packed vga_text_entry_t;

#define VGA_TEXT_BUFFER_START      0x000B8000
#define VGA_TEXT_BUFFER_WIDTH      80
#define VGA_TEXT_BUFFER_HEIGHT     25
#define VGA_TEXT_BUFFER_SIZE       (VGA_TEXT_BUFFER_WIDTH * VGA_TEXT_BUFFER_HEIGHT)
#define VGA_TEXT_BUFFER_EXTENT     (80*25* sizeof(vga_text_entry_t))   // bytes

extern vga_text_entry_t *vga_textbuffer;

//
// VGA framebuffer
// ---------------
//

#define VGA_FRAME_BUFFER_START     0x000A0000
#define VGA_FRAME_BUFFER_WIDTH     320
#define VGA_FRAME_BUFFER_HEIGHT    200
#define VGA_FRAME_BUFFER_SIZE      (VGA_FRAME_BUFFER_WIDTH * VGA_FRAME_BUFFER_END)
#define VGA_FRAME_BUFFER_EXTENT    (VGA_FRAME_BUFFER_SIZE * sizeof(vga_color_t))  // bytes

extern vga_color_t      *vga_framebuffer;

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
// Returns true if text mode is enables
bool vga_is_text_mode(void);

void vga_set_graphics_mode(void); // aka mode 13h (320x200 linear 256-color mode)
// void vga_set_text_mode(void);     // aka mode 3h (80x25 text mode) 

// Text mode

void   vga_putc(int offset, u8_t c, vga_style_t style);
size_t vga_print(int offset, const char* str, vga_style_t style);
size_t vga_print_hex(int offset, u64_t num, vga_style_t style);
void   vga_clear(vga_style_t style);
void   vga_set_cursor(unsigned int x, unsigned int y);

// Graphics mode

void vga_draw_pixel(int x, int y, vga_color_t color);

// VGA console
extern console_t vga_console;

#endif // POVOS_DRIVERS_VGA_H
