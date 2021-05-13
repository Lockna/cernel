/**
 *	@file apic.c
 *	@brief IRQ Handling via the apic
 *
 *  IRQ's are getting handled here by the apic, the advanced programmable interrupt controller
 *  Later on there are lapics that will handle the IRQ's of the core the lapic belongs to
 */

#include <cernel/interrupt/apic.h>