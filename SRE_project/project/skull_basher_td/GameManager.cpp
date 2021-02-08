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
#include <utility>
#include "architecture/music/MusicBuffer.hpp"
#include "architecture/WorldObject.hpp"
#include "architecture/sound/SourceManager.hpp"
#include "architecture/TowerBehaviourComponent.hpp"
#include "architecture/TowerParser.hpp"


using namespace sre;
using namespace glm;

void GameManager::init() {
    /*if(sceneManager == nullptr)
        sceneManager = std::make_unique<SceneManager>();
    towers.clear();
    loadTowers("data/towers.json");
    selectedTower = towers[0];
    quit = false;
    buildModeActive = false;


    levelRunning = true;
    won = false;
    paused = false;
    score = 40;
    path.clear();
    currentWave = 0;
    totalWavesInLevel = 0;

    enemySetsAmount = 0; //assuming this means how many waves


    currentEnemySet = 0;
    totalEnemySetsInCurrentWave = 0;
    // int currentEnemyInset = 0;

    currentEnemy = 0;
    totalEnemiesInCurrentSet = 0;

    enemyAmountWave = 0;
    waveAndEnemies.clear();
    waveAndTimeBetweens.clear();
    lastEnemy = false;*/

    ////////////////////////
    if(sceneManager == nullptr)
        sceneManager = std::make_unique<SceneManager>();
    towers.clear();
    loadTowers("data/towers.json");
    selectedTower = towers[0];
    quit = false;
    buildModeActive = false;

    bool levelRunning = true;
    bool won = false;
    bool paused = false;

    crystal = nullptr;

    // Player stats
    int score = 40;

    //path
    path.clear();
    waveAndEnemies.clear();
    waveAndTimeBetweens.clear();

    currentWave = 0;
    totalWavesInLevel = 0;

    enemySetsAmount = 0; //assuming this means how many waves

    currentEnemySet = 0;
    totalEnemySetsInCurrentWave = 0;

    currentEnemy = 0;
    totalEnemiesInCurrentSet = 0;

    enemyAmountWave = 0;

    totalEnemiesSpawned = 0;
    totalEnemies = 0;
    lastEnemy = false;
}

void GameManager::loadTowers(std::string filename) {
    towers = std::move(TowerParser::readTowersFromFile(filename));
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
//        case SDLK_3:
//            selectedTowerIndex = 2;
//            pressed = true;
//            buildModeActive = true;
//            break;
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
        auto player = sceneManager->getCurrentScene()->cameras[0]->getGameObject()->getComponent<PersonController>();
        if(buildModeActive)
        {
            updateTowerIndicator();
            player->updateHandModel("hammer");
        }
        else {
            auto towerIndicator = sceneManager->currentScene->cameras[0]->getGameObject()->getComponent<PersonController>()->tower;
            towerIndicator->getComponent<ModelRenderer>()->active = false;

            player->updateHandModel("lowpoly_crossbow_2_5");
        }
    }
}

