// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/tty.h>

void tty_init(tty_t *tty, textbuffer_t *textbuffer, console_t *console)
{
  if (!tty) return;

  tty->textbuffer = textbuffer;
  tty->console    = console;
  
  return;
}

void tty_write_input(tty_t *tty, input_event_t event)
{
  (void) tty;
  (void) event;
  
  // TODO
  return;
}

void tty_write(tty_t *tty, const char *buf, size_t len)
{
  (void) tty;
  (void) buf;
  (void) len;
  
  // TODO
  return;
}

void tty_flush(tty_t *tty)
{
  if (!tty || !tty->console) return;
  
  tty->console->draw(tty->textbuffer);
  
  return;
}
