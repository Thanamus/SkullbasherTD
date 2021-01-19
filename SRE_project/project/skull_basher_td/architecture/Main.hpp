#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
//#include "../WorldMap.hpp"
#include "../GuiManager.hpp"
#include "../GameManager.hpp"
#include "./Scene.hpp"
#include "./ScheduleManager.hpp"

class SoundEffectsPlayer;

class Main
{
public:
    Main();
    static std::shared_ptr<Scene> createScene();
    const sre::SDLRenderer &getR() const;
    uint32_t /*ALuint*/ soundA;
private:

    // SoundEffectsPlayer myNewSpeaker;

    std::shared_ptr<Scene> currentScene;
    sre::SDLRenderer r;
    std::shared_ptr<GuiManager> guiManager;
    std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<ScheduleManager> scheduleManager;
};
