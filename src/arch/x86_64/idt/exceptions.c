#include <stdint.h>

#include <arch/x86_64/idt/idt.h>

#include <kprintf.h>
#include <panic.h>

__attribute__((interrupt))
void exception_divide_error(struct interrupt_frame *frame)
{
    (void)frame;
    panic("Exception: Divide Error.", nullptr, 0);
    panic_end();
}

__attribute__((interrupt))
void exception_overflow(struct interrupt_frame *frame)
{
    (void)frame;
    panic("Exception: Overflow.", nullptr, 0);
    panic_end();
}

__attribute__((interrupt))
void exception_invalid_opcode(struct interrupt_frame *frame)
{
    (void)frame;
    panic("Exception: Invalid Opcode.", nullptr, 0);
    panic_end();
}

__attribute__((interrupt))
void exception_double_fault(struct interrupt_frame *frame, uint64_t error_code)
{
    panic("Exception: Double Fault.", frame, error_code);
    panic_end();
}

__attribute__((interrupt))
void exception_general_protection(struct interrupt_frame *frame, uint64_t error_code)
{
    panic("Exception: General Protection.", frame, error_code);
    panic_end();
}

__attribute__((interrupt))
void exception_page_fault(struct interrupt_frame *frame, uint64_t error_code)
{
    panic("Exception: Page Fault.", frame, error_code);
    
    uint64_t cr2;
    __asm__ volatile("mov %%cr2, %0" : "=r" (cr2) : :);
    
    kprintf("CR2:    %x\n", cr2);

    panic_end();
}
