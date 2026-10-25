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

struct isc_interrupt_source_override
{
    struct   acpi_madt_entry_header header;
    uint8_t  bus;
    uint8_t  source;
    uint32_t gsi;
    uint8_t  flags[2];
}__attribute__((packed));

struct isc_nmi_source
{
    struct   acpi_madt_entry_header header;
    uint8_t  flags[2];
    uint32_t gsi;
}__attribute__((packed));

struct isc_lapic_nmi_source
{
    struct  acpi_madt_entry_header header;
    uint8_t acpi_processor_uid;
    uint8_t flags[2];
    uint8_t lapic_lint; // Local APIC interrupt input to which NMI is connected
}__attribute__((packed));
