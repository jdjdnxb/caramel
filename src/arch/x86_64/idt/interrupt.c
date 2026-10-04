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
        case X86_64_EXCEPTION_NON_MASKABLE_INTERRUPT:
            exception_non_maskable_interrupt(frame);
            break;
        case X86_64_EXCEPTION_OVERFLOW:
            exception_overflow(frame);
            break;
        case X86_64_EXCEPTION_INVALID_OPCODE:
            exception_invalid_opcode(frame);
            break;
        case X86_64_EXCEPTION_DEVICE_NOT_AVAILABLE:
            exception_device_not_available(frame);
            break;
        case X86_64_EXCEPTION_DOUBLE_FAULT:
            exception_double_fault(frame);
            break;
        case X86_64_EXCEPTION_INVALID_TSS:
            exception_invalid_tss(frame);
            break;
        case X86_64_EXCEPTION_GENERAL_PROTECTION:
            exception_general_protection(frame);
            break;
        case X86_64_EXCEPTION_PAGE_FAULT:
            exception_page_fault(frame);
            break;
        case X86_64_EXCEPTION_CONTROL_PROTECTION:
            exception_control_protection(frame);
            break;
        default:
            panic("Unregistered ISR", frame);
            panic_end();
    }
}
