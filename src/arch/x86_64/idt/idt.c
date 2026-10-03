#include <stdint.h>

#include <arch/x86_64/gdt/gdt.h>
#include <arch/x86_64/idt/idt.h>
#include <arch/x86_64/idt/exceptions.h>

#include <panic.h>
#include <kprintf.h>

static struct x86_64_idtr       idtr;
static struct x86_64_idt_entry  idt[256];

static inline void lidt(struct x86_64_idtr *idtr)
{
    __asm__ volatile("lidt %0" : : "m" (*idtr));
}

// Gate type MUST be 0xE (interrupt) or 0xF (trap)
// DPL must be 0x3 (ring 3), 0x2 (ring 2), 0x1 (ring 1), 0x0 (ring 0). Hardware interrupts ignore the DPL.
// If the IST index is zero, a modified version of the legacy stack-switching mechanism is used.
void x86_64_idt_install(uint8_t vector, uint8_t ist_index, uint8_t gate_type, uint8_t dpl, void *handler)
{
    uint64_t handler_address = (uint64_t)handler; 
    
    struct x86_64_idt_entry *entry = &idt[vector];

    entry->offset_low   = (uint16_t)(handler_address & 0xFFFF);              // Lower 16 bits 
    entry->offset_mid   = (uint16_t)((handler_address >> 16) & 0xFFFF);      // Bits 15..31
    entry->offset_high  = (uint32_t)((handler_address >> 32) & 0xFFFFFFFF);  // Higher 32 bits

    entry->selector     = X86_64_KERNEL_CODE_SELECTOR;
    entry->ist          = ist_index;

    // Bits 0..3 = gate type
    // Bits 5..6 = dpl
    // Bit 7    = present attribute
    entry->attributes   = X86_64_IDT_ATTRIBUTE_PRESENT | X86_64_IDT_ATTRIBUTE_DPL(dpl) | X86_64_IDT_ATTRIBUTE_GATE_TYPE(gate_type); 

    entry->reserved     = 0x00;

    kprintf("Installed handler with address %llx. Vector: %u, IST: %u, Gate Type: %llx, DPL: %llx.\n", handler_address, vector, ist_index, gate_type, dpl);
}

static void exception_handlers_install(void)
{
    x86_64_idt_install(X86_64_EXCEPTION_DIVIDE_ERROR, 0, X86_64_GATE_TYPE_INTERRUPT, X86_64_IDT_DPL_RING_0, isr_stub_0);
    x86_64_idt_install(X86_64_EXCEPTION_OVERFLOW, 0, X86_64_GATE_TYPE_TRAP, X86_64_IDT_DPL_RING_0, isr_stub_4);
    x86_64_idt_install(X86_64_EXCEPTION_INVALID_OPCODE, 0, X86_64_GATE_TYPE_INTERRUPT, X86_64_IDT_DPL_RING_0, isr_stub_6);
    x86_64_idt_install(X86_64_EXCEPTION_DOUBLE_FAULT, 1, X86_64_GATE_TYPE_INTERRUPT, X86_64_IDT_DPL_RING_0, isr_stub_8);
    x86_64_idt_install(X86_64_EXCEPTION_GENERAL_PROTECTION, 0, X86_64_GATE_TYPE_INTERRUPT, X86_64_IDT_DPL_RING_0, isr_stub_13);
    x86_64_idt_install(X86_64_EXCEPTION_PAGE_FAULT, 0, X86_64_GATE_TYPE_INTERRUPT, X86_64_IDT_DPL_RING_0, isr_stub_14); 
}

void x86_64_idt_init(void)
{
    exception_handlers_install();

    idtr.offset = (uint64_t)(idt);      // Linear address of the IDT must be 64 bits wide in long mode 
    idtr.size   = sizeof(idt) - 1;

    kprintf("IDTR Offset: %llx.\nIDTR Size: %llu.\n", idtr.offset, idtr.size);

    lidt(&idtr);

    kprintf("Loaded IDT succesfully.\n");
}
