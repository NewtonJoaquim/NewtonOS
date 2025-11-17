#include <stdint.h>
#include "io_helpers.h"

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t offset_low;   // lower 16 bits of handler address
    uint16_t selector;     // code segment selector (usually 0x08)
    uint8_t  zero;         // always 0
    uint8_t  type_attr;    // flags (present, DPL, gate type)
    uint16_t offset_high;  // upper 16 bits of handler address
} __attribute__((packed));


struct idt_ptr {
    uint16_t limit;  // size of IDT in bytes - 1
    uint32_t base;   // address of the first entry
} __attribute__((packed));

void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t type_attr);
extern void keyboard_handler_stub();
void idt_init(void);