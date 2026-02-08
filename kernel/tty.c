// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/tty.h>
#include <drivers/video/vga.h>

void tty_init(tty_t *tty, textbuffer_t *textbuffer,
              textbuffer_style_t style, console_t *console)
{
  if (!tty) return;

  tty->textbuffer = textbuffer;
  tty->console    = console;
  tty->style      = style;
  
  return;
}

void tty_write_input(tty_t *tty, input_event_t event)
{

  if (!tty) return;

  if (event.type == INPUT_EVENT_TYPE_CHAR)
  {
    textbuffer_write_style(tty->textbuffer, event.e.c, tty->style);
    return;
  }

  switch (event.e.key.key)
  {
  case KEY_BACKSPACE:
    textbuffer_cursor_regress(tty->textbuffer);
    textbuffer_write_style(tty->textbuffer, ' ', tty->style);
    textbuffer_cursor_regress(tty->textbuffer);
    break;
  case KEY_ENTER:
    textbuffer_cursor_newline(tty->textbuffer);
    textbuffer_write_style(tty->textbuffer, '>', tty->style);
    textbuffer_write_style(tty->textbuffer, ' ', tty->style);
    break;
  default:
    break;
  }
  
  return;
}

void tty_write(tty_t *tty, const char *buf, size_t len)
{
  if (!tty || !buf) return;
  
  for (size_t i = 0; i < len; ++i)
  {
    char c = buf[i];
    switch (c)
    {
    case '\n':
      textbuffer_cursor_newline(tty->textbuffer);
      break;        
    case '\t':
      textbuffer_write_style(tty->textbuffer, ' ', tty->style);
      textbuffer_write_style(tty->textbuffer, ' ', tty->style);
      textbuffer_write_style(tty->textbuffer, ' ', tty->style);
      textbuffer_write_style(tty->textbuffer, ' ', tty->style);
      break;
    default:
      textbuffer_write_style(tty->textbuffer, c, tty->style);
      break;
    }
  }
  return;
}

void tty_flush(tty_t *tty)
{
  if (!tty || !tty->console || !tty->textbuffer) return;
  
  tty->console->draw(tty->textbuffer);
  tty->console->set_cursor(tty->textbuffer->cursor_x,
                           tty->textbuffer->cursor_y);
  
  return;
}
