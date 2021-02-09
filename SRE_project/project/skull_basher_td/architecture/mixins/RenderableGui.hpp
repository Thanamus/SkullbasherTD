#pragma once

#include <sre/RenderPass.hpp>

class RenderableGui {
public:
    virtual void drawGui(sre::RenderPass* renderPass) = 0;
};