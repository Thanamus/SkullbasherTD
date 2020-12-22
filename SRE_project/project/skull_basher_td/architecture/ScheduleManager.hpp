#pragma once

#include "Updatable.hpp"
#include <chrono>

class ScheduleManager {
    public:
        explicit ScheduleManager();
        ~ScheduleManager();

        void update(float deltaTime); //Note, can't inherit from Updateable, since Updateable is private to scene.

    private:

        int timeBetweenWaves = 5;
        int timeInSec;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point kickOffTime;
        // startTimeEpoch;

};

// #include <chrono>

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;