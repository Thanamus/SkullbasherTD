//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GuiManager.hpp"
#include "GameManager.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace sre;
using namespace glm;

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

GuiManager::GuiManager(std::shared_ptr<GameManager> gameManager)
{
    this->gameManager = gameManager;

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
}

void GuiManager::guiGameInfo() {
    auto r = Renderer::instance;
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
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), scoreStr)); // align center
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
void GuiManager::guiTowers() {
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
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text("Towers");

    int count = 0;
    int selectedTowerID = gameManager.get()->selectedTower.get()->getId();
    for (auto& tower : gameManager.get()->GetTowers()){
        ImVec2 uv0(0,1); // flip y axis coordinates
        ImVec2 uv1(1,0);
        ImVec2 s(64,64);
        ImVec4 currentBorder = ImVec4(0,0,0,1);;
        if(gameManager->buildModeActive)
            currentBorder = tower.get()->getId() == selectedTowerID ? selectedBorderColor : ImVec4(0,0,0,1);

        ImGui::Image(tower->getIcon()->getNativeTexturePtr(), s, uv0, uv1 , ImVec4(1,1,1,1),currentBorder);
        ImGui::SameLine();
    }

    ImGui::End();
    if(gameManager->buildModeActive)
        guiBuildPopUp(size);
}

void GuiManager::guiBuildPopUp(ImVec2 towerWindowSize) {
    auto r = Renderer::instance;
    auto winsize = r->getWindowSize();
    auto cond = ImGuiCond_Always;
    ImVec2 pivot = {0,0};

    ImVec2 size = {250, 75};
    ImVec2 pos = {(winsize.x  / 2.0f) - (size.x / 2),winsize.y - towerWindowSize.y - size.y};
    ImGui::SetNextWindowPos(pos, cond, pivot);
    ImGui::SetNextWindowSize(size, cond);
    auto flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar;
    bool* open = nullptr;
    ImGui::Begin("#BuildPopUp", open, flags);
    //Title
    std::string title = "Build: " + gameManager->selectedTower->getName();
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << gameManager->selectedTower->getBuildCost();
    std::string priceString = stream.str();

    std::string price = "cost: $" + priceString;
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), price)); // align center
    ImGui::Text(price.c_str());

    std::string exitText = "Press escape to exit build mode";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), exitText)); // align center
    ImGui::Text(exitText.c_str());

    ImGui::End();
}

void GuiManager::guiDebugInfo()
{
    ImGui::SetNextWindowPos(ImVec2(0, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
    ImGui::Begin("#Debug", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Debug";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text("Debug");
    ImGui::End();
}

void GuiManager::guiWaveInfo()
{
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
    //Title
    ImGui::Begin("#Wave", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Wave";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    //Wave
    std::string waveText = std::to_string(gameManager->currentWave) + "/" + std::to_string(gameManager->waveAmount);
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), waveText)); // align center
    ImGui::Text(waveText.c_str());

    ImGui::Spacing();

    //Enermy
    std::string enemies = "Enemies";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), enemies)); // align center
    ImGui::Text(enemies.c_str());
    std::string enermyText = std::to_string(0) + "/" + std::to_string(gameManager->enermyAmountWave);
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), enermyText)); // align center
    ImGui::Text(enermyText.c_str());

    ImGui::End();
}

void GuiManager::guiQuitScreen()
{
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 150, Renderer::instance->getWindowSize().y / 2), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Always);
    //Title
    ImGui::Begin("#Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Menu";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    //Back To Game
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Back to Menu", ImVec2(100, 50))){
        gameManager->paused = false;
    }

    ImGui::Spacing();

    //Quit
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Quit Game", ImVec2(100, 50))){
        gameManager->quit = true;
    }

    ImGui::End();
}

float GuiManager::centerText(ImVec2 window, std::string text)
{
    float font_size = ImGui::GetFontSize() * text.size() / 2;
    return window.x / 2 - font_size + (font_size / 2);
}

void GuiManager::onGui() {
    if(gameManager->paused)
    {
        guiQuitScreen();
        return;
    }

    guiGameInfo();
    guiTowers();
    //guiDebugInfo();
    guiWaveInfo();

}