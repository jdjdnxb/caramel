#pragma once

#include <stdint.h>
#include <stdarg.h>

void kvprintf(const char* fmt, va_list args);
void kprintf(const char* fmt, ...);

enum {
    PRINTF_STATE_START,
    PRINTF_STATE_LENGTH,
    PRINTF_STATE_SHORT,
    PRINTF_STATE_LONG,
    PRINTF_STATE_SPEC,
    PRINTF_STATE_WIDTH
};

#define PRINTF_LENGTH_START 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4
#define PRINTF_HEX_LOW 5
#define PRINTF_HEX_UPPER 6
