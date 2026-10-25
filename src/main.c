#include <stdint.h>
#include <boot/requests.h>

#include <drivers/video/framebuffer.h>
#include <drivers/video/text.h>
#include <drivers/terminal/terminal.h>
#include <drivers/acpi/acpi.h>

#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/gdt/gdt.h>
#include <arch/x86_64/drivers/apic/lapic.h>

#include <kprintf.h>

struct terminal kernel_terminal;

void main(void)
{
    framebuffer_init();
    framebuffer_clear(0x00000000);
    terminal_init(&kernel_terminal);

    x86_64_gdt_init();
    x86_64_idt_init();

    acpi_init();
    lapic_init();

    kprintf("APIC work paused until memory management is implemented...\n");
    kprintf("Welcome to caramel!\n");
    
    while (1)
    {
        __asm__ volatile("hlt");
    }
}
