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

typedef struct textbuffer_style {
  unsigned char   foreground_color;
  unsigned char   background_color;  
} textbuffer_style_t;

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

void textbuffer_write(textbuffer_t *textbuffer,
                      char c,
                      unsigned char foreground_color,
                      unsigned char background_color,
                      unsigned int x,
                      unsigned int y);
void textbuffer_write_style(textbuffer_t *textbuffer,
                            char c,
                            textbuffer_style_t style,
                            unsigned int x,
                            unsigned int y);
void textbuffer_write_entry(textbuffer_t *textbuffer,
                            textbuffer_entry_t entry,
                            unsigned int x,
                            unsigned int y);

textbuffer_entry_t textbuffer_read(textbuffer_t *textbuffer,
                                   unsigned int x, unsigned int y);

unsigned int textuffer_get_cursor_x(textbuffer_t *textbuffer);
unsigned int textuffer_get_cursor_y(textbuffer_t *textbuffer);
void textbuffer_move_cursor(textbuffer_t *textbuffer,
                            unsigned int x, unsigned int y);
void textbuffer_advance_cursor(textbuffer_t *textbuffer);
textbuffer_entry_t textbuffer_read_cursor(textbuffer_t *textbuffer);

#endif // POVOS_KERNEL_TEXTBUFFER_H
