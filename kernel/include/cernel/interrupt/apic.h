#pragma once

/**
 *	@file apic.h
 *	@brief This file will hold the function prototypes for the apic
 *
 *  These functions are essential for the apic and IRQ handling, additionally these are important
 *  handling keyboard interrupts given by ps/2 keyboards.
 */

void init_apic();

void lapic_enable();

void lapic_write(uint32_t reg, uint32_t data);