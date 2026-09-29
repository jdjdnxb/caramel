#include <stdint.h>

#include <arch/x86_64/idt/exceptions.h>
#include <arch/x86_64/idt/interrupt.h>
#include <arch/x86_64/idt/idt.h>

#include <kprintf.h>
#include <panic.h>

void x86_64_idt_interrupt_dispatch(struct interrupt_frame *frame)
{
    switch (frame->vector)
    {
        case X86_64_EXCEPTION_DIVIDE_ERROR:
            exception_divide_error(frame);
            break;
        case X86_64_EXCEPTION_OVERFLOW:
            exception_overflow(frame);
            break;
        case X86_64_EXCEPTION_INVALID_OPCODE:
            exception_invalid_opcode(frame);
            break;
        case X86_64_EXCEPTION_DOUBLE_FAULT:
            exception_double_fault(frame);
            break;
        case X86_64_EXCEPTION_GENERAL_PROTECTION:
            exception_general_protection(frame);
            break;
        case X86_64_EXCEPTION_PAGE_FAULT:
            exception_page_fault(frame);
            break;
        default:
            panic("Unregistered ISR", frame);
            panic_end();
    }
}
