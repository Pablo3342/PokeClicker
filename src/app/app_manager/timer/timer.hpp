#pragma once

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

class Timer {
public:
    Timer();
    ~Timer();

    void start();
    void reset();
    double getTicks();
    double getTimeSinceStart();

private:
    TimePoint tickTimePoint;
    TimePoint startTimePoint;
};