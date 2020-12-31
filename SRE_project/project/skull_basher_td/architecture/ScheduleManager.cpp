
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
            int enemyToGoGoGo = currentScene->gameManager->getCurrentEnemy();
            int enemySetToGoGoGo = currentScene->gameManager->getCurrentEnemySet();
            int enemyWaveToGoGoGo = currentScene->gameManager->getCurrentWave();

            for (size_t i = 0; i < gameObjectsList.size(); i++)
            {
                auto tempGameObject = gameObjectsList[i];
                if (auto objectWithPathFinder = tempGameObject->getComponent<PathFinder>())
                { //get the GameObject with PathFinder

                    //get the wave number to compare with the next enemy to start moving
                    int currentEnemyWaveNumber = objectWithPathFinder->getWave();
                    int currentEnemyNumber = objectWithPathFinder->getEnemyNumber();
                    int currentEnemySetNumber = objectWithPathFinder->getEnemyNumber();

                    if (currentEnemyWaveNumber == enemyWaveToGoGoGo &&
                        currentEnemySetNumber == enemySetToGoGoGo &&
                        currentEnemyNumber == enemyToGoGoGo)
                    {
                        objectWithPathFinder->setMovingStatus(true);
                    }
                    

                    // if (waveOfCurrentObjectWithPathFinder == currentScene->gameManager->getCurrentWave())
                    // { //wave number is correct, check set number
                    //     if (currentEnemyNumber == currentScene->gameManager->getCurrentEnemy())
                    //     {
                    //         objectWithPathFinder->setMovingStatus(true);

                    //         //TODOMake Game Manager Update Enemy and Wave Details
                    //     }
                    // }
                }
            }
        }
    }

    
    
    

    // std::cout << "Time difference = " << timeInSec.count() << std::endl;

}


void ScheduleManager::fetchInitialWaveSchedule(){
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