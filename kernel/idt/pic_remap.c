#include "pic_remap.h"

void pic_remap(void) {
    uint8_t a1, a2;

    // Save masks
    a1 = inb(0x21);
    a2 = inb(0xA1);

    // Start initialization sequence (ICW1)
    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();

    // Set vector offset (ICW2)
    outb(0x21, 0x20); // Master PIC mapped to 0x20–0x27
    io_wait();
    outb(0xA1, 0x28); // Slave PIC mapped to 0x28–0x2F
    io_wait();

    // Tell Master PIC that there is a slave at IRQ2 (ICW3)
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    // Set mode (ICW4)
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    // Restore saved masks
    outb(0x21, a1);
    outb(0xA1, a2);
}
