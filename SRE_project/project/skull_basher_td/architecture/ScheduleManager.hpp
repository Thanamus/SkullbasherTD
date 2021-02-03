#pragma once

#include "Updatable.hpp"
// #include "Scene.hpp"
#include <chrono>
#include <memory>

//std::shared_ptr<Scene> currentScene;
class Scene;

class ScheduleManager {
    public:
        explicit ScheduleManager();
        virtual ~ScheduleManager();

        void update(float deltaTime); //Note, can't inherit from Updateable, since Updateable is private to scene.

        void fetchInitialWaveSchedule();
    private:

        int timeBetweenWaves = 0;
        int timeBetweenEnemies = 0;
        bool waveGoGoGo = false;
        bool enemyGoGoGo = false;

        bool lastEnemy = false;
        int elapsedTimeInSec = 0;
        int goGoTimeSec = 0;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point kickOffTime;
        std::chrono::steady_clock::time_point goGoTime;

};
