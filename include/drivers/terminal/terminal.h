#pragma once

#define TERMINAL_MARGIN 2

struct terminal {
    uint32_t cursor_x;
    uint32_t cursor_y;

    uint64_t width;
    uint64_t height;

    uint32_t background;
    uint32_t foreground;
};

void terminal_set_cursor(struct terminal *terminal, uint32_t x, uint32_t y);
void terminal_clear(struct terminal *terminal);
void terminal_set_foreground(struct terminal *terminal, uint32_t foreground);
void terminal_set_background(struct terminal *terminal, uint32_t background);
void terminal_resize(struct terminal *terminal, uint64_t width, uint64_t height);

void terminal_init(struct terminal *terminal);

void terminal_newline(struct terminal *terminal);
void terminal_tab(struct terminal *terminal);
void terminal_put_char(char c, struct terminal *terminal);
void terminal_write(const char *s, struct terminal *terminal);
