#include <stdint.h>

struct idt_entry {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero;
} __attribute((packed));

struct idt_ptr {
    uint16_t size;
    uint64_t address;
} __attribute((packed));

static struct idt_entry idt[256];

void idt_init() {

}

void idt_load() {

}