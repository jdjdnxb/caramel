// Disclaimer: this code is trash and should be expected to change.
#include <stdint.h>

#include <drivers/acpi/acpi.h>
#include <drivers/acpi/sdt.h>

bool acpi_sdt_validate(const struct acpi_sdt_header *header)
{
    if (!header)
    {
        return false;
    }

    if (header->length < sizeof(struct acpi_sdt_header))
    {
        return false;
    }
   
    if (!acpi_checksum_valid(header, header->length))
    {
        return false;
    }

    return true;
}

// A more professional project wouldn't do this, but fuck it, we are not a professional project.
// We don't want to add an RSDT/XSDT check in every function requiring *acpi_sdt_find(), so
// we're keeping a root table  in this file's scope. If this function fails, everything related 
// to SDTs is automatically unavailable.
static const struct acpi_sdt_header *root_table;

bool acpi_sdt_init(uintptr_t root_address)
{
    root_table = nullptr;
    if (!root_address)
    {
        root_table = nullptr;
        return false;
    }
    const struct acpi_sdt_header *root = (struct acpi_sdt_header *)root_address;
    if (acpi_signature_equal(root->signature, "RSDT", 4))
    {
        const struct acpi_rsdt *rsdt = (struct acpi_rsdt *)root_address;

        if (!acpi_sdt_validate(&rsdt->header))
        {
            root_table = nullptr;
            return false;
        }
        if ((rsdt->header.length - sizeof(struct acpi_sdt_header)) % sizeof(uint32_t) != 0)
        {
            root_table = nullptr;
            return false;
        }
        root_table = &rsdt->header;

        return true;
    }
    else if (acpi_signature_equal(root->signature, "XSDT", 4))
    {
        const struct acpi_xsdt *xsdt = (struct acpi_xsdt *)root_address;
        if (!acpi_sdt_validate(&xsdt->header))
        {
            root_table = nullptr;
            return false;
        }
        if ((xsdt->header.length - sizeof(struct acpi_sdt_header)) % sizeof(uint64_t) != 0)
        {
            root_table = nullptr;
            return false;
        }
        root_table = &xsdt->header;

        return true;
    }
    root_table = nullptr;
    return false;
}

const struct acpi_sdt_header *acpi_sdt_find(const char signature[4])
{
    if (!root_table)
    {
        return nullptr;
    }
    if (!signature)
    {
        return nullptr;
    }

    const struct acpi_sdt_header *header = (const struct acpi_sdt_header *)root_table;

    if (acpi_signature_equal(header->signature, "RSDT", 4))
    {
        const struct acpi_rsdt *rsdt = (const struct acpi_rsdt *)root_table;
        size_t entry_count = (rsdt->header.length - sizeof(struct acpi_sdt_header)) / sizeof(uint32_t);

        for (size_t i = 0; i < entry_count; i++)
        {
            struct acpi_sdt_header *h = (struct acpi_sdt_header *)(uintptr_t)rsdt->entries[i];
            if (!acpi_sdt_validate(h))
            {
                continue;        
            }
            if (acpi_signature_equal(h->signature, signature, 4))
            {
                return h;
            }
        }
    }
    else if (acpi_signature_equal(header->signature, "XSDT", 4))
    {
        const struct acpi_xsdt *xsdt = (const struct acpi_xsdt *)root_table;
        size_t entry_count = (xsdt->header.length - sizeof(struct acpi_sdt_header)) / sizeof(uint64_t);

        for (size_t i = 0; i < entry_count; i++)
        {
            struct acpi_sdt_header *h = (struct acpi_sdt_header *)xsdt->entries[i];
            if (!acpi_sdt_validate(h))
            {
                continue;
            }
            if (acpi_signature_equal(h->signature, signature, 4))
            {
                return h;
            }
        }
    }
    
    return nullptr;
}
