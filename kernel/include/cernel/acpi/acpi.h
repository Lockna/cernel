#pragma once

#include <stdint.h>

struct RSDPDescriptor {
	char signature[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_address;

	// entries for ACPI version >= 2.0, before using these entries the version of the ACPI must be checked
	uint32_t length;
	uint64_t xsdt_address;
	uint8_t extended_checksum;
	uint8_t reserved[3];
} __attribute__ ((packed));

struct SDT {
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oem_table_id[8];
	uint32_t oem_revision;
	uint32_t creator_id;
	uint32_t creator_revision;
} __attribute__ ((packed));

struct XSDT {
	struct SDT sdt;
	uint64_t sdt_ptr[];
} __attribute__ ((packed));

struct RSDT {
	struct SDT sdt;
	uint32_t sdt_ptr[];
} __attribute__ ((packed));

void acpi_init(struct RSDPDescriptor *rsdp);
void *acpi_find_sdt(char *signature);
