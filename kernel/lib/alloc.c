#include <stddef.h>
#include <stdbool.h>

#include <cernel/lib/alloc.h>
#include <cernel/mm/pmm.h>
#include <cernel/mm/vmm.h>
#include <cernel/lib/print.h>
#include <cernel/interrupt/panic.h>
#include <debug/debug.h>

void *heap_end;
struct HeapNode *head;

static void splitBlock(struct HeapNode *node, size_t size) {
    struct HeapNode *current = (void *)((uint64_t)node + sizeof(struct HeapNode) + size);

    current->size = node->size - size - sizeof(struct HeapNode);
    node->size = size;
    current->isUsed = false;
    current->cookie = HEAP_COOKIE;

    current->next = node->next;
    node->next = current;
    current->previous = node;

    if (current->next != NULL)
        current->next->previous = current;
}

static void combineHeapNodeForward(struct HeapNode *node) {
    if (node->next == NULL)
        return;

    node->size += node->next->size + sizeof(struct HeapNode);
    node->next = node->next->next;

    if (node->next != NULL)
        node->next->previous = node;
}

static void combineHeapNodeBackward(struct HeapNode *node) {
    if (node->previous == NULL)
        return;
    
    combineHeapNodeForward(node->previous);
}

// extend the heap by a certain amount of pages
static void expand_heap(size_t page_count) {

    for (size_t i = 0; i < page_count; i++) {
        uintptr_t ptr = pmm_alloc();
        vmm_map(pt_kernel, (uintptr_t)heap_end, ptr);

        heap_end += PAGE_SIZE;
    }    
}

void alloc_init() {
    heap_end = (void *)HEAP_BASE;

    for (size_t i = 0; i < HEAP_START_SIZE; i += PAGE_SIZE) {
        expand_heap(1); // expand the heap by one page
    }

    // setup up the head node
    head = (struct HeapNode *)HEAP_BASE;

    head->isUsed = false;
    head->next = NULL;
    head->previous = NULL;
    head->size = HEAP_START_SIZE - sizeof(struct HeapNode);
    head->cookie = HEAP_COOKIE;
}

void *kmalloc(size_t size) {

    if (size == 0)
        return NULL;

    struct HeapNode *current = head;

    while (current != NULL) {
        if (current->cookie != HEAP_COOKIE) {
            generic_panic("HEAP Smash Detected!");
        } else if (current->isUsed == false && current->size >= (size + sizeof(struct HeapNode))) {
            // found free memory

            if (current->size  == (size + sizeof(struct HeapNode))) {
                current->isUsed = true;
                return ++current;
            } else {
                // current block is greater than requested size => split the block

                splitBlock(current, size);
                current->isUsed = true;

                return ++current;
            }

        } else if (current->next == NULL) {
            // expand heap is needed

            size_t page_count = size / PAGE_SIZE;
            page_count++;

            expand_heap(page_count);

            current->size += (page_count * PAGE_SIZE);

            return kmalloc(size); 
        }

        current = current->next;
    }

    return NULL;
}

void kfree(void *ptr) {
    if (ptr == NULL)
        return;
    
    struct HeapNode *current = head;

    while (current != NULL) {
        if (current->cookie != HEAP_COOKIE) {
            generic_panic("HEAP Smash Detected!");
        }

        if (((uint64_t)current + sizeof(struct HeapNode)) == (uint64_t)ptr) {
            current->isUsed = false;

            if (current->next != NULL && current->next->isUsed == false)
                combineHeapNodeForward(current);
            
            if (current->previous != NULL && current->previous->isUsed == false)
                combineHeapNodeBackward(current);
            
            return;
        }

        current = current->next;
    }
}