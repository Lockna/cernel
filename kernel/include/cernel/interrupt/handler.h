#pragma once
#include <cernel/init/idt.h>

typedef struct cpu_register_state {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rdi, rsi, rbp, rdx, rcx, rbx, rax;
} __attribute__((packed)) cpu_register_state_t;

typedef struct interrupt_context {
    cpu_register_state_t regs;
    uint64_t int_no, err;
    uint64_t rip, cs, rflags;
    uint64_t rsp, ss;
} __attribute__((packed)) interrupt_context_t;

void handle_interrupt(interrupt_context_t *irq_context);