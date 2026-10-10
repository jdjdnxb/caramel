#include <stdint.h>

#include <boot/requests.h>

#include <drivers/acpi/acpi.h>
#include <drivers/acpi/rsdp.h>
#include <drivers/acpi/sdt.h>
#include <drivers/acpi/madt.h>

#include <limine.h>
#include <kprintf.h>

static struct limine_rsdp_response *limine_rsdp_response;

static struct acpi_rsdp *rsdp;
static struct acpi_xsdp *xsdp; 

void acpi_init(void)
{
    limine_rsdp_response = rsdp_request.response;

    uint64_t rsdp_address = (uint64_t)limine_rsdp_response->address;
    rsdp = (struct acpi_rsdp *)rsdp_address;

    kprintf("RSDP Address: %llx\n", rsdp_address);

    uint8_t revision = rsdp->revision;

    if (revision == 0)
    {
        // ACPI Revision 1.0, use the legacy RSDP
        (void)xsdp;
        
        kprintf("ACPI Revision 1.0, using the legacy RSDP.\n");
        
        bool valid = acpi_rsdp_validate(rsdp);

        if (!valid)
        {
            kprintf("RSDP invalid. Stopping the ACPI initialization process.\n");
            return;
        }

        if (!acpi_sdt_init((uintptr_t)rsdp->rsdt_address))
        {
            kprintf("RSDT initialization failed!");
            return;
        }
    }
    else if (revision >= 2)
    {
        // ACPI Revision 2.0+, use the XSDP
        xsdp = (struct acpi_xsdp *)rsdp_address;
        kprintf("ACPI Revision 2.0+, using the XSDP.\n");

        bool valid = acpi_xsdp_validate(xsdp);
        
        if (!valid)
        {
            kprintf("XSDP invalid. Stopping the ACPI initialization process.\n");
            return;
        }

        if (!acpi_sdt_init((uintptr_t)xsdp->xsdt_address))
        {
            kprintf("XSDT initialization failed!");
            return;
        }
    }
    else
    {
        kprintf("ACPI Revision unsupported or invalid.\n");
        return;
    }
}

bool acpi_checksum_valid(const void *data, size_t length)
{
    uint8_t sum = 0;
    for (size_t i = 0; i < length; i++)
    {
        sum += ((const uint8_t *)data)[i];
    }
    return sum == 0; 
}

bool acpi_signature_equal(const char *a, const char *b, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}
