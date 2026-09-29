#pragma once

#include <arch/x86_64/idt/idt.h>

void x86_64_idt_interrupt_dispatch(struct interrupt_frame *frame);

extern void isr_stub_0(void);
extern void isr_stub_4(void);
extern void isr_stub_6(void);

extern void isr_stub_8(void);
extern void isr_stub_13(void);
extern void isr_stub_14(void);
