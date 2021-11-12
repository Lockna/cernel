/**
 *	@file apic.c
 *	@brief IRQ Handling via the apic
 *
 *  IRQ's are getting handled here by the apic, the advanced programmable interrupt controller
 *  Later on there are lapics that will handle the IRQ's of the core the lapic belongs to
 */

#include <cernel/arch/apic.h>
#include <cernel/acpi/madt.h>
#include <cernel/mm/vmm.h>
#include <cernel/lib/print.h>

uintptr_t lapic_base;

void lapic_init()
{
	lapic_base = ((uint64_t)madt->lapic_address + HIGHER_HALF);
	
	// disable PIC
	asm volatile("movb $0xff,%al	\n \
				  outb %al, $0xa1	\n \
				  outb %al, $0x21	\n");

	lapic_enable();
}

void lapic_enable()
{
	// set the bit 8 to enable
	// set the number of the spurious interrupt to 0xff
	lapic_write(0xf0, lapic_read(0xf0) | 0x1ff);
}

void lapic_eoi()
{
	lapic_write(0xb0, 0);
}

void lapic_write(uint32_t reg, uint32_t value)
{
	*((uint32_t*)(lapic_base + reg)) = value;
}

uint32_t lapic_read(uint32_t reg)
{
	return *((uint32_t*)(lapic_base + reg));
}

