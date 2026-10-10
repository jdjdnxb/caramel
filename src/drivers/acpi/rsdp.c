#include <stdint.h>

#include <drivers/acpi/acpi.h>
#include <drivers/acpi/rsdp.h>

static const char signature[] = "RSD PTR ";

bool acpi_rsdp_validate(const struct acpi_rsdp *rsdp)
{
    if (!acpi_signature_equal(rsdp->signature, signature, 8))
    {
        return false;
    }

    if (!acpi_checksum_valid(rsdp, sizeof(struct acpi_rsdp)))
    {
        return false;
    }

    return true;
}

bool acpi_xsdp_validate(const struct acpi_xsdp *xsdp)
{
    if (!acpi_signature_equal(xsdp->signature, signature, 8))
    {
        return false;
    }

    if (xsdp->length < sizeof(struct acpi_xsdp))
    {
        return false;
    }

    // Legacy checksum
    if (!acpi_checksum_valid(xsdp, sizeof(struct acpi_rsdp)))
    {
        return false;
    }
    
    // Extended checksum
    if (!acpi_checksum_valid(xsdp, xsdp->length))
    {
        return false;
    }

    return true;
}
