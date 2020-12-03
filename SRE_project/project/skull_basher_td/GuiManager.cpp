//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <SDL_events.h>
#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "sre/Texture.hpp"
#include "GuiManager.hpp"
#include "GameManager.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>

using namespace sre;
using namespace glm;

std::map<std::string, std::shared_ptr<sre::Texture>> GuiManager::inventoryTexture;

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;


GuiManager::GuiManager(std::shared_ptr<GameManager> gameManager)
{
    this->gameManager = gameManager;
    inventorySet = {"Antidote",
                    "Axe",
                    "BlueMagic",
                    "Carrot",
                    "Hat",
                    "Lemon",
                    "Necklace",
                    "Shield"};

    heartIcons[heartEmpty] = Texture::create().withFile("assets/hud_heartEmpty.png").withFilterSampling(false).build();
    heartIcons[heartHalf] = Texture::create().withFile("assets/hud_heartHalf.png").withFilterSampling(false).build();
    heartIcons[heartFull] = Texture::create().withFile("assets/hud_heartFull.png").withFilterSampling(false).build();
    heartSize = {heartIcons[heartFull]->getWidth()*0.5f, heartIcons[heartFull]->getHeight()*0.5f};
    powerbar = Texture::create().withFile("powerbar.png").withFilterSampling(false).build();
    powerbarSize = {heartSize.x*3,heartSize.y};

    selectedBorderColor = ImVec4(36.0f/255.0f,250.0f/255.0f,0,1);

    // setup font
    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
    auto fontName = "assets/ProggyTiny.ttf";
    int fontSize = 20;
    //ProggyTiny =
    //        fonts->AddFontFromFileTTF(fontName, fontSize);
}

void GuiManager::guiGameInfo() {
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();
    ImVec2 size = {180, 107};
    ImVec2 pos = {winsize.x - size.x,0};
    //ImVec2 pos = {(winsize.x  / 2.0f) - (size.x / 2),winsize.y - size.y};
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
    //ImGui::PushFont(ProggyTiny);

    // draw health
    ImGui::Text("Health");
    float width = heartSize.x*3;
    float windowWidth = ImGui::GetWindowContentRegionWidth();
    ImVec2 uv0(0,1); // flip y axis coordinates
    ImVec2 uv1(1,0);
    for (int i=0;i<3;i++){

        ImGui::SameLine(windowWidth - width + heartSize.x * i);
        int texIndex = heartFull;
        if (i*2+1 == health){
            texIndex = heartHalf;
        } else if (i*2 > health){
            texIndex = heartEmpty;
        }
        Texture* tex = heartIcons[texIndex].get();
        ImGui::Image(tex->getNativeTexturePtr(),{heartSize.x,heartSize.y}, uv0, uv1);
    }

    // draw Score
    ImGui::PushID(1);
    auto scoreStr = std::to_string(score);
    ImGui::Text("Score"); ImGui::SameLine();
    width = ImGui::CalcTextSize(scoreStr.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    ImGui::Text(scoreStr.c_str());
    ImGui::PopID();

    // Draw powerbar
    ImGui::Text("Power"); ImGui::SameLine();
    width = powerbarSize.x;
    ImGui::SetCursorPosX(windowWidth - width); // align right
    // Draw background
    ImGui::Image(powerbar->getNativeTexturePtr(),{powerbarSize.x,powerbarSize.y}, uv0, uv1); ImGui::SameLine();
    float border=3;
    auto innerSize = powerbarSize - glm::vec2(border*2,border*2);
    ImGui::SetCursorPosX(windowWidth - width + border); // align right
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + border); // move down
    // scale/clip inner bar
    innerSize.x *= power;
    uv1.x *= power;
    ImVec4 tintColor(0,1,0,1);
    ImGui::Image(powerbar->getNativeTexturePtr(),{innerSize.x,innerSize.y}, uv0, uv1, tintColor);
    //ImGui::PopFont();
    ImGui::End();

}

void GuiManager::guiInventory() {
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};

    ImVec2 size = {(gameManager.get()->GetTowers().size() * 64.0f) + 30.0f, 107};
    ImVec2 pos = {(winsize.x  / 2.0f) - (size.x / 2),winsize.y - size.y};
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    std::string title = "Towers";
    ImGui::Begin("#Towers", open, flags);
    float font_size = ImGui::GetFontSize() * title.size() / 2;
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - font_size + (font_size / 2)); // align center
    ImGui::Text("Towers");

    int count = 0;
    int selectedTowerID = gameManager.get()->selectedTower.get()->getId();
    for (auto& tower : gameManager.get()->GetTowers()){
        ImVec2 uv0(0,1); // flip y axis coordinates
        ImVec2 uv1(1,0);
        ImVec2 s(64,64);

        auto currentBorder = tower.get()->getId() == selectedTowerID ? selectedBorderColor : ImVec4(0,0,0,1);

        ImGui::Image(tower->getIcon()->getNativeTexturePtr(), s, uv0, uv1 , ImVec4(1,1,1,1),currentBorder);
        ImGui::SameLine();
    }

    //ImGui::PopFont();
    ImGui::End();
}

void GuiManager::onGui() {
    guiGameInfo();
    guiInventory();
}