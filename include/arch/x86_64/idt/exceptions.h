#pragma once

#include <arch/x86_64/idt/idt.h>

__attribute__((interrupt))
void exception_divide_error(struct interrupt_frame *frame);

__attribute__((interrupt))
void exception_overflow(struct interrupt_frame *frame);

__attribute__((interrupt))
void exception_invalid_opcode(struct interrupt_frame *frame);

__attribute__((interrupt))
void exception_double_fault(struct interrupt_frame *frame, uint64_t error_code);

__attribute__((interrupt))
void exception_general_protection(struct interrupt_frame *frame, uint64_t error_code);

__attribute__((interrupt))
void exception_page_fault(struct interrupt_frame *frame, uint64_t error_code);
