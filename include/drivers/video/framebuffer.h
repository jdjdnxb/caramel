#pragma once

#include <stdint.h>

struct framebuffer {
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
};

void framebuffer_init(void);
void framebuffer_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void framebuffer_clear(uint32_t color);

extern struct framebuffer framebuffer;
