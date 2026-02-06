// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/vga.h>   // implements
#include <bits/port.h>

vga_entry_t *vga_buffer = (vga_entry_t*) VGA_START;

int vga_get_memory_map(void)
{
  port_outb(VGA_ADDRESS_REGISTER, VGA_GRAPHICS_MISC_REGISTER_INDEX);
  unsigned int misc_register = port_inb(VGA_DATA_REGISTER);
  return (misc_register & VGA_GRAPHICS_MISC_REGISTER_MEMORY_MAP_SELECT_MASK) >> 2;
}

bool vga_is_alpha_disabled(void)
{
  port_outb(VGA_ADDRESS_REGISTER, VGA_GRAPHICS_MISC_REGISTER_INDEX);
  unsigned int misc_register = port_inb(VGA_DATA_REGISTER);
  return misc_register & VGA_GRAPHICS_MISC_REGISTER_ALPHA_DISABLED_MASK;
}

void vga_putc(int offset, u8_t c, vga_style_t style)
{
  if (offset >= VGA_BUFFER_SIZE) return;
  
  vga_buffer[offset] = (vga_entry_t) {
    .value = c,
    .style = style,
  };
  return;
}

size_t vga_print(int offset, const char* str, vga_style_t style)
{
  size_t i = 0;
  while (*str != '\0' && i + offset < VGA_BUFFER_SIZE)
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
  for (int i = 0; i < VGA_BUFFER_SIZE; ++i)
    vga_putc(i, ' ', style);
  return;
}
