//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../mixins/Renderable.hpp"
#include "../mixins/Updatable.hpp"
#include "glm/glm.hpp"
#include "../managers/MainMenuGuiManager.hpp"
#include "../managers/ScheduleManager.hpp"
#include "Scene.hpp"




class CameraComponent;
class GameObject;
class Component;
class LightComponent;
class BulletPhysics;
class RigidBodyComponent;
class ScheduleManager;

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

