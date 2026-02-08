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

typedef struct textbuffer_style {
  textbuffer_color_t   foreground;
  textbuffer_color_t   background;  
} textbuffer_style_t;

#define TEXTBUFFER_STYLE_MAKE(fg, bg) \
  (textbuffer_style_t) { .foreground = fg, .background = bg }

#define TEXTBUFFER_STYLE_BW \
  TEXTBUFFER_STYLE_MAKE(VGA_COLOR_WHITE, VGA_COLOR_BLACK)

typedef struct textbuffer_entry {
  char                 c;
  textbuffer_style_t   style;
} textbuffer_entry_t;

typedef struct textbuffer {
  textbuffer_entry_t   *buff;
  unsigned int          width;
  unsigned int          height;

  unsigned int   cursor_x;
  unsigned int   cursor_y;
} textbuffer_t;

//
// Functions
//

// Does not allocate storage, [buff] must be big enough
void textbuffer_init(textbuffer_t *textbuffer,
                     textbuffer_entry_t *buff,
                     unsigned int width,
                     unsigned int height,
                     unsigned int cursor_x,
                     unsigned int cursor_y);

// Write at cursor position and advance cursor

void textbuffer_write(textbuffer_t *textbuffer,
                      char c,
                      textbuffer_color_t foreground,
                      textbuffer_color_t background);
void textbuffer_write_style(textbuffer_t *textbuffer,
                            char c,
                            textbuffer_style_t style);
void textbuffer_write_entry(textbuffer_t *textbuffer,
                            textbuffer_entry_t entry);

// Write at x and y position

void textbuffer_write_pos(textbuffer_t *textbuffer,
                          char c,
                          unsigned char foreground_color,
                          unsigned char background_color,
                          unsigned int x,
                          unsigned int y);
void textbuffer_write_pos_style(textbuffer_t *textbuffer,
                                char c,
                                textbuffer_style_t style,
                                unsigned int x,
                                unsigned int y);
void textbuffer_write_pos_entry(textbuffer_t *textbuffer,
                                textbuffer_entry_t entry,
                                unsigned int x,
                                unsigned int y);

textbuffer_entry_t textbuffer_read(textbuffer_t *textbuffer,
                                   unsigned int x, unsigned int y);

// Cursor

unsigned int textuffer_cursor_get_x(textbuffer_t *textbuffer);
unsigned int textuffer_cursor_get_y(textbuffer_t *textbuffer);
textbuffer_entry_t textbuffer_cursor_read(textbuffer_t *textbuffer);
void textbuffer_cursor_move(textbuffer_t *textbuffer,
                            unsigned int x, unsigned int y);
void textbuffer_cursor_advance(textbuffer_t *textbuffer);
void textbuffer_cursor_regress(textbuffer_t *textbuffer);
void textbuffer_cursor_newline(textbuffer_t *textbuffer);

#endif // POVOS_KERNEL_TEXTBUFFER_H
