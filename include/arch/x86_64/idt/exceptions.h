#pragma once

#include <arch/x86_64/idt/idt.h>

void exception_divide_error(struct interrupt_frame *frame);
void exception_non_maskable_interrupt(struct interrupt_frame *frame);
void exception_overflow(struct interrupt_frame *frame);
void exception_invalid_opcode(struct interrupt_frame *frame);
void exception_device_not_available(struct interrupt_frame *frame);
void exception_double_fault(struct interrupt_frame *frame);
void exception_invalid_tss(struct interrupt_frame *frame);
void exception_general_protection(struct interrupt_frame *frame);
void exception_page_fault(struct interrupt_frame *frame);
void exception_control_protection(struct interrupt_frame *frame);
