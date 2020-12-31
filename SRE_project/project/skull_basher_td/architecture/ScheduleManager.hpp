#pragma once

#include "Updatable.hpp"
// #include "Scene.hpp"
#include <chrono>
#include <memory>

//std::shared_ptr<Scene> currentScene;
class Scene;
class GameManager;

class ScheduleManager {
    public:
        explicit ScheduleManager();
        virtual ~ScheduleManager();

        void update(float deltaTime); //Note, can't inherit from Updateable, since Updateable is private to scene.

        std::shared_ptr<Scene> currentScene;
        void fetchInitialWaveSchedule();
    private:

        int timeBetweenWaves = 0;
        int timeBetweenEnemies = 0;
        bool waveGoGoGo = false;
        bool enemyGoGoGo = false;

        int elapsedTimeInSec = 0;
        int goGoTimeSec = 0;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point kickOffTime;
        std::chrono::steady_clock::time_point goGoTime;
        // startTimeEpoch;

};

// #include <chrono>

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;