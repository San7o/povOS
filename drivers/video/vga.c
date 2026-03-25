// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/video/vga.h>   // implements
#include <bits/port.h>

vga_text_entry_t *vga_textbuffer = (vga_text_entry_t*) VGA_TEXT_BUFFER_START;
vga_color_t *vga_framebuffer     = (vga_color_t*) VGA_FRAME_BUFFER_START;


int vga_get_memory_map(void)
{
  port_outb(VGA_GRAPHICS_ADDRESS_REGISTER, VGA_GRAPHICS_MISC_REGISTER_INDEX);
  unsigned int misc_register = port_inb(VGA_GRAPHICS_DATA_REGISTER);
  return (misc_register & VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK) >> 2;
}

bool vga_is_text_mode(void)
{
  port_outb(VGA_GRAPHICS_ADDRESS_REGISTER, VGA_GRAPHICS_MISC_REGISTER_INDEX);
  unsigned int misc_register = port_inb(VGA_GRAPHICS_DATA_REGISTER);
  return !(misc_register & VGA_GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK);
}

static unsigned char g_320x200x256[] =
{
  // MISC
	0x63,
  // SEQ
	0x03, 0x01, 0x0F, 0x00, 0x0E,
  // CRTC
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
  // GC
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
  // AC
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS  		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
                          VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

static void vga_write_regs(unsigned char *regs)
{
	unsigned i;

  // write miscellaneous reg
	port_outb(VGA_MISC_OUTPUT_WRITE_REGISTER, *regs);
	regs++;
  // write sequencer regs
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		port_outb(VGA_SEQUENCER_ADDRESS_REGISTER, i);
		port_outb(VGA_SEQUENCER_DATA_REGISTER, *regs);
		regs++;
	}
  // unlock crtc registers
	port_outb(VGA_CRTC_ADDRESS_REGISTER, 0x03);
	port_outb(VGA_CRTC_DATA_REGISTER, port_inb(VGA_CRTC_DATA_REGISTER) | 0x80);
	port_outb(VGA_CRTC_ADDRESS_REGISTER, 0x11);
	port_outb(VGA_CRTC_DATA_REGISTER, port_inb(VGA_CRTC_DATA_REGISTER) & ~0x80);
  // make sure they remain unlocked
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
  // write crtc regs
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		port_outb(VGA_CRTC_ADDRESS_REGISTER, i);
		port_outb(VGA_CRTC_DATA_REGISTER, *regs);
		regs++;
	}
  // write graphics controller regs
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		port_outb(VGA_GRAPHICS_ADDRESS_REGISTER, i);
		port_outb(VGA_GRAPHICS_DATA_REGISTER, *regs);
		regs++;
	}
  // write attribute controller regs
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)port_inb(VGA_INPUT_STATUS_1_COLOR_READ_REGISTER);
		port_outb(VGA_ATTRIBUTE_CTRL_ADDRESS_REGISTER, i);
		port_outb(VGA_ATTRIBUTE_CTRL_ADDRESS_REGISTER, *regs);
		regs++;
	}
  // lock 16-color palette and unblank display
	(void)port_inb(VGA_INPUT_STATUS_1_COLOR_READ_REGISTER);
	port_outb(VGA_ATTRIBUTE_CTRL_ADDRESS_REGISTER, 0x20);
}

void vga_set_graphics_mode(void)
{
  vga_write_regs(g_320x200x256);
}

// This is not implemented yet because going from graphics to
// text would require to reload a font, which is not implemented
// yet
/*
void vga_set_text_mode(void)
{
  vga_write_regs(g_80x25_text);
}
*/

void vga_putc(int offset, u8_t c, vga_style_t style)
{
  if (offset >= VGA_TEXT_BUFFER_SIZE) return;
  
  vga_textbuffer[offset] = (vga_text_entry_t) {
    .value = c,
    .style = VGA_STYLE_BYTES(style),
  };
  return;
}

size_t vga_print(int offset, const char* str, vga_style_t style)
{
  size_t i = 0;
  while (*str != '\0' && i + offset < VGA_TEXT_BUFFER_SIZE)
  {
    vga_putc(i + offset, *str, style);
    ++str;
    ++i;
  }
  return i;
}

size_t vga_print_hex(int offset, u64_t num, vga_style_t style)
{
  vga_putc(offset, '0', style);
  vga_putc(offset + 1, 'x', style);
  offset += 2;
  
  for (unsigned long i = 0; i < sizeof(num) * 2; ++i)
  {
    unsigned char hex = (num >> (sizeof(num) * 8 - 4 * i - 4)) & 0xF;
    if (hex > 9)
    {
      vga_putc(offset + i, 'A' + hex - 10, style);
    }
    else
    {
      vga_putc(offset + i, '0' + hex, style);
    }
  }
  return 0;
}

void vga_clear(vga_style_t style)
{
  for (int i = 0; i < VGA_TEXT_BUFFER_SIZE; ++i)
    vga_putc(i, ' ', style);
  return;
}

void vga_set_cursor(unsigned int x, unsigned int y)
{
  if (x >= VGA_TEXT_BUFFER_WIDTH || y >= VGA_TEXT_BUFFER_HEIGHT
      || x < 0 || y < 0)
    return;

  u16_t pos = y * VGA_TEXT_BUFFER_WIDTH + x;
  
	port_outb(VGA_CRTC_ADDRESS_REGISTER, VGA_CRTC_CURSOR_LOCATION_LOW_REGISTER_INDEX);
	port_outb(VGA_CRTC_DATA_REGISTER, (u8_t) (pos & 0xFF));
	port_outb(VGA_CRTC_ADDRESS_REGISTER, VGA_CRTC_CURSOR_LOCATION_HIGH_REGISTER_INDEX);
  port_outb(VGA_CRTC_DATA_REGISTER, (u8_t) ((pos >> 8) & 0xFF));
  
  return;
}

void vga_draw_pixel(int x, int y, vga_color_t color)
{
  if (x >= VGA_FRAME_BUFFER_WIDTH || y >= VGA_FRAME_BUFFER_HEIGHT
      || x < 0 || y < 0)
    return;

  vga_framebuffer[y * VGA_FRAME_BUFFER_WIDTH + x] = color;
}
