//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <SDL_events.h>
#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GameManager.hpp"


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
                buildModeActive = false;
            else if (!buildModeActive && !paused)
            {
                paused = true;
            }
            else if (paused)
            {
                paused = false;
            }
            break;
        default:
            break;
    }

    if(pressed && selectedTowerIndex <= towers.size())
        selectedTower = towers[selectedTowerIndex];
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
