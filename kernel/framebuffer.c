// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <kernel/framebuffer.h>  // implements

void framebuffer_init(framebuffer_t *fb, u8_t *buff, u64_t width, u64_t height)
{
  if (!fb) return;

  fb->buff   = buff;
  fb->width  = width;
  fb->height = height;

  framebuffer_clear(fb, 0);
}

void framebuffer_clear(framebuffer_t *fb, u8_t color)
{
  if (!fb) return;

  for (u64_t x = 0; x < fb->width; ++x)
    for (u64_t y = 0; y < fb->height; ++y)
      fb->buff[y * fb->width + x] = color;
}

void framebuffer_draw_pixel(framebuffer_t *fb, u64_t x, u64_t y, u8_t color)
{
  if (!fb || x >= fb->width || y >= fb->height || x < 0 || y < 0)
    return;

  fb->buff[y * fb->width + x] = color;
}

void framebuffer_draw_rect(framebuffer_t *fb,
                           u64_t x, u64_t y,
                           int h, int w,
                           u8_t color)
{
  for (u64_t i = x; i < x + w; ++i)
    for (u64_t j = y; j < y + h; ++j)
      framebuffer_draw_pixel(fb, i, j, color);
}
