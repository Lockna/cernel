#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cernel/mm/pmm.h>

// just use some number for the heap base that will never be used anywhere else
#define HEAP_BASE 0x0000100000000000

// value that is placed between two allocations, two detect an overflow
#define HEAP_COOKIE 0x123456789ABCDEF

// start size of the heap
#define HEAP_START_SIZE (PAGE_SIZE * 0x10)

struct HeapNode {
    uint64_t cookie;
    struct HeapNode *next;
    struct HeapNode *previous;
    uint64_t size;
    bool isUsed;
};

void alloc_init();

void *kmalloc(size_t size);
void kfree(void *ptr);
