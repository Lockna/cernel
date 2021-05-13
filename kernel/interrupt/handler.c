/**
 *	@file handler.c
 *	@brief Handling interrupts
 */

#include <cernel/interrupt/handler.h>
#include <cernel/init/idt.h>
#include <cernel/interrupt/panic.h>
#include <cernel/lib/print.h>

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
    // Just panic, nothing really to handle yet
    kprintf("error code:%d\n", irq_context->err);
    kprintf("RIP: %x\n", irq_context->rip);
    kprintf("RSP: %x\n", irq_context->rsp);

    panic(&irq_context->regs, err_message[irq_context->int_no]);
}