#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "managers/GuiManager.hpp"
#include "managers/GameManager.hpp"
#include "scenes/Scene.hpp"
#include "managers/ScheduleManager.hpp"
#include "managers/SceneManager.hpp"
class Main
{
public:
    Main();
    //static std::shared_ptr<Scene> createScene(std::string levelName);
    //std::shared_ptr<Scene> createMainMenuScene();
    const sre::SDLRenderer &getR() const;
    uint32_t /*ALuint*/ soundA{};

private:
    sre::SDLRenderer r;
};
