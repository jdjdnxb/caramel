#include <stdint.h>
#include <kprintf.h>

#include <arch/x86_64/gdt/gdt.h>

static struct x86_64_gdtr gdtr;
static struct x86_64_segment_descriptor gdt[256];
static struct x86_64_tss_descriptor tss_descriptor;
static struct x86_64_tss tss;

static void gdt_init_kernel_code(void)
{
    struct x86_64_segment_descriptor *entry = &gdt[1];

    entry->base_low     = 0;
    entry->base_mid     = 0;
    entry->base_high    = 0;
    entry->limit        = 0xFFFF;

    // Code segment for ring 0, executable, readable, present
    entry->access       = 0x9A;

    // G = 1, D/B = 0, L = 1, limit[19:16] = 0xF
    entry->flags_limit  = 0xFA;
}

static void gdt_init_kernel_data(void)
{
    struct x86_64_segment_descriptor *entry = &gdt[2];

    entry->base_low        = 0;
    entry->base_mid        = 0;
    entry->base_high       = 0;
    entry->limit           = 0xFFFF;

    // Data segment for ring 0, non-executable, readable, present
    entry->access          = 0x92;

    // G = 1, D/B = 0, L = 0, limit[19:16] = 0xF
    entry->flags_limit     = 0xFC;
}

static void gdt_init_tss(void)
{
    uint64_t tss_linear = (uint64_t)&tss;

    // Start at index 6 (gdt + 0x28) so we can keep the user info below later
    struct x86_64_tss_descriptor *entry = &tss_descriptor;

    entry->limit       = sizeof(tss) - 1;
    entry->base_low    = (uint16_t)(tss_linear & 0xFFFF);
    entry->base_mid    = (uint8_t )((tss_linear >> 16) & 0xFF);
    
    // System segment, 64 bit available TSS
    entry->access      = 0x89;
    entry->flags_limit = 0;

    entry->base_high   = (uint8_t )((tss_linear >> 24) & 0xFF);
    
    entry->base_upper  = (uint32_t)((tss_linear >> 32) & 0xFFFFFFFF);
    entry->reserved    = 0;

    *(struct x86_64_tss_descriptor*)&gdt[5] = tss_descriptor;
}

static inline void lgdt(struct x86_64_gdtr *gdtr)
{
    __asm__ volatile("lgdt %0" : : "m" (*gdtr));
}

static inline void ltr(uint16_t selector)
{
    __asm__ volatile("ltr %0" : : "r" (selector) : "memory");
}

void x86_64_gdt_init(void)
{
    gdt_init_kernel_code();
    gdt_init_kernel_data();
    gdt_init_tss();

    gdtr.size   = sizeof(gdt) - 1;
    gdtr.offset = (uint64_t)gdt;

    lgdt(&gdtr);
    kprintf("Loaded GDT successfully. We're no longer using Limine's GDT!\n");

    ltr(0x28);
    kprintf("Loaded task register.\n");
}
