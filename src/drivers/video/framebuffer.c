#include <stdint.h>

#include <drivers/video/framebuffer.h>
#include <boot/requests.h>

struct limine_framebuffer *limine_framebuffer;
struct framebuffer framebuffer;

void framebuffer_init(void)
{
    if (framebuffer_request.response == nullptr)
    {
        while (1)
        {
            __asm__ volatile("hlt");
        }
    }

    limine_framebuffer = framebuffer_request.response->framebuffers[0];
    
    framebuffer.address = limine_framebuffer->address;
    framebuffer.width   = limine_framebuffer->width;
    framebuffer.height  = limine_framebuffer->height;
    framebuffer.pitch   = limine_framebuffer->pitch;
    framebuffer.bpp     = limine_framebuffer->bpp;
}

void framebuffer_put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x > framebuffer.width || y > framebuffer.height)
    {
        return;
    }

    uintptr_t pixel = (uintptr_t)framebuffer.address + (uintptr_t)(y * framebuffer.pitch) + (uintptr_t)(x * ((framebuffer.bpp) / 8));
    *(uint32_t*)pixel = color;
}

void framebuffer_clear(uint32_t color)
{
    for (uint64_t y = 0; y < framebuffer.height; y++)
    {
        for (uint64_t x = 0; x < framebuffer.width; x++)
        {
            framebuffer_put_pixel(x, y, color);
        }
    }
}
