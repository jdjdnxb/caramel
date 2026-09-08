#include <stdint.h>
#include <requests.h>

void main(void)
{
    while (1)
    {
        __asm__ volatile("hlt");
    }
}
