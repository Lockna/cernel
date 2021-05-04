#include <cernel/interrupt/panic.h>
#include <cernel/lib/print.h>
#include <cernel/mm/vmm.h>

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

__attribute__((noreturn)) 
void panic(struct cpu_state *cpu) {

    kprintf("\n\n%s\n\n", err_message[cpu->int_no]);
    kprintf("error number:%d\n", cpu->err);
    kprintf("RIP: %x\n", cpu->rip);
    kprintf("RSP: %x\n", cpu->rsp);
    kprintf("RAX: %x\n", cpu->rax);
    kprintf("RBX: %x\n", cpu->rbx);
    kprintf("RCX: %x\n", cpu->rcx);
    kprintf("RDX: %x\n", cpu->rdx);
    kprintf("RBP: %x\n", cpu->rbp);
    kprintf("RSI: %x\n", cpu->rsi);
    kprintf("RDI: %x\n", cpu->rdi);
    kprintf("R15: %x\n", cpu->r15);
    kprintf("CR3: %x\n", (uintptr_t)cr3_read());
    uintptr_t cr2;
	asm("\t mov %%cr2,%0" : "=r"(cr2));
    kprintf("CR2: %x", cr2);

    while(1) {
        // Halt the CPU
        asm volatile("cli; hlt");
    }
}