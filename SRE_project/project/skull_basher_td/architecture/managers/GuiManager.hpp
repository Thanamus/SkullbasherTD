#pragma once

#include "sre/SDLRenderer.hpp"

class GuiManager
{
public:
    explicit GuiManager();
    virtual ~GuiManager();
    virtual void onGui();

    static float centerText(ImVec2 window, std::string text);
};
