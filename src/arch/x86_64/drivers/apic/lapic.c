#include <stdint.h>

#include <kprintf.h>

#include <drivers/acpi/isc.h>
#include <drivers/acpi/madt.h>

#include <arch/x86_64/drivers/apic/lapic.h>
#include <arch/x86_64/msr.h>

void lapic_init(void)
{
    // TODO: actually check if LAPIC is supported by given machine
    uint64_t apic_base = rdmsr(IA32_APIC_BASE);
    kprintf("[LAPIC] APIC Base: %llx\n", apic_base); 

    // Bits 31:12 hold the physical base address...
    uint64_t lapic_base = apic_base & 0xFFFFF000;
    kprintf("[LAPIC] LAPIC Base: %llx\n", lapic_base);
}
