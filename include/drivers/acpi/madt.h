// Multiple APIC Description Table definitions
#pragma once

#include <drivers/acpi/sdt.h>

// Interrupt Controller Structure Types
#define ISC_PROCESSOR_LOCAL_APIC        0
#define ISC_IO_APIC                     1
#define ISC_INTERRUPT_SOURCE_OVERRIDE   2
#define ISC_NMI_SOURCE                  3
#define ISC_LOCAL_APIC_NMI              4
#define ISC_LOCAL_APIC_ADDRESS_OVERRIDE 5

struct acpi_madt_entry_header
{
    uint8_t type;       // Interrupt Controller Structure type
    uint8_t length;     // Length of entry in bytes
};

struct acpi_madt
{
    struct   acpi_sdt_header header;      // Signature: "APIC"
    uint32_t lic_address;                 // Local Interrupt Controller address
    uint32_t flags;

    struct acpi_madt_entry_header entries[];
};

const struct acpi_madt *acpi_madt_find(void);
struct acpi_madt_entry_header *acpi_madt_next_entry(const struct acpi_madt *madt, const struct acpi_madt_entry_header *current);
