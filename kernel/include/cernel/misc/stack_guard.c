#include <stdint.h>
#include <cernel/interrupt/panic.h>

#define STACK_CHK_GUARD 0x12345678

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn))
void __stack_chk_fail(void) {
    generic_panic("Stack smash detected. Fallback into idle.");
    while (1);
}