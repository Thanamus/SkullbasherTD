#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
//#include "../WorldMap.hpp"
#include "../GuiManager.hpp"
#include "../GameManager.hpp"
#include "scenes/Scene.hpp"
#include "./ScheduleManager.hpp"
#include "scenes/SceneManager.hpp"

class SoundEffectsPlayer;

class Main
{
public:
    Main();
    //static std::shared_ptr<Scene> createScene(std::string levelName);
    //std::shared_ptr<Scene> createMainMenuScene();
    const sre::SDLRenderer &getR() const;
    uint32_t /*ALuint*/ soundA;

    std::shared_ptr<SceneManager> sceneManager;
private:
    //std::shared_ptr<Scene> currentScene;
    sre::SDLRenderer r;
    std::shared_ptr<GuiManager> guiManager;
    std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<ScheduleManager> scheduleManager;
};
