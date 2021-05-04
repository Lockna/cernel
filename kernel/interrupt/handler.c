#include <cernel/interrupt/handler.h>
#include <cernel/init/idt.h>
#include <cernel/interrupt/panic.h>

void handle_interrupt(struct cpu_state *cpu)
{
    // Just panic, nothing really to handle yet
    panic(cpu);
}