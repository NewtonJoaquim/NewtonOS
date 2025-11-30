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
    uint32_t total_seconds = ticks / 18; // 18.2 Hz ≈ 18 ticks per second
    struct uptime u;
    u.hours   = total_seconds / 3600;
    u.minutes = (total_seconds % 3600) / 60;
    u.seconds = total_seconds % 60;
    return u;
}
