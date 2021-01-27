
#include "GuiGameInfo.hpp"
#include "../GuiManager.hpp"
#include <sre/Renderer.hpp>

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

GuiGameInfo::GuiGameInfo(GameObject* gameObject) : Component(gameObject)
{
    heartIcons[heartEmpty] = sre::Texture::create().withFile("assets/hud_heartEmpty.png").withFilterSampling(false).build();
    heartIcons[heartHalf] = sre::Texture::create().withFile("assets/hud_heartHalf.png").withFilterSampling(false).build();
    heartIcons[heartFull] = sre::Texture::create().withFile("assets/hud_heartFull.png").withFilterSampling(false).build();
    heartSize = {heartIcons[heartFull]->getWidth()*0.5f, heartIcons[heartFull]->getHeight()*0.5f};
    powerbar = sre::Texture::create().withFile("powerbar.png").withFilterSampling(false).build();
    powerbarSize = {heartSize.x*3,heartSize.y};
}

GuiGameInfo::~GuiGameInfo()
{
}

void GuiGameInfo::drawGui(sre::RenderPass *renderPass) {
    auto r = sre::Renderer::instance;
    auto winsize = r->getWindowSize();
    ImVec2 size = {180, 107};
    ImVec2 pos = {winsize.x - size.x,0};
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};
    ImGui::SetNextWindowPos(pos, cond, pivot);

    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::Begin("#gameinfo", open, flags);

    // draw health
    float width = heartSize.x*3;
    float windowWidth = ImGui::GetWindowContentRegionWidth();
    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);

    // draw Score
    ImGui::PushID(1);
    auto scoreStr = std::to_string(gameManager->getScore());
    ImGui::Text("Money"); ImGui::SameLine();
    width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    ImGui::SetCursorPosX(guiManager->centerText(ImGui::GetWindowSize(), scoreStr)); // align center
    ImGui::Text(scoreStr.c_str());
    ImGui::PopID();

    // Draw powerbar
    ImGui::Text("Health"); ImGui::SameLine();
    width = powerbarSize.x;
    ImGui::SetCursorPosX(windowWidth - width); // align right

    // Draw background
    ImGui::Image(powerbar->getNativeTexturePtr(),{powerbarSize.x,powerbarSize.y}, uv0, uv1); ImGui::SameLine();
    float border=3;
    auto innerSize = powerbarSize - glm::vec2(border*2,border*2);
    ImGui::SetCursorPosX(windowWidth - width + border); // align right
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + border); // move down

    // scale/clip inner bar
    innerSize.x *= gameManager->getPower();
    uv1.x *= gameManager->getPower();
    ImVec4 tintColor(0,1,0,1);
    ImGui::Image(powerbar->getNativeTexturePtr(),{innerSize.x,innerSize.y}, uv0, uv1, tintColor);

    ImGui::End();

}