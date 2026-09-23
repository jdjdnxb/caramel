#include <stdint.h>

#include <drivers/video/text.h>
#include <drivers/video/font.h>

#include <drivers/video/framebuffer.h>

static inline bool is_pixel_set(uint32_t row, uint32_t col)
{
    return row & (1 << (7 - col));
}

void text_render_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg) 
{
    uint8_t *glyph = font8x16[(uint8_t)c];
    for (uint32_t row = 0; row < FONT_HEIGHT; row++)
    {
        uint8_t current_row = glyph[row];
        for (uint32_t col = 0; col < FONT_WIDTH; col++)
        {
            uint32_t pixel = is_pixel_set(current_row, col) ? fg : bg;
            framebuffer_put_pixel(x + col, y + row, pixel);
        }
    }
}

void text_render_string(const char *s, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    while (*s)
    {
        text_render_char(*s, x, y, fg, bg);
        s++;
        x+=FONT_ADVANCE_X;
    }
}