void GameManager::onMouse(SDL_Event &event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        auto personController = sceneManager->currentScene->cameras[0]->getGameObject()->getComponent<PersonController>();
        if(personController->allowedToBuild)
        {
//            //todo: re-enable
//            auto tower = sceneManager->currentScene->createGameObject(selectedTower->getName());
//            auto towerTR = tower->getComponent<Transform>();
//            towerTR->position = personController->tower->getComponent<Transform>()->position;
//            towerTR->rotation = personController->tower->getComponent<Transform>()->rotation;
//            towerTR->scale = {0.5f,0.5f,0.5f};
//
//            auto towerMR = tower->addComponent<ModelRenderer>();
//            auto towerAN = tower->addComponent<Animator>();
//
//            towerTR->setModelRenderer(towerMR);
//            towerTR->setAnimator(towerAN);
//
//            auto towerTB = tower->addComponent<TowerBehaviourComponent>();
//            towerTB->setEnabled(true);
//            auto path_ = ".\\assets\\" + selectedTower->getMesh();
//            std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path_).withName(selectedTower->getMesh()).build();
//            //towerMR->setMesh(personController->tower->getComponent<ModelRenderer>()->getMesh());
//            towerMR->setModel(modelHolder);
            auto model = Model::create().withOBJ(R"(.\assets\Forest Arena\catapult_base.obj)").build();
            auto catapult = sceneManager->currentScene->createGameObject(selectedTower->getName());
            auto arm = sceneManager->currentScene->createGameObject("Arm");
            arm->setParent(catapult.get());
            auto ball = sceneManager->currentScene->createGameObject("Ball");
            ball->setParent(arm.get());

            auto towerTR = catapult->getComponent<Transform>();
            towerTR->position = personController->tower->getComponent<Transform>()->position;
            towerTR->position.y += 0.5;
            towerTR->rotation = personController->tower->getComponent<Transform>()->rotation;
            towerTR->scale = {0.5f,0.5f,0.5f};

            auto catMR = catapult->addComponent<ModelRenderer>();
            catMR->setModel(model);
            catapult->getComponent<Transform>()->setModelRenderer(catMR);
            model = Model::create().withOBJ(R"(.\assets\Forest Arena\catapult_arm.obj)").build();

            auto armMR = arm->addComponent<ModelRenderer>();
            armMR->setModel(model);
            auto armAN = arm->addComponent<Animator>();
            arm->getComponent<Transform>()->setModelRenderer(armMR);
            arm->getComponent<Transform>()->setAnimator(armAN);
            std::shared_ptr<Animation> launch = std::make_shared<Animation>(false);
            launch->addFrame(glm::vec3(0), glm::vec3(1), glm::vec3(-90, 0, 0), 1.f);
            armAN->addAnimation("launch", launch);
            std::shared_ptr<Animation> reload = std::make_shared<Animation>(true);
            reload->addFrame(glm::vec3(0), glm::vec3(1), glm::vec3(90, 0, 0), 2.f);
            armAN->addAnimation("reload", reload);
            auto catTR = catapult->getComponent<Transform>();
            auto armTR = arm->getComponent<Transform>();
            auto ballTR = ball->getComponent<Transform>();
            model = Model::create().withOBJ(R"(.\assets\Forest Arena\ball.obj)").build();
            auto ballMR = ball->addComponent<ModelRenderer>();
            ballMR->setModel(model);
            ball->getComponent<Transform>()->setModelRenderer(ballMR);

            armMR->getModel()->setTransform(glm::translate(glm::vec3(0, 1, -0.5)));
            ballMR->getModel()->setTransform(glm::translate(glm::vec3(0, 0.4, 1.6)));

            auto towerTB = catapult->addComponent<TowerBehaviourComponent>();
            towerTB->setEnabled(true);
            score -= selectedTower->getBuildCost();

            personController->targetBlock->getComponent<WorldObject>()->setBuildable(false);
            if(personController->hand->getComponent<Animator>()->getAnimationState() != "build")
                personController->hand->getComponent<Animator>()->setAnimationState("build");

            SourceManager::Get()->playMyJam_global("wood-hammering.wav");
        }
    }
}

int GameManager::getScore() const {
    return score;
}

void GameManager::setScore(int score) {
    GameManager::score = score;
}

void GameManager::updateTowerIndicator()
{
    if(sceneManager->currentScene == nullptr)
        return;

    // todo: fix to use actual composite model!
    auto towerIndicator = sceneManager->currentScene->cameras[0]->getGameObject()->getComponent<PersonController>()->tower;

    auto path_ = ".\\assets\\" + selectedTower->getIndicator();
    std::shared_ptr<Model> modelHolder = Model::create().withOBJ(path_).withName(selectedTower->getIndicator()).build();
    std::cout << "mesh name: " << selectedTower->getIndicator() << std::endl;
    std::cout << "mesh indices size: " << modelHolder->getMesh()->getIndices().size() << std::endl;
    std::cout << "materials size: " << modelHolder->getMaterials().size() << std::endl;
    std::cout << modelHolder->getMesh()->getIndices().size() << std::endl;
    towerIndicator->getComponent<ModelRenderer>()->setModel(modelHolder);
    towerIndicator->getComponent<ModelRenderer>()->active = true;
}


void GameManager::TogglePause()
{
    this->paused = !this->paused;
    ToggleLockMouse();
}

