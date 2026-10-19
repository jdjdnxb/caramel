#include <stdint.h>

#include <kprintf.h>

#include <drivers/acpi/acpi.h>
#include <drivers/acpi/madt.h>
#include <drivers/acpi/sdt.h>

const struct acpi_madt *acpi_madt_find()
{
    const struct acpi_madt *madt = (struct acpi_madt *)acpi_sdt_find("APIC");

    if (!madt)
    {
        kprintf("[ACPI] MADT not found!\n");
        return nullptr;
    }
}
