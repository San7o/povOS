// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/textbuffer.h>   // implements
#include <drivers/video/vga.h>

void textbuffer_init(textbuffer_t *textbuffer,
                     textbuffer_entry_t *buff,
                     unsigned int width,
                     unsigned int height,
                     unsigned int cursor_x,
                     unsigned int cursor_y)
{
  if (!textbuffer) return;

  textbuffer->buff     = buff;
  textbuffer->width    = width;
  textbuffer->height   = height;
  textbuffer->cursor_x = cursor_x;
  textbuffer->cursor_y = cursor_y;

  for (unsigned int i = 0; i < width * height; ++i)
  {
    textbuffer->buff[i] = (textbuffer_entry_t) {
      .c = ' ',
      .style = (textbuffer_style_t) {
        .foreground = VGA_COLOR_WHITE,
        .background = VGA_COLOR_BLACK,
      },
    };
  }
  
  return;
}

void textbuffer_write(textbuffer_t *textbuffer,
                      char c,
                      unsigned char foreground_color,
                      unsigned char background_color)
{
  if (!textbuffer) return;
  
  textbuffer_write_pos(textbuffer, c,
                       foreground_color,
                       background_color,
                       textbuffer->cursor_x,
                       textbuffer->cursor_y);
  textbuffer_cursor_advance(textbuffer);
  return;
}

void textbuffer_write_style(textbuffer_t *textbuffer,
                            char c,
                            textbuffer_style_t style)
{
  if (!textbuffer) return;

  textbuffer_write_pos_style(textbuffer, c, style,
                             textbuffer->cursor_x,
                             textbuffer->cursor_y);
  textbuffer_cursor_advance(textbuffer);
  return;
}

void textbuffer_write_entry(textbuffer_t *textbuffer,
                            textbuffer_entry_t entry)
{
  if (!textbuffer) return;

  textbuffer_write_pos_entry(textbuffer, entry,
                             textbuffer->cursor_x,
                             textbuffer->cursor_y);
  textbuffer_cursor_advance(textbuffer);
  return;
}


void textbuffer_write_pos(textbuffer_t *textbuffer,
                          char c,
                          textbuffer_color_t foreground,
                          textbuffer_color_t background,
                          unsigned int x,
                          unsigned int y)
{
  textbuffer_write_pos_style(textbuffer, c,
                             (textbuffer_style_t) {
                               .foreground = foreground,
                               .background = background },
                             x, y);
  return;
}

void textbuffer_write_pos_style(textbuffer_t *textbuffer,
                                char c,
                                textbuffer_style_t style,
                                unsigned int x,
                                unsigned int y)
{
  textbuffer_write_pos_entry(textbuffer,
                             (textbuffer_entry_t) { .c = c, .style = style },
                             x, y);
  return;
}

void textbuffer_write_pos_entry(textbuffer_t *textbuffer,
                                textbuffer_entry_t entry,
                                unsigned int x,
                                unsigned int y)
{
  if (!textbuffer || x >= textbuffer->width || y >= textbuffer->height
      || x < 0 || y < 0)
    return;

  textbuffer->buff[y * textbuffer->width + x] = entry;
  return;
}

textbuffer_entry_t textbuffer_read(textbuffer_t *textbuffer,
                                   unsigned int x, unsigned int y)
{
  if (!textbuffer || x >= textbuffer->width || y >= textbuffer->height
      || x < 0 || y < 0)
    goto exit;

  return textbuffer->buff[y * textbuffer->width + x];
  
 exit:
  return (textbuffer_entry_t) {0};
}

unsigned int textuffer_cursor_get_x(textbuffer_t *textbuffer)
{
  if (!textbuffer) return 0;
  return textbuffer->cursor_x;
}

unsigned int textuffer_cursor_get_y(textbuffer_t *textbuffer)
{
  if (!textbuffer) return 0;
  return textbuffer->cursor_y;
}

void textbuffer_cursor_move(textbuffer_t *textbuffer,
                            unsigned int x, unsigned int y)
{
  if (!textbuffer || x >= textbuffer->width || y >= textbuffer->height
      || x < 0 || y < 0)
    return;

  textbuffer->cursor_x = x;
  textbuffer->cursor_y = y;
  
  return;
}
void textbuffer_cursor_advance(textbuffer_t *textbuffer)
{
  if (!textbuffer)
    return;

  if (textbuffer->cursor_x == textbuffer->width)
  {
    textbuffer->cursor_x = 0;
    textbuffer->cursor_y =
      (textbuffer->cursor_y + 1) % textbuffer->height;
  }
  else
  {
    textbuffer->cursor_x++;
  }
  return;
}

void textbuffer_cursor_regress(textbuffer_t *textbuffer)
{
  if (!textbuffer)
    return;

  if (textbuffer->cursor_x == 0)
  {
    if (textbuffer->cursor_y == 0) return;
    textbuffer->cursor_y--;
    textbuffer->cursor_x = textbuffer->width - 1;
  }
  else
  {
    textbuffer->cursor_x--;
  }
  return;
}

textbuffer_entry_t textbuffer_cursor_read(textbuffer_t *textbuffer)
{
  if (!textbuffer || textbuffer->cursor_x >= textbuffer->width
      || textbuffer->cursor_y >= textbuffer->height)
    goto exit;

  unsigned int index = textbuffer->cursor_y * textbuffer->width + textbuffer->cursor_x;
  return textbuffer->buff[index];
  
 exit:
  return (textbuffer_entry_t) {0};  
}

void textbuffer_cursor_newline(textbuffer_t *textbuffer)
{
  if (!textbuffer) return;

  textbuffer->cursor_x = 0;
  textbuffer->cursor_y = (textbuffer->cursor_y + 1) % textbuffer->height;
  
  return;
}
