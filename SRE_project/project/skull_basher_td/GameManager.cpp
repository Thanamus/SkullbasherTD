//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <SDL_events.h>
#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GameManager.hpp"
#include "./architecture/Camera.hpp"
#include "./architecture/PersonController.hpp"
#include "./architecture/ModelRenderer.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <iostream>

using namespace sre;
using namespace glm;

GameManager::GameManager() {
}

void GameManager::init() {
    loadTowers("data/towers.json");
    selectedTower = towers[0];
}

void GameManager::loadTowers(std::string filename) {
    using namespace rapidjson;
    std::ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    int rowArrayCount = d["Towers"].GetArray().Size(); //get the tile map key, the array size is the outset loop's size
    std::cout << "rowArrayCount: " << rowArrayCount << "\n";
    for (size_t row = 0; row < rowArrayCount; row++) //go through each 'row' of the map
    {
        int id = d["Towers"][row]["id"].GetInt();
        std::string name = d["Towers"][row]["name"].GetString();
        std::string icon = d["Towers"][row]["icon"].GetString();
        std::string mesh = d["Towers"][row]["mesh"].GetString();
        float buildCost = d["Towers"][row]["buildCost"].GetFloat();
        float constructionTime = d["Towers"][row]["constructionTime"].GetFloat();
        float delay = d["Towers"][row]["delay"].GetFloat();
        towers.push_back(std::make_shared<Tower>(id, name, icon, mesh, buildCost, constructionTime, delay));
    }
}

std::vector<std::shared_ptr<Tower>> GameManager::GetTowers() {
    return this->towers;
}


void GameManager::onKey(SDL_Event &event)
{
    if (event.type != SDL_KEYDOWN)
        return;

    int selectedTowerIndex = 0;
    bool pressed = false;
    switch (event.key.keysym.sym)
    {
        case SDLK_1:
            selectedTowerIndex = 0;
            pressed = true;
            buildModeActive = true;
            break;
        case SDLK_2:
            selectedTowerIndex = 1;
            pressed = true;
            buildModeActive = true;
            break;
        case SDLK_3:
            selectedTowerIndex = 2;
            pressed = true;
            buildModeActive = true;
            break;
        case SDLK_ESCAPE:
            if(buildModeActive)
            {
                buildModeActive = false;
                pressed = true;
            }
            else if (!buildModeActive && !paused)
            {
                //paused = true;
                //ToggleLockMouse();
                TogglePause();
            }
            else if (paused)
            {
                /*paused = false;
                ToggleLockMouse();*/
                TogglePause();
            }
            break;
        default:
            break;
    }

    if(pressed && selectedTowerIndex <= towers.size())
    {
        selectedTower = towers[selectedTowerIndex];
        if(buildModeActive)
        {
            updateTowerIndicator();
        }
        else {
            auto towerIndicator = currentScene->cameras[0]->getGameObject()->getComponent<PersonController>()->tower;
            towerIndicator->getComponent<ModelRenderer>()->active = false;
        }
    }
}

void GameManager::onMouse(SDL_Event &event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        auto personController = currentScene->cameras[0]->getGameObject()->getComponent<PersonController>();
        if(personController->allowedToBuild)
        {
            auto tower = currentScene->createGameObject(selectedTower->getName());
            tower->getComponent<Transform>()->position = personController->tower->getComponent<Transform>()->position;
            tower->getComponent<Transform>()->rotation = personController->tower->getComponent<Transform>()->rotation;
            tower->getComponent<Transform>()->scale = {0.5f,0.5f,0.5f};

            auto towerMR = tower->addComponent<ModelRenderer>();
            auto path =  ".\\assets\\"+ selectedTower->getMesh();
            std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path).withName(selectedTower->getMesh()).build();
            //towerMR->setMesh(personController->tower->getComponent<ModelRenderer>()->getMesh());
            towerMR->setModel(modelHolder);

            score -= selectedTower->getBuildCost();
        }
    }
}

int GameManager::getScore() const {
    return score;
}

void GameManager::setScore(int score) {
    GameManager::score = score;
}

float GameManager::getPower() const {
    return power;
}

void GameManager::setPower(float power) {
    GameManager::power = power;
}

void GameManager::updateTowerIndicator()
{
    if(currentScene == nullptr)
        return;

    auto towerIndicator = currentScene->cameras[0]->getGameObject()->getComponent<PersonController>()->tower;

    auto path =  ".\\assets\\"+ selectedTower->getMesh();
    std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path).withName(selectedTower->getMesh()).build();

    towerIndicator->getComponent<ModelRenderer>()->setModel(modelHolder);
    towerIndicator->getComponent<ModelRenderer>()->active = true;
}


