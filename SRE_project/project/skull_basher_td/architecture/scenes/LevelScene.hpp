//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../mixins/Renderable.hpp"
#include "../mixins/Updatable.hpp"
#include "glm/glm.hpp"
#include "../managers/GuiManager.hpp"
#include "../managers/ScheduleManager.hpp"
#include "Scene.hpp"

#include <vector>
#include <string>


class CameraComponent;
class GameObject;
class Component;
class LightComponent;
class BulletPhysics;
class RigidBodyComponent;
class ScheduleManager;

class LevelScene: public Scene {
public:
    explicit LevelScene(std::string name = "Scene");
    ~LevelScene();

    void render() override;
    void update(float deltaTime) override;
    void onKey(SDL_Event &event) override;
    void onMouse(SDL_Event &event) override;
private:
    friend class GameObject;
};

