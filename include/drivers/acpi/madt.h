// Multiple APIC Description Table definitions
#pragma once

#include <drivers/acpi/sdt.h>

struct acpi_madt
{
    struct acpi_sdt_header header;      // Signature: "APIC"
    uint32_t lapic_address;
    uint32_t flags;
};

const struct acpi_madt *acpi_madt_find();
