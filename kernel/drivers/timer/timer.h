#ifndef TIMER_H
#define TIMER_H

struct uptime {
    int hours;
    int minutes;
    int seconds;
};

struct uptime get_uptime();
void timer_init();

#endif /* TIMER_H */