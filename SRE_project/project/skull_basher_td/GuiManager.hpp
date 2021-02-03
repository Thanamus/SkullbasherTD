#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "GameManager.hpp"

class  SceneManager;

class GuiManager
{
public:
    explicit GuiManager();
    virtual ~GuiManager();
    virtual void onGui();
    //std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<SceneManager> sceneManager;

    static float centerText(ImVec2 window, std::string text);
private:

};
