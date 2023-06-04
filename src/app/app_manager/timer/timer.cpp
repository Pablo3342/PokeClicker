#include "timer.hpp"

Timer::Timer()
    :   tickTimePoint(),
        startTimePoint()
{}

Timer::~Timer()
{}

void Timer::start() {
    startTimePoint = std::chrono::system_clock::now();
    tickTimePoint = startTimePoint;
}

void Timer::reset() {
    tickTimePoint = std::chrono::system_clock::now();
}

double Timer::getTicks() {
    std::chrono::duration<double> time = std::chrono::system_clock::now() - tickTimePoint;
    return time.count();
}

double Timer::getTimeSinceStart() {
    std::chrono::duration<double> time = std::chrono::system_clock::now() - startTimePoint;
    return time.count();
}