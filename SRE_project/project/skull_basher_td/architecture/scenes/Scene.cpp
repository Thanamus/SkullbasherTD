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


Scene::Scene(std::string name) : name(std::move(name)) {
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

std::shared_ptr<GameObject> Scene::createGameObject(std::string name_){
    auto res = std::shared_ptr<GameObject>(new GameObject(name_, this));
    gameObjects.push_back(res);
    return res;
}

void Scene::addComponent(const std::shared_ptr<Component>& component) {
    auto componentRawPtr = component.get();
    auto camera = dynamic_cast<Camera*>(componentRawPtr);
    if (camera){
        cameras.push_back(camera);
    }
    auto renderable = dynamic_cast<Renderable*>(componentRawPtr);
    if (renderable) {
        renderables.push_back(renderable);
    }
    auto renderableGui = dynamic_cast<RenderableGui*>(componentRawPtr);
    if (renderableGui) {
        renderablesGui.push_back(renderableGui);
    }
    auto updatable = dynamic_cast<Updatable*>(componentRawPtr);
    if (updatable) {
        updatables.push_back(updatable);
    }
    auto scriptable = dynamic_cast<Scriptable*>(componentRawPtr);
    if (scriptable) {
        scriptables.push_back(scriptable);
    }
    auto targetable = dynamic_cast<Targetable*>(componentRawPtr);
    if (targetable) {
        targetables.push_back(targetable);
    }
    auto enemy = dynamic_cast<EnemyComponent*>(componentRawPtr);
    if (enemy) {
        enemies.push_back(enemy);
    }
    auto light = dynamic_cast<Light*>(componentRawPtr);
    if (light) {
        lights.push_back(light);
    }
    auto rigidBody = dynamic_cast<RigidBody*>(componentRawPtr);
    if (rigidBody) {
        rigidBodies.push_back(rigidBody);
    }
}

void Scene::removeComponent(const std::shared_ptr<Component>& component) {
    auto componentRawPtr = component.get();
    //NOTE: updated to use the erase-remove idiom (https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Erase-Remove)
    auto camera = dynamic_cast<Camera*>(componentRawPtr);
    if (camera) {
        if (!cameras.empty()) {
            cameras.erase(std::remove(cameras.begin(), cameras.end(), camera), cameras.end());
        }
    }
    auto renderable = dynamic_cast<Renderable*>(componentRawPtr);
    if (renderable) {
        if(!renderables.empty()) {
            renderables.erase(std::remove(renderables.begin(), renderables.end(), renderable), renderables.end());
        }
    }
    auto renderableGui = dynamic_cast<RenderableGui*>(componentRawPtr);
    if (renderableGui) {
        if(!renderablesGui.empty()) {
            renderablesGui.erase(std::remove(renderablesGui.begin(), renderablesGui.end(), renderableGui), renderablesGui.end());
        }
    }
    auto updatable = dynamic_cast<Updatable*>(componentRawPtr);
    if (updatable) {
        if(!updatables.empty()) {
            updatables.erase(std::remove(updatables.begin(), updatables.end(), updatable), updatables.end());
        }
    }
    auto scriptable = dynamic_cast<Scriptable*>(componentRawPtr);
    if (scriptable) {
        if(!scriptables.empty()) {
            scriptables.erase(std::remove(scriptables.begin(), scriptables.end(), scriptable), scriptables.end());
        }
    }
    auto targetable = dynamic_cast<Targetable*>(componentRawPtr);
    if (targetable) {
        if(!targetables.empty()) {
            targetables.erase(std::remove(targetables.begin(), targetables.end(), targetable), targetables.end());
        }
    }
    auto enemy = dynamic_cast<EnemyComponent*>(componentRawPtr);
    if (enemy) {
        if(!enemies.empty()) {
            enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
        }
    }
    auto light = dynamic_cast<Light*>(componentRawPtr);
    if (light) {
        if (!lights.empty()) {
            lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
        }
    }
    auto rigidBody = dynamic_cast<RigidBody*>(componentRawPtr);
    if (rigidBody) {
        if(!rigidBodies.empty()) {
            rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), rigidBody), rigidBodies.end());
        }
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

const std::vector<std::shared_ptr<GameObject>>& Scene::getGameObjects() const {
    return gameObjects;
}

const std::vector<EnemyComponent*>& Scene::getEnemies() {
    return enemies;
}

void Scene::deleteGameObject(const std::shared_ptr<GameObject>& gameObject) {
    // look for gameObject to delete
    for(const auto& t : targetables)
        if(t->getTarget() == gameObject.get())
            t->setTarget(nullptr);
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    // if found, release the managed object and remove it from the array
    if(it != gameObjects.end()) {
        auto parent = gameObject->getParent();
        if(parent)
            parent->removeChild(gameObject.get());
        gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject), gameObjects.end());
    }

}

#pragma clang diagnostic pop