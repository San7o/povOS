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

struct framebuffer {
  u8_t* buff;
  u64_t width;
  u64_t height;
};

void framebuffer_init(struct framebuffer *fb, u8_t *buff, u64_t width, u64_t height);
void framebuffer_clear(struct framebuffer *fb, u8_t color);
void framebuffer_draw_pixel(struct framebuffer *fb, u64_t x, u64_t y, u8_t color);
void framebuffer_draw_rect(struct framebuffer *fb, u64_t x, u64_t y,
                           int h, int w, u8_t color);

#endif // POVOS_KERNEL_FRAMEBUFFER_H
