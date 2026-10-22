#include <stdint.h>

#include <kprintf.h>

#include <drivers/acpi/acpi.h>
#include <drivers/acpi/madt.h>
#include <drivers/acpi/sdt.h>
#include <drivers/acpi/isc.h>

const struct acpi_madt *acpi_madt_find(void)
{
    const struct acpi_madt *madt = (struct acpi_madt *)acpi_sdt_find("APIC");

    if (!madt)
    {
        kprintf("[ACPI] MADT not found!\n");
        return nullptr;
    }

    kprintf("[ACPI] MADT found at %llx.\n", madt);
    return madt;
}

struct acpi_madt_entry_header *acpi_madt_next_entry(const struct acpi_madt *madt, const struct acpi_madt_entry_header *current)
{
    if (!madt)
    {
        return nullptr;
    }

    uintptr_t madt_start = (uintptr_t)madt;
    uintptr_t madt_end   = madt_start + madt->header.length;
    uintptr_t madt_entries_start = (uintptr_t)madt->entries;
    uintptr_t next_entry_address;

    if (!current)
    {
        next_entry_address = madt_entries_start;
    }
    else
    {
        if (current->length < sizeof(struct acpi_madt_entry_header) || (uintptr_t)current + (uintptr_t)current->length > madt_end)
        {
            return nullptr;
        }

        next_entry_address = (uintptr_t)current + current->length;
    }

    if (next_entry_address >= madt_end || next_entry_address + sizeof(struct acpi_madt_entry_header) > madt_end)
    {
        return nullptr;
    }

    struct acpi_madt_entry_header *next_entry = (struct acpi_madt_entry_header *)next_entry_address;

    if (next_entry->length < sizeof(struct acpi_madt_entry_header) || next_entry_address + next_entry->length > madt_end)
    {
        return nullptr;
    }

    return next_entry;
}
