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
    void GameManager::onMouse(SDL_Event &event);
    void ToggleLockMouse();
    bool quit = false;
    bool buildModeActive = false;

    int getScore() const;

    void setScore(int score);

    float getPower() const;

    void setPower(float power);

    bool paused = false;
    bool debugBricks = true;
    bool lockRotation = false;

    // Wave stats
    int currentWave = 0;
    int waveAmount = 10;
    int enermyAmountWave = 5;
    void updateTowerIndicator();
    std::shared_ptr<class Scene> currentScene;

private:
    void loadTowers(std::string filename);
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    std::vector<std::shared_ptr<Tower>> towers;

    // Player stats
    int score = 42;
    float power = 0.7; // between 0.0 and 1.0
};
