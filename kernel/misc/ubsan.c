#include <stdint.h>
#include <cernel/lib/print.h>
#include <cernel/interrupt/panic.h>

const char *type_check_kinds[] = {
    "load of", "store to", "reference binding to", "member access within",
    "member call on", "constructor call on", "downcast of", "downcast of",
    "upcast of", "cast to virtual base of", "_Nonnull binding to",
    "dynamic operation on"
};

struct source_location {
    char *filename;
    uint32_t line;
    uint32_t column;
};

struct type_descriptor {
    uint16_t type_kind;
    uint16_t type_info;
    char type_name[1];
};

struct shift_out_ouf_bounds_data {
    struct source_location loc;
    struct type_descriptor *lhs;
    struct type_descriptor *rhs;
};

struct type_mismatch_data {
    struct source_location loc;
    struct type_descriptor *type;
    unsigned char log_alignment;
    unsigned char type_check_kind;
};

void print_location(struct source_location *location)
{
    kprintf("%s:%u:%u\n", location->filename, location->line, location->column);
}

void __ubsan_handle_shift_out_of_bounds(struct shift_out_ouf_bounds_data *data, void *lhs, void *rhs)
{
    print_location(&data->loc);
    generic_panic("shift out of bounds\n");    
}

void __ubsan_handle_type_mismatch_v1(struct type_mismatch_data *data, void *pointer) {
    // TODO: handle other options
    
    uint64_t alignment = (uint64_t)1 << data->log_alignment;

    if ((uint64_t)pointer & (alignment - 1)) {
        kprintf("alignment issue\n");

        kprintf("%s misaligned address %p for type %x, which requires %d byte alignment\n", type_check_kinds[data->type_check_kind], pointer, data->type, alignment);
    }

    print_location(&data->loc);
    kprintf("pointer: %p\n", pointer);
    kprintf("alignment: %u %u\n", data->log_alignment, alignment);

    generic_panic("type mismatch");
}

void __ubsan_handle_pointer_overflow() { generic_panic("pointer overflow"); }
void __ubsan_handle_sub_overflow() { generic_panic("sub overflow"); }
void __ubsan_handle_mul_overflow() { generic_panic("mul overflow"); }
void __ubsan_handle_add_overflow() { generic_panic("add oveflow"); }
void __ubsan_handle_load_invalid_value() { generic_panic("load invalid value"); }
void __ubsan_handle_out_of_bounds() { generic_panic("out of bounds"); }
void __ubsan_handle_divrem_overflow() { generic_panic("divrem overflow"); }
void __ubsan_handle_negate_overflow() { generic_panic("negate overflow"); }

