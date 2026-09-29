#pragma once

#include <stdint.h>

#define X86_64_GATE_TYPE_INTERRUPT 0xE
#define X86_64_GATE_TYPE_TRAP      0xF

#define DPL_RING_3  0x3
#define DPL_RING_2  0x2
#define DPL_RING_1  0x1
#define DPL_RING_0  0x0

#define X86_64_IDT_ATTRIBUTE_PRESENT            (1 << 7)
#define X86_64_IDT_ATTRIBUTE_DPL(x)             (((x) & 0x3) << 5)
#define X86_64_IDT_ATTRIBUTE_GATE_TYPE(x)       ((x) & 0xF)

struct interrupt_frame {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    
    uint64_t vector;
    uint64_t error_code;
    
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

#define X86_64_EXCEPTION_DIVIDE_ERROR       0
#define X86_64_EXCEPTION_OVERFLOW           4
#define X86_64_EXCEPTION_INVALID_OPCODE     6
#define X86_64_EXCEPTION_DOUBLE_FAULT       8
#define X86_64_EXCEPTION_GENERAL_PROTECTION 13
#define X86_64_EXCEPTION_PAGE_FAULT         14

extern void isr_stub_0(void);
extern void isr_stub_4(void);
extern void isr_stub_6(void);
extern void isr_stub_8(void);
extern void isr_stub_13(void);
extern void isr_stub_14(void);
