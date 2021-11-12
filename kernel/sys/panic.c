/**
 *	@file panic.c
 *	@brief Dumbing registers to screen and halt execution
 *
 *  If a fatal error occurs during execution the panic function will get called, which prints
 *  the registers to screen and halts the cpu.
 */

#include <cernel/sys/panic.h>
#include <cernel/arch/handler.h>
#include <cernel/lib/print.h>
#include <cernel/mm/vmm.h>

void dump_register_state(cpu_register_state_t *state) {
  asm (
    "movq %%r15, %[r15];"
    "movq %%r14, %[r14];"
    "movq %%r13, %[r13];"
    "movq %%r12, %[r12];"
    "movq %%r11, %[r11];"
    "movq %%r10, %[r10];"
    "movq %%r9, %[r9];"
    "movq %%r8, %[r8];"
    "movq %%rdi, %[rdi];"
    "movq %%rsi, %[rsi];"
    "movq %%rbp, %[rbp];"
    "movq %%rdx, %[rdx];"
    "movq %%rcx, %[rcx];"
    "movq %%rbx, %[rbx];"
    "movq %%rax, %[rax];"
    :
    [r15] "=m" (state->r15), 
    [r14] "=m" (state->r14),
    [r13] "=m" (state->r13),
    [r12] "=m" (state->r12),
    [r11] "=m" (state->r11),
    [r10] "=m" (state->r10),
    [r9] "=m" (state->r9),
    [r8] "=m" (state->r8),
    [rdi] "=m" (state->rdi),
    [rsi] "=m" (state->rsi),
    [rbp] "=m" (state->rbp),
    [rdx] "=m" (state->rdx),
    [rcx] "=m" (state->rcx),
    [rbx] "=m" (state->rbx),
    [rax] "=m" (state->rax)
  );
}

__attribute__((noreturn)) 
void panic(cpu_register_state_t *state, char *error_message) {

    kprintf("\n\nKERNEL PANIC - %s\n", error_message);
    kprintf("RAX: %x\n", state->rax);
    kprintf("RBX: %x\n", state->rbx);
    kprintf("RCX: %x\n", state->rcx);
    kprintf("RDX: %x\n", state->rdx);
    kprintf("RBP: %x\n", state->rbp);
    kprintf("RSI: %x\n", state->rsi);
    kprintf("RDI: %x\n", state->rdi);
    kprintf("R15: %x\n", state->r15);
    kprintf("CR3: %x\n", (uintptr_t)cr3_read());
    uintptr_t cr2;
	  asm("\t mov %%cr2,%0" : "=r"(cr2));
    kprintf("CR2: %x", cr2);

    while(1) {
        // Halt the CPU
        asm volatile("cli; hlt");
    }
}

__attribute__((noreturn))
void __generic_panic(const char *error_message, const char *file, int line) {
  // cpu_register_state_t regs;
  // dump_register_state(&regs);

  kprintf("%s:%d KERNEL PANIC - %s\n", file, line, error_message);

  while(1) {
      // Halt the CPU
      asm volatile("cli; hlt");
  }
}
