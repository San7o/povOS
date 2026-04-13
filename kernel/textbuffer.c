// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/textbuffer.h>   // implements
#include <drivers/video/vga.h>

void textbuffer_init(textbuffer_t *tb,
                     textbuffer_entry_t *buff,
                     unsigned int width,
                     unsigned int height,
                     unsigned int cursor_x,
                     unsigned int cursor_y)
{
  if (!tb) return;

  tb->buff     = buff;
  tb->width    = width;
  tb->height   = height;
  tb->cursor_x = cursor_x;
  tb->cursor_y = cursor_y;

  for (unsigned int i = 0; i < width * height; ++i)
  {
    tb->buff[i] = (textbuffer_entry_t) {
      .c = ' ',
      .style = (textbuffer_style_t) {
        .foreground = VGA_COLOR_WHITE,
        .background = VGA_COLOR_BLACK,
      },
    };
  }
  
  return;
}

void textbuffer_write(textbuffer_t *tb,
                      textbuffer_entry_t entry)
{
  if (!tb) return;

  textbuffer_write_pos(tb, entry,
                       tb->cursor_x,
                       tb->cursor_y);
  textbuffer_cursor_advance(tb);
  return;
}

void textbuffer_write_pos(textbuffer_t *tb,
                          textbuffer_entry_t entry,
                          unsigned int x,
                          unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    return;

  tb->buff[y * tb->width + x] = entry;
  return;
}

textbuffer_entry_t textbuffer_read(textbuffer_t *tb,
                                   unsigned int x, unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    goto exit;

  return tb->buff[y * tb->width + x];
  
 exit:
  return (textbuffer_entry_t) {0};
}

unsigned int textuffer_cursor_get_x(textbuffer_t *tb)
{
  if (!tb) return 0;
  return tb->cursor_x;
}

unsigned int textuffer_cursor_get_y(textbuffer_t *tb)
{
  if (!tb) return 0;
  return tb->cursor_y;
}

void textbuffer_cursor_move(textbuffer_t *tb,
                            unsigned int x, unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    return;

  tb->cursor_x = x;
  tb->cursor_y = y;
  
  return;
}
void textbuffer_cursor_advance(textbuffer_t *tb)
{
  if (!tb)
    return;

  if (tb->cursor_x == tb->width)
  {
    tb->cursor_x = 0;
    tb->cursor_y =
      (tb->cursor_y + 1) % tb->height;
  }
  else
  {
    tb->cursor_x++;
  }
  return;
}

void textbuffer_cursor_regress(textbuffer_t *tb)
{
  if (!tb)
    return;

  if (tb->cursor_x == 0)
  {
    if (tb->cursor_y == 0) return;
    tb->cursor_y--;
    tb->cursor_x = tb->width - 1;
  }
  else
  {
    tb->cursor_x--;
  }
  return;
}

textbuffer_entry_t textbuffer_cursor_read(textbuffer_t *tb)
{
  if (!tb || tb->cursor_x >= tb->width
      || tb->cursor_y >= tb->height)
    goto exit;

  unsigned int index = tb->cursor_y * tb->width + tb->cursor_x;
  return tb->buff[index];
  
 exit:
  return (textbuffer_entry_t) {0};  
}

void textbuffer_cursor_newline(textbuffer_t *tb)
{
  if (!tb) return;

  tb->cursor_x = 0;
  tb->cursor_y = (tb->cursor_y + 1) % tb->height;
  
  return;
}
