#include <stdint.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/text.h>
#include <drivers/terminal/terminal.h>
#include <arch/x86_64/idt/idt.h>

#include <kprintf.h>

struct terminal kernel_terminal;

void main(void)
{
    framebuffer_init();
    framebuffer_clear(0x00000000);
    terminal_init(&kernel_terminal);

    x86_64_idt_init();
    
    kprintf("Welcome to caramel!");

    while (1)
    {
        __asm__ volatile("hlt");
    }
}
