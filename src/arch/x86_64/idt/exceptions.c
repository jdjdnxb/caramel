#include <stdint.h>

#include <arch/x86_64/idt/idt.h>

#include <kprintf.h>
#include <panic.h>

void exception_divide_error(struct interrupt_frame *frame)
{
    panic("Exception: Divide Error.", frame);
    panic_end();
}

void exception_overflow(struct interrupt_frame *frame)
{
    panic("Exception: Overflow.", frame); 
    panic_end();
}

void exception_invalid_opcode(struct interrupt_frame *frame)
{
    panic("Exception: Invalid Opcode.", frame); 
    panic_end();
}

void exception_double_fault(struct interrupt_frame *frame)
{
    panic("Exception: Double Fault.", frame);
    panic_end();
}

void exception_general_protection(struct interrupt_frame *frame)
{
    panic("Exception: General Protection.", frame);
    panic_end();
}

void exception_page_fault(struct interrupt_frame *frame)
{
    panic("Exception: Page Fault.", frame);
    
    uint64_t cr0, cr2, cr3, cr4;
    __asm__ volatile("mov %%cr0, %0" : "=r" (cr0));
    __asm__ volatile("mov %%cr2, %0" : "=r" (cr2));
    __asm__ volatile("mov %%cr3, %0" : "=r" (cr3));
    __asm__ volatile("mov %%cr4, %0" : "=r" (cr4));

    kprintf("\nControl Registers:\n");
    kprintf("  CR0:    %016llx\n", cr0);
    kprintf("  CR2:    %016llx\n", cr2);
    kprintf("  CR3:    %016llx\n", cr3);
    kprintf("  CR4:    %016llx\n", cr4);
    kprintf("\n");

    panic_end();
}
