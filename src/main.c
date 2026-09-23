#include <stdint.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/text.h>
#include <drivers/terminal/terminal.h>

struct terminal kernel_terminal;

void main(void)
{
    framebuffer_init();
    framebuffer_clear(0x00000000);

    terminal_init(&kernel_terminal);        

    terminal_write("Welcome to caramel!\n", &kernel_terminal);

    while (1)
    {
        __asm__ volatile("hlt");
    }
}
