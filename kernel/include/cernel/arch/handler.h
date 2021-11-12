#pragma once

/**
 *	@file handler.h
 *	@brief Interrupt and register struct are placed here
 *
 *  The cpu_register_state struct represents all general purpose register of a cpu
 *  The interrupt_context holds, additionally to the general purpose registers, special things
 *  like the interrupt number or error code. Represents the stackframe when interrupt is hit
 */

#include <cernel/arch/idt.h>


/**
 *  @brief All general purpose registers in one struct
 */
typedef struct cpu_register_state {
    /// Every gp register is defined as unsigned 64bit number
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rdx, rcx, rbx, rax; 
} __attribute__((packed)) cpu_register_state_t;

typedef struct interrupt_context {
    cpu_register_state_t regs; ///< general purpose registers which get pushed upon interrupt
    uint64_t int_no, err;      ///< interrupt number and the error code of the interrupt
    uint64_t rip, cs, rflags;  ///< Instruction pointer to see where the interrupt happened, code segments and flags
    uint64_t rsp, ss;          ///< Stack pointer and stack segment
} __attribute__((packed)) interrupt_context_t;

/**
 *  @brief Handles an raised interrupt and tries to continue execution
 *  @param irq_context A struct which holds the stackframe when interrupt occured
 *
 *  The function is called when an interrupt happens.
 *  In isr.asm every idt entry has it's own trampoline code, since some of the interrupts
 *  have an error code and some not.
 *  The trampoline code pushes zero if no error code is available for this interrupt and then
 *  handle_interrupt is called which tries to handle the interrupt accordingly and if not
 *  it panics.
 */
void handle_interrupt(interrupt_context_t *irq_context);
