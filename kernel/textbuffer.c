// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/video/vga.h>
#include <kernel/textbuffer.h>   // implements

void textbuffer_init(struct textbuffer *tb,
                     struct textbuffer_entry *buff,
                     unsigned int width,
                     unsigned int height,
                     unsigned int cursor_x,
                     unsigned int cursor_y)
{
  if (!tb)
    return;

  tb->buff     = buff;
  tb->width    = width;
  tb->height   = height;
  tb->cursor_x = cursor_x;
  tb->cursor_y = cursor_y;

  for (unsigned int i = 0; i < width * height; ++i) {
    tb->buff[i] = (struct textbuffer_entry) {
      .c = ' ',
      .style = (struct textbuffer_style) {
        .foreground = VGA_COLOR_WHITE,
        .background = VGA_COLOR_BLACK,
      },
    };
  }

  return;
}

void textbuffer_write(struct textbuffer *tb,
                      struct textbuffer_entry entry)
{
  if (!tb)
    return;

  textbuffer_write_pos(tb, entry,
                       tb->cursor_x,
                       tb->cursor_y);
  textbuffer_cursor_advance(tb);
  return;
}

void textbuffer_write_pos(struct textbuffer *tb,
                          struct textbuffer_entry entry,
                          unsigned int x,
                          unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    return;

  tb->buff[y * tb->width + x] = entry;
  return;
}

struct textbuffer_entry textbuffer_read(struct textbuffer *tb,
                                        unsigned int x, unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    return (struct textbuffer_entry) {0};

  return tb->buff[y * tb->width + x];
}

unsigned int textuffer_cursor_get_x(struct textbuffer *tb)
{
  if (!tb)
    return 0;
  return tb->cursor_x;
}

unsigned int textuffer_cursor_get_y(struct textbuffer *tb)
{
  if (!tb)
    return 0;
  return tb->cursor_y;
}

void textbuffer_cursor_move(struct textbuffer *tb,
                            unsigned int x, unsigned int y)
{
  if (!tb || x >= tb->width || y >= tb->height
      || x < 0 || y < 0)
    return;

  tb->cursor_x = x;
  tb->cursor_y = y;

  return;
}
void textbuffer_cursor_advance(struct textbuffer *tb)
{
  if (!tb)
    return;

  if (tb->cursor_x == tb->width) {
    tb->cursor_x = 0;
    tb->cursor_y =
      (tb->cursor_y + 1) % tb->height;
  } else {
    tb->cursor_x++;
  }
  return;
}

void textbuffer_cursor_regress(struct textbuffer *tb)
{
  if (!tb)
    return;

  if (tb->cursor_x == 0) {
    if (tb->cursor_y == 0)
      return;
    tb->cursor_y--;
    tb->cursor_x = tb->width - 1;
  } else {
    tb->cursor_x--;
  }
  return;
}

struct textbuffer_entry textbuffer_cursor_read(struct textbuffer *tb)
{
  if (!tb || tb->cursor_x >= tb->width
      || tb->cursor_y >= tb->height)
    return (struct textbuffer_entry) {0};

  unsigned int index = tb->cursor_y * tb->width + tb->cursor_x;
  return tb->buff[index];
}

void textbuffer_cursor_newline(struct textbuffer *tb)
{
  if (!tb)
    return;

  tb->cursor_x = 0;
  tb->cursor_y = (tb->cursor_y + 1) % tb->height;

  return;
}
