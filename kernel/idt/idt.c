#include "idt.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t type_attr) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = selector;
    idt[n].zero        = 0;
    idt[n].type_attr   = type_attr;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

extern void keyboard_handler_stub(); // assembly stub

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base  = (uint32_t)&idt;

    // Example: set entry 0x21 (IRQ1 keyboard) to point to keyboard_handler
    idt_set_gate(0x21, (uint32_t)keyboard_handler_stub, 0x08, 0x8E);

    // Load IDT
    asm volatile("lidt %0" : : "m"(idtp));
}
