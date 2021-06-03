#include <cernel/acpi/madt.h>
#include <cernel/interrupt/panic.h>
#include <cernel/lib/print.h>

#include <stddef.h>

struct MADT *madt;

void madt_init()
{
	madt = acpi_find_sdt("APIC");

	kprintf("Found MADT at %p\n", madt);

	size_t length = madt->sdt.length;

	kprintf("MADT size: %u\n", length);

	void *begin = &madt->entries_begin;

	size_t offset = 0;

	while (offset < length) {
		struct MADT_header *current_header = (struct MADT_header*)(begin + offset);
		kprintf("ENTRY FOUND   ADDRESS: %p   TYPE: %u   LENGTH: %u\n", current_header, current_header->type, current_header->length);
		offset += current_header->length;
		struct MADT_local_apic *lapic;		

		switch (current_header->type) {
			case MADT_ENTRY_PROCESSOR_LAPIC:
				lapic = (struct MADT_local_apic*)current_header;
				kprintf("    LAPIC found processor_id=%u apic_id=%u\n", lapic->processor_id, lapic->apic_id);
				break; 
		}
	}
}
