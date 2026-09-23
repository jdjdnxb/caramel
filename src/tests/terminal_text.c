#include <stdint.h>
#include <tests/terminal_text.h>
#include <drivers/terminal/terminal.h>

extern struct terminal kernel_terminal;

void test_terminal_text(void)
{
    terminal_put_char('A', &kernel_terminal);
    terminal_put_char('B', &kernel_terminal);
    terminal_put_char('C', &kernel_terminal);
}

void test_terminal_backspace(void)
{
    terminal_put_char('A', &kernel_terminal);
    terminal_put_char('B', &kernel_terminal);
    terminal_put_char('C', &kernel_terminal);
    terminal_put_char('\b', &kernel_terminal);
    terminal_put_char('X', &kernel_terminal);
}

void test_terminal_space(void)
{
    terminal_put_char(' ', &kernel_terminal);
}
