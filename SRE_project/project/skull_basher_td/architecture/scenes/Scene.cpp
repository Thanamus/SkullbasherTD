//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Scene.hpp"
#include <algorithm>
#include "../Camera.hpp"
#include "../physics/RigidBody.hpp"
#include "../Light.hpp"
#include "../physics/BulletPhysics.hpp"

//fps camera stuff
#include "../PersonController.hpp"

//WorldMap Imports
    //WorldObject
#include "../WorldObject.hpp"

#include "../sound/SoundEffectsLibrary.hpp"
#include "../sound/SourceManager.hpp"

//rapidjson imports
#include "../rapidjson/rapidjson.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


Scene::Scene(std::string name)
:name(std::move(name))
{
    bulletPhysics = new BulletPhysics();
}

Scene::~Scene(){
    delete bulletPhysics;
	bulletPhysics = nullptr;
}

void Scene::update(float deltaTime){

}

void Scene::onKey(SDL_Event &event){

}

void Scene::onMouse(SDL_Event &event){

}


void Scene::render(){

}

std::shared_ptr<GameObject> Scene::createGameObject(std::string name){
    auto res = std::shared_ptr<GameObject>(new GameObject(name, this));
    gameObjects.push_back(res);
    return res;
}

void Scene::addComponent(Component *component) {
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.push_back(camera);
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.push_back(renderable);
    }
    auto renderableGui = dynamic_cast<RenderableGui*>(component);
    if (renderableGui) {
        renderablesGui.push_back(renderableGui);
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.push_back(updatable);
    }
    auto scriptable = dynamic_cast<Scriptable*>(component);
    if (scriptable) {
        scriptables.push_back(scriptable);
    }
    auto enemy = dynamic_cast<EnemyComponent*>(component);
    if (enemy) {
        enemies.push_back(enemy);
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.push_back(light);
    }
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.push_back(rigidBody);
    }
}

void Scene::removeComponent(Component *component) {
    //NOTE: updated to use the erase-remove idiom (https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Erase-Remove)
    auto camera = dynamic_cast<Camera*>(component);
    if (camera){
        cameras.erase(std::remove(cameras.begin(),cameras.end(), camera), cameras.end());
    }
    auto renderable = dynamic_cast<Renderable*>(component);
    if (renderable) {
        renderables.erase(std::remove(renderables.begin(), renderables.end(), renderable), renderables.end());
    }
    auto renderableGui = dynamic_cast<RenderableGui*>(component);
    if (renderableGui) {
        renderablesGui.erase(std::remove(renderablesGui.begin(), renderablesGui.end(), renderableGui), renderablesGui.end());
    }
    auto updatable = dynamic_cast<Updatable*>(component);
    if (updatable) {
        updatables.erase(std::find(updatables.begin(), updatables.end(), updatable), updatables.end());
    }
    auto scriptable = dynamic_cast<Scriptable*>(component);
    if (scriptable) {
        scriptables.erase(std::find(scriptables.begin(), scriptables.end(), scriptable), scriptables.end());
    }
    auto enemy = dynamic_cast<EnemyComponent*>(component);
    if (enemy) {
        enemies.erase(std::find(enemies.begin(), enemies.end(), enemy), enemies.end());
    }
    auto light = dynamic_cast<Light*>(component);
    if (light) {
        lights.erase(std::find(lights.begin(), lights.end(), light), lights.end());
    }
    auto rigidBody = dynamic_cast<RigidBody*>(component);
    if (rigidBody) {
        rigidBodies.erase(std::find(rigidBodies.begin(), rigidBodies.end(), rigidBody), rigidBodies.end());
    }
}

bool Scene::isDebugPhysics() const {
    return debugPhysics;
}

void Scene::setDebugPhysics(bool debugPhysics) {
    Scene::debugPhysics = debugPhysics;
}

const glm::vec3 &Scene::getAmbientColor() const {
    return ambientColor;
}

void Scene::setAmbientColor(const glm::vec3 &ambientColor) {
    Scene::ambientColor = ambientColor;
}

std::vector<std::shared_ptr<GameObject>> Scene::getGameObjects() {
    return gameObjects;
}

std::vector<EnemyComponent *> Scene::getEnemies() {
    return enemies;
}

#pragma clang diagnostic pop