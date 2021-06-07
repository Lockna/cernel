#include <cernel/acpi/madt.h>
#include <cernel/interrupt/panic.h>
#include <cernel/lib/print.h>
#include <cernel/lib/alloc.h>

#include <stddef.h>

struct MADT *madt;

struct MADT_lapic **madt_lapics;
size_t madt_lapic_count = 0;

struct MADT_io_apic **madt_io_apics;
size_t madt_io_apic_count = 0;

struct MADT_iso **madt_isos;
size_t madt_iso_count = 0;

struct MADT_lapic_nmi **madt_lapic_nmis;
size_t madt_lapic_nmi_count = 0;

struct MADT_ioapic_nmi **madt_ioapic_nmis;
size_t madt_ioapic_nmi_count = 0;

void madt_init()
{
	madt = acpi_find_sdt("APIC");

	if (madt == NULL)
		generic_panic("MADT not found");

	kprintf("Found MADT at %p\n", madt);

	// calculate length of the madt entries
	size_t length = (uint64_t)madt->sdt.length - sizeof(struct SDT) - 8;

	kprintf("MADT size: %u\n", length);

	// TODO: create constant variable for 256
	// for now 256 Bytes should be enough
	madt_lapics = kmalloc(256);
	madt_io_apics = kmalloc(256);
	madt_isos = kmalloc(256);
	madt_lapic_nmis = kmalloc(256);
	madt_ioapic_nmis = kmalloc(256);

	void *begin = &madt->entries_begin;

	size_t offset = 0;

	while (offset < length) {
		struct MADT_header *current_header = (struct MADT_header*)(begin + offset);
		offset += current_header->length;

		switch (current_header->type) {
			case MADT_ENTRY_PROCESSOR_LAPIC:
				madt_lapics[madt_lapic_count] = (struct MADT_lapic*)current_header;
				madt_lapic_count++;
				break; 
			case MADT_ENTRY_IO_APIC:
				madt_io_apics[madt_io_apic_count] = (struct MADT_io_apic*)current_header;
				madt_io_apic_count++;
				break;
			case MADT_ENTRY_IO_APIC_INTERRUPT_SOURCE_OVERRIDE:
				madt_isos[madt_iso_count] = (struct MADT_iso*)current_header;
				madt_iso_count++;
				break;
			case MADT_ENTRY_IO_APIC_NMI_SOURCE:
				madt_ioapic_nmis[madt_ioapic_nmi_count] = (struct MADT_ioapic_nmi*)current_header;
				madt_ioapic_nmi_count++;
				break;
			case MADT_ENTRY_LAPIC_NMI:
				madt_lapic_nmis[madt_lapic_nmi_count] = (struct MADT_lapic_nmi*)current_header;
				madt_lapic_nmi_count++;
				break;
			case MADT_ENTRY_LAPIC_ADDRESS_OVERRIDE:
				madt->lapic_address = ((struct MADT_lapic_address_override*)current_header)->lapic_phys_addr;
				break;
		}
	}
}
