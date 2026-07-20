// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <drivers/video/vga.h>
#include <kernel/tty.h>

void tty_init(struct tty *tty, struct textbuffer *textbuffer,
              struct textbuffer_style style, struct console *console)
{
  if (!tty)
    return;

  tty->textbuffer = textbuffer;
  tty->console    = console;
  tty->style      = style;

  return;
}

static inline void tty_handle_char(struct tty *tty, char c)
{
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE(c, tty->style));
}

static inline void tty_handle_backspace(struct tty *tty)
{
  textbuffer_cursor_regress(tty->textbuffer);
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE(' ', tty->style));
  textbuffer_cursor_regress(tty->textbuffer);
}

static inline void tty_handle_enter(struct tty *tty)
{
  textbuffer_cursor_newline(tty->textbuffer);
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE('>', tty->style));
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE(' ', tty->style));
}

void tty_write_input(struct tty *tty, struct input_event event)
{
  if (!tty)
    return;

  if (event.type == INPUT_EVENT_TYPE_CHAR) {
    tty_handle_char(tty, event.e.c);
    return;
  }

  switch (event.e.key.key)
  {
  case KEY_BACKSPACE:
    tty_handle_backspace(tty);
    break;
  case KEY_ENTER:
    tty_handle_enter(tty);
    break;
  default:
    break;
  }

  return;
}

static inline void tty_write_space(struct tty *tty)
{
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE(' ', tty->style));
}

static inline void tty_write_char(struct tty *tty, char c)
{
  textbuffer_write(tty->textbuffer, TEXTBUFFER_ENTRY_MAKE(c, tty->style));
}

void tty_write(struct tty *tty, const char *buf, size_t len)
{
  if (!tty || !buf)
    return;

  for (size_t i = 0; i < len; ++i)
  {
    char c = buf[i];
    switch (c)
    {
    case '\n':
      textbuffer_cursor_newline(tty->textbuffer);
      break;
    case '\t':
      tty_write_space(tty);
      break;
    default:
      tty_write_char(tty, c);
      break;
    }
  }
  return;
}

void tty_flush(struct tty *tty)
{
  if (!tty || !tty->console || !tty->textbuffer)
    return;

  tty->console->draw(tty->textbuffer);
  tty->console->set_cursor(tty->textbuffer->cursor_x,
                           tty->textbuffer->cursor_y);

  return;
}
