#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "Tower.hpp"

class GameManager
{
public:
    explicit GameManager();
    void init();

    std::vector<std::shared_ptr<Tower>> GetTowers();
    std::shared_ptr<Tower> selectedTower;
    void GameManager::onKey(SDL_Event &event);

private:
    void loadTowers(std::string filename);
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    std::vector<std::shared_ptr<Tower>> towers;


    // Player stats
    int health = 3; // between 0 and 6
    int score = 42;
    float power = 0.7; // between 0.0 and 1.0

};
