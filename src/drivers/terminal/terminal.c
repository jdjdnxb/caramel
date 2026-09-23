#include <stdint.h>

#include <drivers/video/text.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/font.h>

#include <drivers/terminal/terminal.h>

#define COLOR_WHITE 0xFFFFFFFFu
#define COLOR_BLACK 0x00000000u

void terminal_set_cursor(struct terminal *terminal, uint32_t x, uint32_t y)
{
    terminal->cursor_x = x;
    terminal->cursor_y = y;
}

void terminal_clear(struct terminal *terminal)
{
    framebuffer_clear(terminal->background);
    terminal_set_cursor(terminal, TERMINAL_MARGIN, TERMINAL_MARGIN);
}

void terminal_set_foreground(struct terminal *terminal, uint32_t foreground)
{
    terminal->foreground = foreground;
}

void terminal_set_background(struct terminal *terminal, uint32_t background)
{
    terminal->background = background;
}

void terminal_resize(struct terminal *terminal, uint64_t width, uint64_t height)
{
    terminal->width = width;
    terminal->height = height;
}

void terminal_init(struct terminal *terminal)
{
    terminal_resize(terminal, framebuffer.width, framebuffer.height);
    terminal_set_cursor(terminal, TERMINAL_MARGIN, TERMINAL_MARGIN);
    
    terminal_set_foreground(terminal, COLOR_WHITE);
    terminal_set_background(terminal, COLOR_BLACK);

    terminal_clear(terminal);
}

void terminal_newline(struct terminal *terminal)
{
    uint32_t next_y = terminal->cursor_y + FONT_ADVANCE_Y;

    if (next_y + FONT_ADVANCE_Y > terminal->height) 
    {
        // TODO: implement terminal_scroll()     
    }
    else 
    {
        terminal_set_cursor(terminal, TERMINAL_MARGIN, next_y);
    }
}

void terminal_tab(struct terminal *terminal)
{
    // The tab size is measured in spaces, so multiply it by the width to get the actual length
    uint32_t target_x = terminal->cursor_x + (TAB_SIZE * FONT_WIDTH);
    if (target_x > terminal->width)
    {
        terminal_newline(terminal);
    } 
    else 
    {
        terminal_set_cursor(terminal, target_x, terminal->cursor_y);
    }
}

void terminal_backspace(struct terminal *terminal)
{
    if (terminal->cursor_x - FONT_ADVANCE_X < TERMINAL_MARGIN)
    {
        // if cursor at line beginning
        if (terminal->cursor_y - FONT_ADVANCE_Y >= TERMINAL_MARGIN)
        {
            terminal_set_cursor(terminal, terminal->width - FONT_ADVANCE_X, terminal->cursor_y - FONT_ADVANCE_Y);
        }
    }
    else
    {
        // normal backspace
        terminal_set_cursor(terminal, terminal->cursor_x - FONT_ADVANCE_X, terminal->cursor_y);
    }
}

void terminal_scroll(struct terminal *terminal)
{
    // WIP
}

void terminal_put_char(char c, struct terminal *terminal)
{
    switch (c)
    {
        case '\n':
            terminal_newline(terminal);
            break;
        case '\t':
            terminal_tab(terminal);
            break;
        case '\b':
            terminal_backspace(terminal);
            break;
        default:
            if (terminal->cursor_x + FONT_ADVANCE_X > terminal->width)
            {
                terminal_newline(terminal);
            }
            text_render_char(c, terminal->cursor_x, terminal->cursor_y, terminal->foreground, terminal->background);
            terminal_set_cursor(terminal, terminal->cursor_x + FONT_ADVANCE_X, terminal->cursor_y);
    }
}

void terminal_write(const char *str, struct terminal *terminal)
{
    while (*str)
    {
        terminal_put_char(*str, terminal);
        str++;
    }
}
