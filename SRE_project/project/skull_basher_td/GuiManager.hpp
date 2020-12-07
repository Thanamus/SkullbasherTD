#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "GameManager.hpp"

class GuiManager
{
public:
    explicit GuiManager(std::shared_ptr<GameManager> gameManager);
    void onGui();

private:
    void guiGameInfo();
    void guiTowers();
    void guiDebugInfo();
    void guiWaveInfo();

    std::shared_ptr<sre::Texture> powerbar;
    glm::vec2 powerbarSize;
    std::shared_ptr<sre::Texture> heartIcons[3];
    glm::vec2 heartSize;
    glm::vec2 velocity;

    float speachBubbleTimeOut = 10;
    std::string message = "Hi! AWSD to control.";

    glm::ivec2 heroSize;

    ImVec4 selectedBorderColor;
    std::shared_ptr<GameManager> gameManager;

    float centerText(ImVec2 window, std::string text);

    float totalTime = 0.0f;
};
