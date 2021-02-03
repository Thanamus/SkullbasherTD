//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GuiManager.hpp"
#include "MainMenuGuiManager.hpp"
#include "architecture/scenes/SceneManager.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>

using namespace sre;
using namespace glm;

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

MainMenuGuiManager::MainMenuGuiManager() : GuiManager()
{

}

void MainMenuGuiManager::guiMainMenu() {
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 150, Renderer::instance->getWindowSize().y / 2), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_Always);
    //Title
    ImGui::Begin("#Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Menu";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    //Back To Game
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Start Game", ImVec2(100, 50))){
        pickLevel = true;
    }

    ImGui::Spacing();

    //Quit
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Quit Game", ImVec2(100, 50))){
        //gameManager->quit = true;
    }

    ImGui::End();
}

void MainMenuGuiManager::guiLevelGrid() {
    int count = 0;
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 150);
    for (auto& level : sceneManager->getLevelsData()){
        if(level->sceneType == 1)
            continue;
        ImVec2 uv0(0,1); // flip y axis coordinates
        ImVec2 uv1(1,0);
        ImVec2 s(64,64);
        ImVec4 currentBorder = ImVec4(0,0,0,1);

        if (ImGui::Button(level->levelName.c_str(), ImVec2(100, 50))){
            sceneManager->changeScene(level);
        }

        if (count == 0 || count %2 != 0)
            ImGui::SameLine();
        else
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 150);
        count ++;
    }
}

void MainMenuGuiManager::guiPickLevel() {
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 150, Renderer::instance->getWindowSize().y / 2), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_Always);
    //Title
    ImGui::Begin("#LevelPicker", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Pick a Level";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    ImGui::Spacing();

    guiLevelGrid();

    ImGui::Spacing();

    std::string loading = "Pressing Start, takes time to load the map";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), loading)); // align center
    ImGui::Text(loading.c_str());

    ImGui::Spacing();

    //Back
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Back", ImVec2(100, 50))){
        pickLevel = false;
    }

    ImGui::End();
}

void MainMenuGuiManager::onGui() {
    if(!pickLevel)
        guiMainMenu();
    else
        guiPickLevel();
}

MainMenuGuiManager::~MainMenuGuiManager() = default;
