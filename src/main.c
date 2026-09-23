#include <stdint.h>
#include <boot/requests.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/text.h>

void main(void)
{
    framebuffer_init();
    framebuffer_clear(0x00000000);

    while (1)
    {
        __asm__ volatile("hlt");
    }
}
