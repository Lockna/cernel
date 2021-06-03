#include <cernel/acpi/acpi.h>
#include <cernel/interrupt/panic.h>
#include <cernel/lib/print.h>
#include <cernel/lib/memory.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct RSDPDescriptor *rsdp;
struct XSDT *xsdt;
struct RSDT *rsdt;

bool use_xsdt;

bool sdt_checksum_valid(struct SDT *sdt)
{
	uint8_t sum = 0;

	for (size_t i = 0; i < sdt->length; i++) {
		sum += ((uint8_t*)sdt)[i];
	}

	return sum == 0;
}

void acpi_init(struct RSDPDescriptor *stivale_rsdp)
{
	//check the ACPI version and panic if the it's v1.0
	if (stivale_rsdp->revision == 0) {
		use_xsdt = false;
	} else {
		use_xsdt = true;
	}

	// check if the checksum is correct
	uint8_t sum = 0;
	
	if (use_xsdt) {
		for (size_t i = 0; i < stivale_rsdp->length; i++) {
			sum += ((uint8_t *)stivale_rsdp)[i];
		}
	} else {
		for (size_t i = 0; i < 20; i++) {
			sum += ((uint8_t *)stivale_rsdp)[i];
		}
	}

	if (sum != 0)
		generic_panic("RSDP checksum check failed");

	rsdp = stivale_rsdp;

	// set the xsdt or rsdt and check the checksum
	if (use_xsdt) {
		xsdt = (struct XSDT*)rsdp->xsdt_address;

		if (!sdt_checksum_valid(&xsdt->sdt))
			generic_panic("XSDT checksum check failed\n");
	} else {
		rsdt = (struct RSDT*)(uint64_t)rsdp->rsdt_address;

		if (!sdt_checksum_valid(&rsdt->sdt))
			generic_panic("RSDT checksum check failed\n");
	}

	// call all table init functions
		
}

void *acpi_find_sdt(char *signature)
{
	struct SDT *sptr;

	if (use_xsdt) {
		for (size_t i = 0; i < xsdt->sdt.length; i++) {
			sptr = (struct SDT *)xsdt->sdt_ptr[i];

			if (memcmp(sptr->signature, signature, 4)) {
				return sptr;
			}
		}
	} else {
		for (size_t i = 0; i < rsdt->sdt.length; i++) {
			sptr = (struct SDT *)(uint64_t)rsdt->sdt_ptr[i];

			if (memcmp(sptr->signature, signature, 4)) {
				return sptr;
			}
		}
	}

	return NULL;
}

