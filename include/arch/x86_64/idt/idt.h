#pragma once

#include <stdint.h>

#define X86_64_GATE_TYPE_INTERRUPT 0xE
#define X86_64_GATE_TYPE_TRAP      0xF

#define DPL_RING_3  0x3
#define DPL_RING_2  0x2
#define DPL_RING_1  0x1
#define DPL_RING_0  0x0

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

struct x86_64_idtr {
    uint16_t size;
    uint64_t offset;
}__attribute__((packed));

struct x86_64_idt {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;           // Only bits 0..2 are used, they hold the IST offset
    uint8_t  attributes;    // Gate type, DPL and p fields
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
}__attribute__((packed));

void x86_64_idt_init(void);
void x86_64_idt_install(uint8_t vector, uint8_t gate_type, uint8_t dpl, void *handler);
