// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_TEXTBUFFER_H
#define POVOS_KERNEL_TEXTBUFFER_H

//
// Textbuffer
// ----------
//
// Simple abstraction over buffers of chars.
//

#include <libk/stddef.h>

typedef u8_t textbuffer_color_t;

struct textbuffer_style {
  textbuffer_color_t  foreground;
  textbuffer_color_t  background;  
};

#define TEXTBUFFER_STYLE_MAKE(fg, bg) \
  (struct textbuffer_style) { .foreground = fg, .background = bg }

#define TEXTBUFFER_STYLE_BW \
  TEXTBUFFER_STYLE_MAKE(VGA_COLOR_WHITE, VGA_COLOR_BLACK)

struct textbuffer_entry {
  struct textbuffer_style  style;
  char  c;
};

#define TEXTBUFFER_ENTRY_MAKE(_c, _style) \
  (struct textbuffer_entry) { .style = _style, .c = _c }

struct textbuffer {
  struct textbuffer_entry *buff;
  unsigned int  width;
  unsigned int  height;

  unsigned int  cursor_x;
  unsigned int  cursor_y;
};

//
// Functions
//

// Does not allocate storage, [buff] must be big enough
void textbuffer_init(struct textbuffer *textbuffer,
                     struct textbuffer_entry *buff,
                     unsigned int width,
                     unsigned int height,
                     unsigned int cursor_x,
                     unsigned int cursor_y);

// Write at cursor position and advance cursor
void textbuffer_write(struct textbuffer *textbuffer,
                      struct textbuffer_entry entry);
// Write at x and y position
void textbuffer_write_pos(struct textbuffer *textbuffer,
                          struct textbuffer_entry entry,
                          unsigned int x,
                          unsigned int y);

struct textbuffer_entry
textbuffer_read(struct textbuffer *textbuffer,
                unsigned int x, unsigned int y);

// Cursor

unsigned int textuffer_cursor_get_x(struct textbuffer *textbuffer);
unsigned int textuffer_cursor_get_y(struct textbuffer *textbuffer);
struct textbuffer_entry textbuffer_cursor_read(struct textbuffer *textbuffer);
void textbuffer_cursor_move(struct textbuffer *textbuffer,
                            unsigned int x, unsigned int y);
void textbuffer_cursor_advance(struct textbuffer *textbuffer);
void textbuffer_cursor_regress(struct textbuffer *textbuffer);
void textbuffer_cursor_newline(struct textbuffer *textbuffer);

#endif // POVOS_KERNEL_TEXTBUFFER_H
