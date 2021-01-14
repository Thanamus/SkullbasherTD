#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
//#include "../WorldMap.hpp"
#include "../GuiManager.hpp"
#include "../GameManager.hpp"
#include "./Scene.hpp"

class Main
{
public:
    Main();
    static std::shared_ptr<Scene> createScene();
    const sre::SDLRenderer &getR() const;
private:

    std::shared_ptr<Scene> currentScene;
    sre::SDLRenderer r;
    std::shared_ptr<GuiManager> guiManager;
    std::shared_ptr<GameManager> gameManager;
};
