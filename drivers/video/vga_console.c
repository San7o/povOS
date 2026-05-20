// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/video/vga.h>   // implements

static void vga_console_draw(const struct textbuffer *textbuffer)
{
  if (!textbuffer)
    return;

  for (unsigned int i = 0; i < textbuffer->width * textbuffer->height; ++i) {
    vga_putc(i, textbuffer->buff[i].c,
             VGA_STYLE_MAKE(textbuffer->buff[i].style.foreground,
                            textbuffer->buff[i].style.background));
  }
  
  return;
}

static void vga_console_set_cursor(unsigned int x, unsigned int y)
{
  vga_set_cursor(x, y);
  return;
}

static void vga_console_clear(void)
{
  vga_clear(VGA_STYLE_BW);
  return;
}

struct console vga_console = {
  .draw         = vga_console_draw,
  .set_cursor   = vga_console_set_cursor,
  .clear        = vga_console_clear,
};
