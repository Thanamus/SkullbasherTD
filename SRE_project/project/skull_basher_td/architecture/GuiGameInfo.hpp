#pragma once

#include "Component.hpp"
#include "../GameManager.hpp"
#include "../GuiManager.hpp"
#include "RenderableGui.hpp"


class GuiGameInfo : public Component, public RenderableGui  {
public:
    explicit GuiGameInfo(GameObject* gameObject);
    ~GuiGameInfo();

    void drawGui(sre::RenderPass* renderPass) override;

    std::shared_ptr<sre::Texture> powerbar;
    glm::vec2 powerbarSize;
    std::shared_ptr<sre::Texture> heartIcons[3];
    glm::vec2 heartSize;

    std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<GuiManager> guiManager;

private:

};

