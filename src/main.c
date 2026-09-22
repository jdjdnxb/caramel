#include <stdint.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>

void main(void)
{
    framebuffer_init();

    framebuffer_clear(0xffff0000);

    while (1)
    {
        __asm__ volatile("hlt");
    }
}
