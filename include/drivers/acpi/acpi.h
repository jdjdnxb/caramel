#pragma once

#include <stdint.h>
#include <stddef.h>
#include <drivers/acpi/rsdp.h>

void acpi_init(void);

bool acpi_checksum_valid(const void *data, size_t length);
bool acpi_rsdp_validate(const struct acpi_rsdp *rsdp);
bool acpi_xsdp_validate(const struct acpi_xsdp *xsdp);

// Generic Address Structure, used to express register addresses within ACPI tables
struct acpi_gas
{
    uint8_t address_space_id;       // The address space where the data structure or register exists
    uint8_t register_width;         // The size in bits of the given register. Must be 0 when addressing a data structure
    uint8_t register_offset;        // The bit offset of the given register at the given address. Must be 0 when addressing a data structure
    uint8_t access_size;            // Specifies access size
    uint64_t address;               // Address of given register or data structure
}__attribute__((packed));

// Address Space IDs
#define ACPI_SYSTEM_MEMORY_SPACE    0x00
#define ACPI_SYSTEM_IO_SPACE        0x01
#define ACPI_PCI_CONFIG_SPACE       0x02
#define ACPI_EMBEDDED_CONTROLLER    0x03
#define ACPI_SM_BUS                 0x04
#define ACPI_SYSTEM_CMOS            0x05
#define ACPI_PCI_BAR_TARGET         0x06
#define ACPI_IPMI                   0x07
#define ACPI_GENERAL_PURPOSE_IO     0x08
#define ACPI_GENERIC_SERIAL_BUS     0x09
#define ACPI_PCC                    0x0A        // Platform Comunication Channel
#define ACPI_PRM                    0x0B        // Platform Runtime Mechanism
// 0x0C - 0x7E are reserved
#define ACPI_FFH                    0x7F        // Functional Fixed Hardware
// 0x80 - 0xFF are OEM defined

// Address Space Format
#define ACPI_ASF_SYSTEM_MEMORY      0x00
#define ACPI_ASF_SYSTEM_IO          0x01
#define ACPI_ASF_PCI_CONFIG         0x02
#define ACPI_ASF_PCI_BAR_TARGET     0x06
#define ACPI_ASF_PCC                0x0A
#define ACPI_ASF_FFH                0x7F

// System Description Table Header
struct acpi_sdt_header
{
    char     signature[4];          // The signature determines the content of the system description table.
                                    // For more information, check 5.2.6 (System Description Table Header) inside of the ACPI Specification 6.5a.
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
