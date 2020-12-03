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
    void guiSpeechBubble();
    void guiInventory();

    std::shared_ptr<sre::Texture> powerbar;
    glm::vec2 powerbarSize;
    std::shared_ptr<sre::Texture> heartIcons[3];
    glm::vec2 heartSize;
    std::shared_ptr<sre::SpriteAtlas> heroSpriteAtlas;
    static std::map<std::string, std::shared_ptr<sre::Texture>> inventoryTexture;
    glm::vec2 velocity;
    ImFont* ProggyTiny;

    float speachBubbleTimeOut = 10;
    std::string message = "Hi! AWSD to control.";

    glm::ivec2 heroSize;

    ImVec4 selectedBorderColor;

    // hero stats
    int health = 3; // between 0 and 6
    int score = 42;
    float power = 0.7; // between 0.0 and 1.0
    std::set<std::string> inventorySet;
    std::shared_ptr<GameManager> gameManager;
};
