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

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t type_attr) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = selector;
    idt[n].zero        = 0;
    idt[n].type_attr   = type_attr;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);   // read from keyboard port
    char c = translate_scancode(scancode); // simple lookup
    vga_put_char(c);
    outb(0x20, 0x20);               // send EOI to PIC
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
