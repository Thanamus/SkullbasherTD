//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "GuiManager.hpp"
#include "MainMenuGuiManager.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace sre;
using namespace glm;

const int heartEmpty = 0;
const int heartHalf = 1;
const int heartFull = 2;

MainMenuGuiManager::MainMenuGuiManager(std::shared_ptr<GameManager> gameManager) : GuiManager(gameManager)
{
    // setup font
    auto fonts = ImGui::GetIO().Fonts;
    fonts->AddFontDefault();
}

void MainMenuGuiManager::guiMainMenu() {
    ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 150, Renderer::instance->getWindowSize().y / 2), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Always);
    //Title
    ImGui::Begin("#Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    std::string title = "Menu";
    ImGui::SetCursorPosX(centerText(ImGui::GetWindowSize(), title)); // align center
    ImGui::Text(title.c_str());

    //Back To Game
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Start Game", ImVec2(100, 50))){
        //gameManager->TogglePause();
        auto scene = gameManager->sceneManager->;
    }

    ImGui::Spacing();

    //Quit
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 50); // align center
    if (ImGui::Button("Quit Game", ImVec2(100, 50))){
        //gameManager->quit = true;
    }

    ImGui::End();
}

void MainMenuGuiManager::onGui() {
    guiMainMenu();

}