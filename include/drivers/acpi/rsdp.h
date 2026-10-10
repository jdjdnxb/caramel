#pragma once

/*
 *  signature - an 8 byte magic number used for locating the RSDP/XSDP, containing "RSD PTR ". Including the trailing character.
 *  checksum - a byte used to verify the first 20 bytes of the RSDP.
 *  oemid - an OEM-supplied string that identifies the OEM.
 *  revision - the revision of this structure.
 *  rsdt_address - 32-bit physical address of the RSDT. Deprecated since version 2.0
 *  
 *  length - the length of the table in bytes
 *  xsdt_address - 64-bit physical address of the XSDT.
 *  extended_checksum - the checksum of the entire table.
 *  reserved - reserved field (obviously)
 */

// Structure for revision 1
struct acpi_rsdp
{
    char     signature[8];
    uint8_t  checksum;
    char     oemid[6];
    uint8_t  revision;
    uint32_t rsdt_address;
}__attribute__((packed));

// Structure for revision 2+
struct acpi_xsdp
{
    char     signature[8];
    uint8_t  checksum;
    char     oemid[6];
    uint8_t  revision;
    uint32_t rsdt_address;
   
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t  extended_checksum;
    uint8_t  reserved[3];
}__attribute__((packed));

static_assert(sizeof(struct acpi_rsdp) == 20);
static_assert(sizeof(struct acpi_xsdp) == 36);

bool acpi_rsdp_validate(const struct acpi_rsdp *rsdp);
bool acpi_xsdp_validate(const struct acpi_xsdp *xsdp);
