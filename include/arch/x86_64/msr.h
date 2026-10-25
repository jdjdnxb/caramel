#pragma once

#include <stdint.h>

/*
 * Both rdmsr() and wrmsr() can raise a general protection fault for
 * invalid or innaccessible MSRs. They do not guarantee correct hardware 
 * memory ordering.
 */
static inline uint64_t rdmsr(uint32_t msr) 
{
    uint32_t lo, hi;
    __asm__ volatile ("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return ((uint64_t)hi << 32) | lo;
}

static inline void wrmsr(uint32_t msr, uint64_t value) 
{
    uint32_t lo = (uint32_t)value;
    uint32_t hi = (uint32_t)(value >> 32);
    __asm__ volatile ("wrmsr" : : "c"(msr), "a"(lo), "d"(hi) : "memory");
}

