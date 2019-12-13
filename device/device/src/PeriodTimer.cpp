#include "PeriodTimer.h"

PeriodTimer::PeriodTimer(uint64_t msdelay, on_period_t on_period)
{
    _on_period = on_period;
    _last = 0;
    _delay = msdelay;
}

void PeriodTimer::update()
{
    uint64_t ms = millis();
    if (ms > _last + _delay) {
        _last = ms;
        if (_on_period) {
            _on_period();
        }
    }
}
