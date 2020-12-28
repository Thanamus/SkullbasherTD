
#include "ScheduleManager.hpp"
#include "../GameManager.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"
#include "PathFinder.hpp"


ScheduleManager::ScheduleManager(){
    startTime = std::chrono::steady_clock::now();
}

ScheduleManager::~ScheduleManager(){

}

void ScheduleManager::update(float deltaTime){
    kickOffTime = std::chrono::steady_clock::now();
    elapsedTimeInSec = std::chrono::duration_cast<std::chrono::seconds>(kickOffTime - startTime).count();

    if (waveGoGoGo == false)
    {
        // TODO check for wave go go go
        if (elapsedTimeInSec >= timeBetweenWaves)
        {
            waveGoGoGo = true;
            // std::cout << "go go go" << std::endl;
            // currentScene->gameManager->enermyAmountWave
        }


    } else if ( waveGoGoGo == true) {

        // int currentWave = gameManager->getCurrentWave();
        // int currentEnemy = gameManager->getCurrentEnemy();
        if ( elapsedTimeInSec % timeBetweenEnemies == 0)
        {
            auto gameObjectsList = currentScene->getGameObjects();
            for (size_t i = 0; i < gameObjectsList.size(); i++)
            {
                auto tempGameObject = gameObjectsList[i];
                if (auto objectWithPathFinder = tempGameObject->getComponent<PathFinder>())
                {
                    int waveOfCurrentObjectWithPathFinder = objectWithPathFinder->getWave();
                    if (waveOfCurrentObjectWithPathFinder == currentScene->gameManager->getCurrentWave())
                    {
                        int currentEnemyNumber = objectWithPathFinder->getEnemyNumber();
                        if (currentEnemyNumber == currentScene->gameManager->getCurrentEnemy())
                        {
                            objectWithPathFinder->setMovingStatus(true);

                            //TODOMake Game Manager Update Enemy and Wave Details
                        }
                    }
                }
            }
        }
    }

    
    
    

    // std::cout << "Time difference = " << timeInSec.count() << std::endl;

}


void ScheduleManager::fectInitialWaveSchedule(){
        //set inital timeBetweenWaves
    waveScheduleDetails initialTimeBetweens = currentScene->gameManager->getCurrentTimeBetweenWaves();
    timeBetweenWaves = initialTimeBetweens.timeBetweenWaves;
    timeBetweenEnemies = initialTimeBetweens.timeBetweenEnemies;
}
// #include <chrono>

// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;