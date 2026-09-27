#include <stdint.h>

#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/idt/exceptions.h>

#include <kprintf.h>

static struct x86_64_idtr idtr;
static struct x86_64_idt  idt[256];

static inline void lidt(struct x86_64_idtr *idtr)
{
    __asm__ volatile("lidt %0" : : "m" (*idtr));
}

// Gate type MUST be 0xE (interrupt) or 0xF (trap)
// DPL must be 0x3 (ring 3), 0x2 (ring 2), 0x1 (ring 1), 0x0 (ring 0). Hardware interrupts ignore the DPL
void x86_64_idt_install(uint8_t vector, uint8_t gate_type, uint8_t dpl, void *handler)
{
    uint64_t handler_address = (uint64_t)handler; 

    idt[vector].offset_low   = (uint16_t)(handler_address & 0xFFFF);              // Lower 16 bits 
    idt[vector].offset_mid   = (uint16_t)((handler_address >> 16) & 0xFFFF);      // Bits 15..31
    idt[vector].offset_high  = (uint32_t)((handler_address >> 32) & 0xFFFFFFFF);  // Higher 32 bits

    idt[vector].selector     = 0x28;
    idt[vector].ist          = 0x00;        // No IST for now
    
    // Bit 0..3 = gate type
    // Bit 5..6 = dpl
    // Bit 7    = present attribute
    idt[vector].attributes   = X86_64_IDT_ATTRIBUTE_PRESENT | X86_64_IDT_ATTRIBUTE_DPL(dpl) | X86_64_IDT_ATTRIBUTE_GATE_TYPE(gate_type); 

    idt[vector].reserved     = 0x00;

    kprintf("Installed handler with address %lx. Vector: %u, Gate Type: %X, DPL: %X.\n", handler_address, vector, gate_type, dpl);
}

static void exception_handlers_install(void)
{
    x86_64_idt_install(X86_64_EXCEPTION_DIVIDE_ERROR, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &exception_divide_error);
    x86_64_idt_install(X86_64_EXCEPTION_OVERFLOW, X86_64_GATE_TYPE_TRAP, DPL_RING_0, &exception_overflow);
    x86_64_idt_install(X86_64_EXCEPTION_INVALID_OPCODE, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &exception_invalid_opcode);
    x86_64_idt_install(X86_64_EXCEPTION_DOUBLE_FAULT, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &exception_double_fault);
    x86_64_idt_install(X86_64_EXCEPTION_GENERAL_PROTECTION, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &exception_general_protection);
    x86_64_idt_install(X86_64_EXCEPTION_PAGE_FAULT, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &exception_page_fault);
}

void x86_64_idt_init(void)
{
    exception_handlers_install();

    idtr.offset = (uint64_t)(idt);      // Linear address of the IDT must be 64 bits wide in long mode 
    idtr.size   = sizeof(idt) - 1;

    kprintf("IDTR Offset: %lx.\nIDTR Size: %u.\n", idtr.offset, idtr.size);

    lidt(&idtr);

    kprintf("Loaded IDT succesfully.\n");
}
