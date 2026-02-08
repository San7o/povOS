// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/textbuffer.h>   // implements

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

  return;
}

void textbuffer_write(textbuffer_t *textbuffer,
                      char c,
                      unsigned char foreground_color,
                      unsigned char background_color,
                      unsigned int x,
                      unsigned int y)
{
  textbuffer_write_style(textbuffer, c,
                         (textbuffer_style_t) {
                           .foreground_color = foreground_color,
                           .background_color = background_color },
                         x, y);
  return;
}

void textbuffer_write_style(textbuffer_t *textbuffer,
                            char c,
                            textbuffer_style_t style,
                            unsigned int x,
                            unsigned int y)
{
  textbuffer_write_entry(textbuffer,
                         (textbuffer_entry_t) { .c = c, .style = style },
                         x, y);
  return;
}

void textbuffer_write_entry(textbuffer_t *textbuffer,
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

unsigned int textuffer_get_cursor_x(textbuffer_t *textbuffer)
{
  if (!textbuffer) return 0;
  return textbuffer->cursor_x;
}

unsigned int textuffer_get_cursor_y(textbuffer_t *textbuffer)
{
  if (!textbuffer) return 0;
  return textbuffer->cursor_y;
}

void textbuffer_move_cursor(textbuffer_t *textbuffer,
                            unsigned int x, unsigned int y)
{
  if (!textbuffer || x >= textbuffer->width || y >= textbuffer->height
      || x < 0 || y < 0)
    return;

  textbuffer->cursor_x = x;
  textbuffer->cursor_y = y;
  
  return;
}
void textbuffer_advance_cursor(textbuffer_t *textbuffer)
{
  if (!textbuffer || textbuffer->cursor_x+1 >= textbuffer->width ||
      textbuffer->cursor_y+1 >= textbuffer->height)
    return;

  textbuffer->cursor_x++;
  textbuffer->cursor_y++;
  
  return;
}

textbuffer_entry_t textbuffer_read_cursor(textbuffer_t *textbuffer)
{
  if (!textbuffer || textbuffer->cursor_x >= textbuffer->width
      || textbuffer->cursor_y >= textbuffer->height)
    goto exit;

  unsigned int index = textbuffer->cursor_y * textbuffer->width + textbuffer->cursor_x;
  return textbuffer->buff[index];
  
 exit:
  return (textbuffer_entry_t) {0};  
}
