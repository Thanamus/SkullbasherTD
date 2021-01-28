//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../Renderable.hpp"
#include "../Updatable.hpp"
#include "glm/glm.hpp"
#include "../../GuiManager.hpp"
#include "../ScheduleManager.hpp"
#include "Scene.hpp"

#include <vector>
#include <string>


class Camera;
class GameObject;
class Component;
class Light;
class BulletPhysics;
class RigidBody;
class ScheduleManager;
class GameManager;

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

