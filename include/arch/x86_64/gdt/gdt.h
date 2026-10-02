#pragma once

#include <stdint.h>

struct x86_64_gdtr 
{
    uint16_t size;
    uint64_t offset;
}__attribute__((packed));

struct x86_64_segment_descriptor
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  flags_limit;       // The flags and the last 4 bits of the limit are kept in a single byte
    uint8_t  base_high;
}__attribute__((packed));

struct x86_64_tss_descriptor
{
    uint16_t limit;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  flags_limit;       // Same as for the segment descriptor
    uint8_t  base_high;
    uint32_t base_upper;
    uint32_t reserved;
}__attribute__((packed));

struct x86_64_tss
{
    uint32_t reserved_0;
    
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    
    uint64_t reserved_1;
    
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    
    uint32_t reserved_2;
    uint16_t reserved_3;
    
    uint16_t iopb;      // I/O Permission bitmap
}__attribute__((packed));

void x86_64_gdt_init(void);
