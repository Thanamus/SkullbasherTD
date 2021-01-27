#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "GameManager.hpp"

class  SceneManager;

class GuiManager
{
public:
    explicit GuiManager(std::shared_ptr<GameManager> gameManager);
    virtual ~GuiManager();
    virtual void onGui();
    std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<SceneManager> sceneManager;

    static float centerText(ImVec2 window, std::string text);
private:
    /*void guiGameInfo();
    void guiCrosshair();
    void guiTowers();
    void guiBuildPopUp(ImVec2 size);
    void guiQuitScreen();
    void guiWinLooseScreen();
    void guiWaveInfo();*/

   /* std::shared_ptr<sre::Texture> powerbar;
    std::shared_ptr<sre::Texture> crosshair;
    glm::vec2 powerbarSize;
    std::shared_ptr<sre::Texture> heartIcons[3];
    glm::vec2 heartSize;
    glm::vec2 velocity;

    float speachBubbleTimeOut = 10;
    std::string message = "Hi! AWSD to control.";

    glm::ivec2 heroSize;

    ImVec4 selectedBorderColor;

    float totalTime = 0.0f;*/
};
