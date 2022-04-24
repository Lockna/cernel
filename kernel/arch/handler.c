/**
 *	@file handler.c
 *	@brief Handling interrupts
 */

#include <cernel/arch/handler.h>
#include <cernel/arch/idt.h>
#include <cernel/sys/panic.h>
#include <cernel/lib/print.h>
#include <cernel/arch/apic.h>
#include <debug/debug.h>

char* err_message[] = {
    "Divide by 0",
    "Reserved",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bounds range exceeded",
    "Invalid Opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 FPU error",
    "Alignment check",
    "Machine check",
    "SIMD Floating Point Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void handle_interrupt(interrupt_context_t *irq_context)
{
	// check which interrupt happened and handle it
	// default: print info and panic
    switch (irq_context->int_no) {
		case 255:
			// spurious interrupt
			break;
        case 32:
            // timer interrupt
            lapic_eoi();
            kprintf("timer interrupt\n");
            break;
        default:
            print_stacktrace(irq_context->rip, irq_context->regs.rbp);

			kprintf("error code:%d\n", irq_context->err);
			kprintf("RIP: %x\n", irq_context->rip);
			kprintf("RSP: %x\n", irq_context->rsp);

			panic(&irq_context->regs, err_message[irq_context->int_no]);
	}
}
