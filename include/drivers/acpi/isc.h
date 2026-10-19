// Interrupt Controller Structures
#pragma once

#include <stdint.h>
#include <drivers/acpi/madt.h>

struct isc_processor_local_apic
{
    struct   acpi_madt_entry_header header;
    uint8_t  acpi_processor_uid;
    uint8_t  apic_id;
    uint32_t flags;
}__attribute__((packed));

struct isc_io_apic
{
    struct   acpi_madt_entry_header header;
    uint8_t  io_apic_id;
    uint8_t  reserved;
    uint32_t io_apic_address;
    uint32_t gsi_base;          // Global System Interrupt base
}__attribute__((packed));
