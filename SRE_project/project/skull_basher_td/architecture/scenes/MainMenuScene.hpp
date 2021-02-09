//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include <vector>
#include <string>

#include "../Renderable.hpp"
#include "../Updatable.hpp"

#include "../../MainMenuGuiManager.hpp"
#include "Scene.hpp"




class Camera;
class GameObject;
class Component;
class Light;


class MainMenuScene: public Scene {
public:
    explicit MainMenuScene(std::string name = "MainMenu");
    ~MainMenuScene();

    void render() override;
    void update(float deltaTime) override;
    void onKey(SDL_Event &event) override;
    void onMouse(SDL_Event &event) override;
private:
    friend class GameObject;
    friend class SceneManager;
};