void GameManager::TogglePause()
{
    this->paused = !this->paused;
    ToggleLockMouse();
}

void GameManager::ToggleLockMouse()
{
    auto r = SDLRenderer::instance;
    SDL_SetWindowGrab(r->getSDLWindow(), paused ? SDL_FALSE : SDL_TRUE);
    SDL_SetRelativeMouseMode(paused ? SDL_FALSE : SDL_TRUE);
}

void GameManager::setPath(std::vector<glm::vec3> pathToBe){
    path = pathToBe;
}

std::vector<glm::vec3> GameManager::getPath() {
    return path;
}

glm::vec3 GameManager::getNextPathPoint(int currentPathIndex){
    //path should count down from end of vector to 0
     if (currentPathIndex == 0)
    {
        return path[0];
    } else if (currentPathIndex <= (path.size()-1))
    {
        return path[currentPathIndex-1];
    } else  
    {
        return path[0];
    }
}


int GameManager::getFirstPathIndex(){
    return path.size()-1;
}

void GameManager::addWave(int waveNumber, std::vector<enemySetsInWave> enemySets, waveScheduleDetails waveDetails){
    waveAndEnemys[waveNumber]=enemySets;
    waveAndTimeBetweens[waveNumber]=waveDetails;
    enemyAmountWave += 1; //adds a wave amount every time add wave is called. 
    totalWavesInLevel += 1;
}

int GameManager::getCurrentEnemy(){
    return currentEnemy;
}

int GameManager::getCurrentEnemySet(){
    return currentEnemySet;
}

int GameManager::getCurrentWave(){
    return currentWave;
}

int GameManager::getEnemyAmountWave(){
    return enemyAmountWave;
}

waveScheduleDetails GameManager::getCurrentTimeBetweenWaves(){

    return waveAndTimeBetweens[currentWave];
}

void GameManager::updateAllWaveStats(){
    int tempCurrentEnemyHolder = currentEnemy;
    int tempCurrentEnemySetHolder = currentEnemySet;
    int tempCurrentEnemyWaveHolder = currentWave;

    //check enemy
    tempCurrentEnemyHolder ++;
    if (tempCurrentEnemyHolder > totalEnemiesInCurrentSet)
    { //temp number is higher than in the set

        //check set
        tempCurrentEnemySetHolder ++;
        if (tempCurrentEnemySetHolder >= totalEnemySetsInCurrentWave)
        { // temp set is higher than the total

            //check wave
            tempCurrentEnemyWaveHolder ++;
            // std::cout << "waves in level" << totalWavesInLevel;

            if (tempCurrentEnemyWaveHolder >= totalWavesInLevel)
            { // temp waves is higher than total waves
                return; //Do nothing, the level is at the last wave anyway
            } else //update wave
            { //temp wave is under total waves
                checkAndUpdateWaveNumber(tempCurrentEnemyWaveHolder);
                return;
            }


        } else // update the set
        {
            currentEnemySet = tempCurrentEnemySetHolder;
            //get total number of enemies in the current set
            setTotalEnemiesInCurrentSet();

            //reset enemy number
            currentEnemy = 0;

            return;
        }

    } else
    { //temp enemy number wasn't too high, means there is another enemy in the set
        currentEnemy = tempCurrentEnemyHolder;
    }
}


void GameManager::checkAndUpdateEnemyNumber(){

}

void GameManager::checkAndUpdateEnemySetNumber(){

}

void GameManager::checkAndUpdateWaveNumber(int tempCurrentEnemyWaveHolder){
    currentWave = tempCurrentEnemyWaveHolder;
    //reset cenemy and set numbers
    currentEnemy = 0;
    currentEnemySet = 0;

    //get new total enemies
    setTotalEnemiesInCurrentSet();

    //get new total sets
    totalEnemySetsInCurrentWave = waveAndEnemys[currentWave].size();
}

void GameManager::setInitialWaveStats(){
    //get wave 0 sets
    totalEnemySetsInCurrentWave = waveAndEnemys[0].size();

    //get wave 0 enemies
    totalEnemiesInCurrentSet = waveAndEnemys[0][0].quantiy - 1; //minus one, because counting starts at 1, not 0
}

void GameManager::setTotalEnemiesInCurrentSet() {
    totalEnemiesInCurrentSet = waveAndEnemys[currentWave][currentEnemySet].quantiy - 1; //minus one, because counting starts at 1, not 0
}

const std::map<int, std::vector<enemySetsInWave>> &GameManager::getWaveAndEnemys() const {
    return waveAndEnemys;
}

int GameManager::getTotalEnemiesInCurrentSet() const {
    return totalEnemiesInCurrentSet;
}
