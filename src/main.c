#include <stdint.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/text.h>
#include <drivers/terminal/terminal.h>
#include <tests/terminal_text.h>

struct terminal kernel_terminal;

void main(void)
{
    framebuffer_init();
    framebuffer_clear(0x00000000);

    terminal_init(&kernel_terminal);        

    terminal_write("Welcome to caramel!\n", &kernel_terminal);


    test_terminal_newline();
    test_terminal_newline();

    
    // tests
    test_terminal_text();
    test_terminal_space();
    test_terminal_newline();
    test_terminal_backspace();
    test_terminal_scroll();


    while (1)
    {
        __asm__ volatile("hlt");
    }
}
