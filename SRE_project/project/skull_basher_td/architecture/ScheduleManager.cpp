
#include "ScheduleManager.hpp"
#include "../GameManager.hpp"


ScheduleManager::ScheduleManager(){
    startTime = std::chrono::steady_clock::now();
}

ScheduleManager::~ScheduleManager(){

}

void ScheduleManager::update(float deltaTime){
    kickOffTime = std::chrono::steady_clock::now();
    timeInSec = std::chrono::duration_cast<std::chrono::seconds>(kickOffTime - startTime).count();

    if (timeInSec >= timeBetweenWaves)
    {
        // std::cout << "go go go" << std::endl;
        //TODO get current wave and enemy
        //TODO make 'current enemy' go go go
    }
    

    // std::cout << "Time difference = " << timeInSec.count() << std::endl;

}

// #include <chrono>

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;