//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GuiManager.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

using namespace sre;
using namespace glm;

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

GuiManager::GuiManager()
{
}

float GuiManager::centerText(ImVec2 window, std::string text)
{
    float font_size = ImGui::GetFontSize() * text.size() / 2;
    return window.x / 2 - font_size + (font_size / 2);
}

void GuiManager::onGui() {

}

GuiManager::~GuiManager() {

}
