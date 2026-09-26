#include <stdarg.h>

#include <arch/x86_64/idt/idt.h>

#include <drivers/terminal/terminal.h>
#include <kprintf.h>

extern struct terminal kernel_terminal;

static uint64_t get_rip(void)
{
    uint64_t rip;
    __asm__ ("lea (%%rip), %0" : "=r" (rip));
    return rip;
}

void panic(const char *reason, struct interrupt_frame *frame, uint64_t error_code)
{
    terminal_set_foreground(&kernel_terminal, 0xFFFF0000);
    terminal_set_background(&kernel_terminal, 0xFFFFFFFF);

    terminal_clear(&kernel_terminal);

    kprintf("Kernel Panic!!!\n");
    kprintf("Reason: %s\n\n", reason);

    if (frame)
    {
        kprintf("Error Code: %llx\n", error_code);

        kprintf("RIP:    %x\n", frame->rip);
        kprintf("CS:     %x\n", frame->cs);
        kprintf("RFLAGS: %x\n", frame->rflags);
        kprintf("RSP:    %x\n", frame->rsp);
        kprintf("SS:     %x\n", frame->ss);
    }
    else 
    {
        kprintf("RIP:    %x\n", get_rip());
    }
}

__attribute__((noreturn))
void panic_end(void)
{
    kprintf("\nSystem halted.\n");
    while (1)
    {
        __asm__ volatile("hlt");
    }
}
