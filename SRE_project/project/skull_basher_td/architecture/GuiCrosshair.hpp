#pragma once

#include "Component.hpp"
#include "RenderableGui.hpp"


class GuiCrosshair : public Component, public RenderableGui  {
public:
    explicit GuiCrosshair(GameObject* gameObject);
    ~GuiCrosshair();

    void drawGui(sre::RenderPass* renderPass) override;

    std::shared_ptr<sre::Texture> crosshair;

private:

};

