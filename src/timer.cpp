#include "timer.hpp"

std::string Timer::current_time()
{
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %X");

    return ss.str();
}