//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../mixins/Renderable.hpp"
#include "../mixins/Updatable.hpp"
#include "glm/glm.hpp"
#include "../managers/GuiManager.hpp"
#include "../managers/ScheduleManager.hpp"
#include "../mixins/RenderableGui.hpp"
#include "../mixins/Scriptable.hpp"
#include "../components/game_entities/enemies/EnemyComponent.hpp"
#include "../mixins/Targetable.hpp"

#include <vector>
#include <string>


class CameraComponent;
class GameObject;
class Component;
class LightComponent;
class BulletPhysics;
class RigidBodyComponent;
class ScheduleManager;
class Scriptable;
class Targetable;
class EnemyComponent;
class RenderableGui;
class GuiManager;
class Updatable;

class Scene {
public:
    explicit Scene(std::string name = "MainScene");
    ~Scene();
    virtual void render();
    virtual void update(float deltaTime);
    virtual void onKey(SDL_Event &event);
    virtual void onMouse(SDL_Event &event);

    std::shared_ptr<GameObject> createGameObject(std::string name_ = "Scene");
    void deleteGameObject(const std::shared_ptr<GameObject>& gameObject);

    bool isDebugPhysics() const;

    void setDebugPhysics(bool debugPhysics);

    const glm::vec3 &getAmbientColor() const;

    void setAmbientColor(const glm::vec3 &ambientColor);
    const std::vector<std::shared_ptr<GameObject>>& getGameObjects() const;
    const std::vector<EnemyComponent*>& getEnemies();

    //World Map stuff
    //void loadMap(std::string filename, std::shared_ptr<Scene> res);
    // load map()
    // add stuff we need for load map

    std::vector<CameraComponent*> cameras;
    std::shared_ptr<GuiManager> guiManager;
    //std::shared_ptr<SceneManager> sceneManager;
    std::shared_ptr<ScheduleManager> scheduleManager;

protected:
    std::string name;
    bool debugPhysics = true;
    bool debugPerformance = false;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    glm::vec3 ambientColor = {0.5f,0.5f,0.5f};
    std::vector<Renderable*> renderables;
    std::vector<RenderableGui*> renderablesGui;
    std::vector<Updatable*> updatables;
    std::vector<Scriptable*> scriptables;
    std::vector<RigidBodyComponent*> rigidBodies;
    std::vector<EnemyComponent*> enemies;
    std::vector<Targetable*> targetables;
    std::vector<LightComponent*> lights;
    sre::WorldLights worldLights;

    void addComponent(const std::shared_ptr<Component>& component);
    void removeComponent(const std::shared_ptr<Component>& component);

    BulletPhysics* bulletPhysics;

    std::string mapAssetFolderLoc = ".\\assets\\worldMapAssets"; // apparently does work
    std::string enemiesAssetFolderLoc = ".\\assets\\enemies";
    double tileHeightOffset = -1;
    double tilePosOffset = 1;

private:
    friend class GameObject;
    friend class RigidBodyComponent;
    friend class GhostObject;
    friend class MainMenuGuiManager;
};