void GameManager::ToggleLockMouse() const
{
    auto r = SDLRenderer::instance;
    SDL_SetWindowGrab(r->getSDLWindow(), paused ? SDL_FALSE : SDL_TRUE);
    SDL_SetRelativeMouseMode(paused ? SDL_FALSE : SDL_TRUE);
}

void GameManager::setPath(std::vector<glm::vec3> pathToBe){
    path = std::move(pathToBe);
}

std::vector<glm::vec3> GameManager::getPath() {
    return path;
}

glm::vec3 GameManager::getPathPoint(int index){
    //path should count down from end of vector to 0
    return (index > 0 && index <= (path.size()-1)) ?
    path[index] :
    path[0];
}


int GameManager::getFirstPathIndex(){
    return path.size()-1;
}

void GameManager::addWave(int waveNumber, std::vector<enemySetsInWave> enemySets, waveScheduleDetails waveDetails){
    waveAndEnemies[waveNumber]=enemySets;
    waveAndTimeBetweens[waveNumber]=waveDetails;
    enemyAmountWave += 1; //adds a wave amount every time add wave is called. 
    totalWavesInLevel += 1;
}

int GameManager::getCurrentEnemy(){
    return currentEnemy;
}

int GameManager::getTotalWavesInLevel(){
    return totalWavesInLevel;
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

bool GameManager::updateAllWaveStats(){
    int tempCurrentEnemyHolder = currentEnemy;
    int tempCurrentEnemySetHolder = currentEnemySet;
    int tempCurrentEnemyWaveHolder = currentWave;

    if (!lastEnemy)
    {
        /* code */
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
                    // getting here should mean that the last enemy was triggered
                    lastEnemy = true;
                    return false; //Do nothing, the level is at the last wave anyway
                } else //update wave
                { //temp wave is under total waves
                    checkAndUpdateWaveNumber(tempCurrentEnemyWaveHolder);
                    return true;
                }


            } else // update the set
            {
                currentEnemySet = tempCurrentEnemySetHolder;
                //get total number of enemies in the current set
                setTotalEnemiesInCurrentSet();

                //reset enemy number
                currentEnemy = 0;

                return true;
            }

        } else
        { //temp enemy number wasn't too high, means there is another enemy in the set
            currentEnemy = tempCurrentEnemyHolder;
            return true;
        }
    }

    return false; // Game is on the last enemy, so no update happened
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
    totalEnemySetsInCurrentWave = waveAndEnemies[currentWave].size();
}

void GameManager::setInitialWaveStats(){
    //get wave 0 sets
    totalEnemySetsInCurrentWave = waveAndEnemies[0].size();

    //get wave 0 enemies
    totalEnemiesInCurrentSet = waveAndEnemies[0][0].quantity - 1; //minus one, because counting starts at 1, not 0
}

void GameManager::setTotalEnemiesInCurrentSet() {
    totalEnemiesInCurrentSet = waveAndEnemies[currentWave][currentEnemySet].quantity - 1; //minus one, because counting starts at 1, not 0
}

const std::map<int, std::vector<enemySetsInWave>> &GameManager::getWaveAndEnemies() const {
    return waveAndEnemies;
}

int GameManager::getTotalEnemiesInCurrentSet() const {
    return totalEnemiesInCurrentSet;
}

const std::unique_ptr<SceneManager> &GameManager::getSceneManager() const {
    return sceneManager;
}

void GameManager::addScore(int score) {
    this->score += score;
}

int GameManager::getTotalEnemiesSpawned() const {
    return totalEnemiesSpawned;
}

void GameManager::setTotalEnemiesSpawned(int totalEnemiesSpawned) {
    GameManager::totalEnemiesSpawned = totalEnemiesSpawned;
}

int GameManager::getTotalEnemies() const {
    return totalEnemies;
}

void GameManager::setTotalEnemies(int totalEnemies) {
    GameManager::totalEnemies = totalEnemies;
}

void GameManager::toggleWinState(bool winState) {
    GameManager::getInstance().levelRunning = false;
    GameManager::getInstance().won = winState;
    GameManager::getInstance().ToggleLockMouse();
    if(!winState)
        MusicBuffer::Get()->changeTracks(R"(.\assets\music\A-Bitter-Hope-MP3.wav)");
    else
        MusicBuffer::Get()->changeTracks(R"(.\assets\music\The-Precipice-of-Victory-MP3.wav)");
}
