#ifndef ASSERT_H
#define ASSERT_H

#include <cernel/sys/panic.h>
#include <cernel/lib/print.h>
#include <debug/debug.h>

void __assert (const char *msg, const char *file, int line) {
    kprintf("%s:%d: assertion failed: %s\n", file, line, msg);
}

#define assert(EX) ((EX) ? (void)0 : __assert( #EX, __FILE__, __LINE__))
#define assert_not_reached() (assert(0))

#endif // ASSERT_H