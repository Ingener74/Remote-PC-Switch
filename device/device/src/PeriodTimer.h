
#ifndef PERIODTIMER_H
#define PERIODTIMER_H

#include <Arduino.h>

typedef void (*on_period_t)();
class PeriodTimer {
public:
    PeriodTimer(uint64_t msdelay, on_period_t on_period);
    void update();

    on_period_t _on_period;
    uint64_t _last;
    uint64_t _delay;
};

#endif
