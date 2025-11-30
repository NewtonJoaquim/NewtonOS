#ifndef RTC_H
#define RTC_H

struct rtc_time {
    int sec, min, hour, day, month, year;
};

struct rtc_time get_time();

#endif /* RTC_H */