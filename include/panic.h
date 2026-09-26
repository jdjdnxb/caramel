#pragma once

#include <stdarg.h>
#include <arch/x86_64/idt/idt.h>

void panic(const char *reason, struct interrupt_frame *frame, uint64_t error_code);

__attribute__((noreturn)) 
void panic_end(void);
