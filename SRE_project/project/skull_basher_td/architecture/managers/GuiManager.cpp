//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "GuiManager.hpp"
#define GLM_ENABLE_EXPERIMENTAL


using namespace sre;
using namespace glm;

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
