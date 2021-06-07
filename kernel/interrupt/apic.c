/**
 *	@file apic.c
 *	@brief IRQ Handling via the apic
 *
 *  IRQ's are getting handled here by the apic, the advanced programmable interrupt controller
 *  Later on there are lapics that will handle the IRQ's of the core the lapic belongs to
 */

#include <cernel/interrupt/apic.h>
#include <cernel/apic/madt.h>
#include <cernel/mm/vmm.h>

uint32_t *lapic_eoi_ptr;

void init_apic()
{
    uintptr_t lapic_base = madt->lapic_address +  KERNEL_PHYS_OFFSET;
    lapic_enable();
}

void lapic_enable()
{
    lapic_write(0xb0, 0);
}

void lapic_write(uint32_t reg, uint32_t data)
{
    uintptr_t lapic_base = madt->lapic_address +  KERNEL_PHYS_OFFSET;
    *((volatile uint32_t)(lapic_base + reg)) = data;
}
