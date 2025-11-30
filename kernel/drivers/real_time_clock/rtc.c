#include <stdint.h>
#include "rtc.h"
#include "io_helpers.h"

//The RTC is accessible via I/O ports 0x70 (index) and 0x71 (data).
//Registers store seconds, minutes, hours, day, month, year.
static uint8_t read_cmos(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}

static int is_bcd() {
    outb(0x70, 0x0B);
    uint8_t status = inb(0x71);
    return !(status & 0x04); // if bit 2 is 0, values are BCD
}

static uint8_t bcd_to_bin(uint8_t val) {
    return (val & 0x0F) + ((val / 16) * 10);
}

struct rtc_time get_time() {
    struct rtc_time t;
    int bcd = is_bcd();

    t.sec   = read_cmos(0x00);
    t.min   = read_cmos(0x02);
    t.hour  = read_cmos(0x04);
    t.day   = read_cmos(0x07);
    t.month = read_cmos(0x08);
    t.year  = read_cmos(0x09);

    if (bcd) {
        t.sec   = bcd_to_bin(t.sec);
        t.min   = bcd_to_bin(t.min);
        t.hour  = bcd_to_bin(t.hour);
        t.day   = bcd_to_bin(t.day);
        t.month = bcd_to_bin(t.month);
        t.year  = bcd_to_bin(t.year);
    }

    t.year += 2000; // adjust depending on RTC base year
    return t;
}
