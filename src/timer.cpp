#include "raylib.hpp"
#include "timer.hpp"


Timer::Timer(): period(0.0F) {}


Timer::Timer(double period): period(period) {}


double Timer::tick(void) {
    double now = GetTime();
    double time_passed = now - statemark;

    if (time_passed > period) {
        statemark = now;
        return time_passed;
    }
    else {
        return 0.0F;
    }
}