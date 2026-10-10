// System Description Table definitions
#pragma once

// System Description Table Header
struct acpi_sdt_header
{
    char     signature[4];          // The signature determines the content of the system description table.
                                    // For more information, check 5.2.6 (System Description Table Header) 
                                    // inside of the ACPI Specification 6.5a.
    uint32_t length;                // Length of the table in bytes.
    uint8_t  revision;              // Revision of the structure corresponding to the signature field for this table.
    uint8_t  checksum;              // The entire table, including the checksum field, must add to zero to be considered valid.
    char     oemid[6];              // An OEM-supplied string that identifies the OEM.
    char     oem_table_id[8];       // An OEM-supplied string that the OEM uses to identify the particular data table.
    uint32_t oem_revision;          // An OEM-supplied revision number.
    uint32_t creator_id;            // Vendor ID of utility that created this table.
    uint32_t creator_revision;      // Revision of utility that created this table.
}__attribute__((packed));

static_assert(sizeof(struct acpi_sdt_header) == 36);

// Root System Descriptor Table
struct acpi_rsdt
{
    struct acpi_sdt_header header;
    uint32_t entries[];
}__attribute__((packed));

// eXtended System Descriptor Table
struct acpi_xsdt
{
    struct acpi_sdt_header header;
    uint64_t entries[];
}__attribute__((packed));

bool acpi_sdt_init(uintptr_t root_address);

bool acpi_sdt_validate(const struct acpi_sdt_header *header);
const struct acpi_sdt_header *acpi_sdt_find(const char signature[4]);
