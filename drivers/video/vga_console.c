#include <drivers/video/vga.h>   // implements

void vga_console_draw(const textbuffer_t *textbuffer)
{
  if (!textbuffer) return;

  // TODO
  
  return;
}

void vga_console_set_cursor(unsigned int x, unsigned int y)
{
  (void) x;
  (void) y;
  
  // TODO
  return;
}

void vga_console_clear(void)
{
  // TODO
  return;
}

console_t vga_console = {
  .draw       = vga_console_draw,
  .set_cursor = vga_console_set_cursor,
  .clear      = vga_console_clear,
};
