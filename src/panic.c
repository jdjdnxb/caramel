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

void panic(const char *reason, struct interrupt_frame *frame)
{
    terminal_clear(&kernel_terminal);

    kprintf("Kernel Panic!!!\n");
    kprintf("Reason: %s\n\n", reason);

    if (frame)
    {
        kprintf("General Purpose Registers:\n");
        kprintf("  RAX: %016llx    RBX: %016llx\n", frame->rax, frame->rbx);
        kprintf("  RCX: %016llx    RDX: %016llx\n", frame->rcx, frame->rdx);
        kprintf("  RSI: %016llx    RDI: %016llx\n", frame->rsi, frame->rdi);
        kprintf("  RBP: %016llx    RSP: %016llx\n", frame->rbp, frame->rsp);
        kprintf("  R8:  %016llx    R9:  %016llx\n", frame->r8, frame->r9);
        kprintf("  R10: %016llx    R11: %016llx\n", frame->r10, frame->r11);
        kprintf("  R12: %016llx    R13: %016llx\n", frame->r12, frame->r13);
        kprintf("  R14: %016llx    R15: %016llx\n", frame->r14, frame->r15);

        kprintf("\n");
        kprintf("Interrupt Frame:\n");
        kprintf("  Vector:     %llu\n", frame->vector);
        kprintf("  Error Code: %016llx\n", frame->error_code);
        kprintf("  RIP:        %016llx\n", frame->rip);
        kprintf("  CS:         %016llx\n", frame->cs);
        kprintf("  RFLAGS:     %016llx\n", frame->rflags);
        kprintf("  RSP:        %016llx\n", frame->rsp);
        kprintf("  SS:         %016llx\n", frame->ss);
    }
    else 
    {
        kprintf("  RIP:    %016llx\n", get_rip());
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
