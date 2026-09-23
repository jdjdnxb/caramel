#pragma once

#include <stdint.h>

void text_render_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);
void text_render_string(const char *s, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);
