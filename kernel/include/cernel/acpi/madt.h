#pragma once

#include <cernel/acpi/acpi.h>

#include <stdint.h>
#include <stddef.h>

struct MADT {
	struct SDT sdt;

	uint32_t lapic_address;
	uint32_t flags;
	
	uint8_t *entries_begin;
} __attribute__ ((packed));

struct MADT_header {
	uint8_t type;
	uint8_t length;
} __attribute__ ((packed));

struct MADT_lapic {
    struct MADT_header header;
    uint8_t processor_id;
    uint8_t apic_id;
    uint32_t flags;
} __attribute__ ((packed));

struct MADT_io_apic {
    struct MADT_header header;
    uint8_t apic_id;
    uint8_t reserved;
    uint32_t address;
    uint32_t gsib; // global system interrupt base
} __attribute__ ((packed));

struct MADT_iso {
    struct MADT_header header;
    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t gsi; // global system interrupt
    uint16_t flags;
} __attribute__ ((packed));

struct MADT_lapic_nmi {
    struct MADT_header header;
    uint8_t processor;
    uint16_t flags;
    uint8_t lint;
} __attribute__ ((packed));

struct MADT_ioapic_nmi {
	struct MADT_header header;
	uint8_t nmi_source;
	uint8_t reserved;
	uint16_t flags;
	uint32_t gsi;
} __attribute__ ((packed));

struct MADT_lapic_address_override {
	struct MADT_header header;
	uint16_t reserved;
	uint64_t lapic_phys_addr;
} __attribute__ ((packed));

enum {
	MADT_ENTRY_PROCESSOR_LAPIC,
	MADT_ENTRY_IO_APIC,
	MADT_ENTRY_IO_APIC_INTERRUPT_SOURCE_OVERRIDE,
	MADT_ENTRY_IO_APIC_NMI_SOURCE,
	MADT_ENTRY_LAPIC_NMI,
	MADT_ENTRY_LAPIC_ADDRESS_OVERRIDE
};

extern struct MADT *madt;

extern struct MADT_lapic **madt_lapics;
extern size_t madt_lapic_count;

extern struct MADT_io_apic **madt_io_apics;
extern size_t madt_io_apic_count;

extern struct MADT_iso **madt_isos;
extern size_t madt_iso_count;

extern struct MADT_lapic_nmi **madt_lapic_nmis;
extern size_t madt_lapic_nmi_count;

extern struct MADT_ioapic_nmi **madt_ioapic_nmis;
extern size_t madt_ioapic_nmi_count;

extern struct MADT_lapic_address_override **madt_lapic_address_overrides;
extern size_t madt_lapic_address_override_count;

void madt_init();
