#include <stdint.h>

#include <arch/x86_64/idt/idt.h>

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

    idt[vector].offset_low   = (uint16_t)(handler_address & 0xFFFF);             // Lower 16 bits 
    idt[vector].offset_mid   = (uint16_t)((handler_address >> 16) & 0xFFFF);      // Bits 15..31
    idt[vector].offset_high  = (uint32_t)((handler_address >> 32) & 0xFFFFFFFF);  // Higher 32 bits

    idt[vector].selector     = 0x28;
    idt[vector].ist          = 0x00;        // No IST for now
    
    // Bit 0..3 = gate type
    // Bit 5..6 = dpl
    // Bit 7    = present attribute
    idt[vector].attributes   = (0 << 7) | ((dpl & 0x3) < 5) | (gate_type & 0xF); 

    idt[vector].reserved     = 0x00;
}

__attribute__((interrupt))
void test(struct interrupt_frame *frame)
{
    (void)frame;
    while(1) { __asm__ volatile ("hlt"); }
}

void x86_64_idt_init(void)
{
    x86_64_idt_install(3, X86_64_GATE_TYPE_INTERRUPT, DPL_RING_0, &test);

    idtr.offset = (uint64_t)(idt);      // Linear address of the IDT must be 64 bits wide in long mode 
    idtr.size   = sizeof(idt) - 1;

    lidt(&idtr);     
}
