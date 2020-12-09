//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "Renderable.hpp"
#include "Updatable.hpp"
#include "glm/glm.hpp"
#include <vector>
#include <string>



class Camera;
class GameObject;
class Component;
class Light;
class BulletPhysics;
class RigidBody;

class Scene {
public:
    explicit Scene(std::string name = "MainScene");
    ~Scene();
    void render();
    void update(float deltaTime);

    std::shared_ptr<GameObject> createGameObject(std::string name = "Scene");

    bool isDebugPhysics() const;

    void setDebugPhysics(bool debugPhysics);

    const glm::vec3 &getAmbientColor() const;

    void setAmbientColor(const glm::vec3 &ambientColor);

    std::vector<std::shared_ptr<GameObject>> getGameObjects();

    //World Map stuff
    void loadMap(std::string filename, std::shared_ptr<Scene> res);
    // load map()
    // add stuff we need for load map
    std::shared_ptr<GameObject> rayTestedCube;
    std::shared_ptr<GameObject> physicsCube;
    std::vector<Camera*> cameras;
    //
private:
    std::string name;
    bool debugPhysics = true;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    glm::vec3 ambientColor = {0.5f,0.5f,0.5f};
    std::vector<Renderable*> renderables;
    std::vector<Updatable*> updatables;
    std::vector<RigidBody*> rigidBodies;
    std::vector<Light*> lights;
    sre::WorldLights worldLights;

    void addComponent(Component* component);
    void removeComponent(Component* component);

    BulletPhysics* bulletPhysics;

    friend class GameObject;
    friend class RigidBody;

    //World Map stuff
    // std::string mapAssetFolderLoc = "..\\Assets\\WorldMapAssets"; //didn't work
    std::string mapAssetFolderLoc = ".\\assets\\worldMapAssets"; // apparently does work
    double tileHeightOffset = -1;
    double tilePosOffset = 1;

};

