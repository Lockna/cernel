#pragma once

/**
 *	@file apic.h
 *	@brief This file will hold the function prototypes for the apic
 *
 *  These functions are essential for the apic and IRQ handling, additionally these are important
 *  handling keyboard interrupts given by ps/2 keyboards.
 */

#include <stdint.h>

void lapic_init();
void lapic_enable();
void lapic_eoi();
void lapic_write(uint32_t reg, uint32_t value);
uint32_t lapic_read(uint32_t reg);
