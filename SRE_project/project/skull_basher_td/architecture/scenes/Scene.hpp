//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "../Renderable.hpp"
#include "../Updatable.hpp"
#include "glm/glm.hpp"
#include "../../GuiManager.hpp"
#include "../ScheduleManager.hpp"
#include "../RenderableGui.hpp"
#include "../Scriptable.hpp"
#include "../EnemyComponent.hpp"

#include <vector>
#include <string>


class Camera;
class GameObject;
class Component;
class Light;
class BulletPhysics;
class RigidBody;
class ScheduleManager;

class Scene {
public:
    explicit Scene(std::string name = "MainScene");
    ~Scene();
    virtual void render();
    virtual void update(float deltaTime);
    virtual void onKey(SDL_Event &event);
    virtual void onMouse(SDL_Event &event);

    std::shared_ptr<GameObject> createGameObject(std::string name = "Scene");

    bool isDebugPhysics() const;

    void setDebugPhysics(bool debugPhysics);

    const glm::vec3 &getAmbientColor() const;

    void setAmbientColor(const glm::vec3 &ambientColor);
    std::vector<std::shared_ptr<GameObject>> getGameObjects();
    std::vector<EnemyComponent*> getEnemies();

    //World Map stuff
    //void loadMap(std::string filename, std::shared_ptr<Scene> res);
    // load map()
    // add stuff we need for load map

    std::vector<Camera*> cameras;
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
    std::vector<RigidBody*> rigidBodies;
    std::vector<EnemyComponent*> enemies;
    std::vector<Light*> lights;
    sre::WorldLights worldLights;

    void addComponent(Component* component);
    void removeComponent(Component* component);

    BulletPhysics* bulletPhysics;

    std::string mapAssetFolderLoc = ".\\assets\\worldMapAssets"; // apparently does work
    std::string enemiesAssetFolderLoc = ".\\assets\\enemies";
    double tileHeightOffset = -1;
    double tilePosOffset = 1;
private:
    friend class GameObject;
    friend class RigidBody;
    friend class GhostObject;
    friend class MainMenuGuiManager;
};

