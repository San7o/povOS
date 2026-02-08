#include <drivers/video/vga.h>   // implements

void vga_console_draw(const textbuffer_t *textbuffer)
{
  if (!textbuffer) return;

  for (unsigned int i = 0; i < textbuffer->width * textbuffer->height; ++i)
  {
    vga_putc(i, textbuffer->buff[i].c, (vga_style_t) {
        .foreground = textbuffer->buff[i].style.foreground,
        .background = textbuffer->buff[i].style.background,
      });
  }
  
  return;
}

void vga_console_set_cursor(unsigned int x, unsigned int y)
{
  vga_set_cursor(x, y);
  return;
}

void vga_console_clear(void)
{
  vga_clear(VGA_STYLE_BW);
  return;
}

console_t vga_console = {
  .draw       = vga_console_draw,
  .set_cursor = vga_console_set_cursor,
  .clear      = vga_console_clear,
};
