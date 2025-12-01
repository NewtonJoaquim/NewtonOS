#include <stdint.h>
#include "io_helpers.h"

volatile uint32_t ticks = 0;

void timer_handler() {
    ticks++;
    outb(0x20, 0x20); // EOI
}

struct uptime {
    int hours;
    int minutes;
    int seconds;
};

struct uptime get_uptime() {
    // Convert ticks to seconds using the configured TIMER_FREQ
    // If TIMER_FREQ is 100, then total_seconds = ticks / 100
    const uint32_t TIMER_FREQ = 100;
    uint32_t total_seconds = ticks / TIMER_FREQ;
    struct uptime u;
    u.hours   = total_seconds / 3600;
    u.minutes = (total_seconds % 3600) / 60;
    u.seconds = total_seconds % 60;
    return u;
}

// Initialize the Programmable Interval Timer (PIT) to a given frequency.
void timer_init() {
    const uint32_t PIT_FREQ = 1193182;
    const uint16_t divisor = (uint16_t)(PIT_FREQ / 100); // 100 Hz

    // Command byte: channel 0, access mode lobyte/hibyte, mode 3 (square wave), binary
    outb(0x43, 0x36);
    // Send divisor low byte then high byte
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
