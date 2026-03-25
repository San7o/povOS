// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#ifndef POVOS_KERNEL_FRAMEBUFFER_H
#define POVOS_KERNEL_FRAMEBUFFER_H

//
// Framebuffer
// -----------
//
// Essentially a buffer of pixels.
//

#include <libk/stddef.h>

typedef struct framebuffer {
  u8_t* buff;
  u64_t width;
  u64_t height;
} framebuffer_t;

void framebuffer_init(framebuffer_t *fb, u8_t *buff, u64_t width, u64_t height);
void framebuffer_clear(framebuffer_t *fb, u8_t color);
void framebuffer_draw_pixel(framebuffer_t *fb, int x, int y, u8_t color);
void framebuffer_draw_rect(framebuffer_t *fb, int x, int y,
                           int h, int w, u8_t color);

#endif // POVOS_KERNEL_FRAMEBUFFER_H
