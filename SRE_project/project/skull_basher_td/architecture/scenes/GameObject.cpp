//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "GameObject.hpp"
#include <utility>
#include "../components/TransformComponent.hpp"

GameObject::GameObject(std::string name_, Scene* scene_) : name(std::move(name_)), scene(scene_) {
    addComponent<TransformComponent>();
}

GameObject::~GameObject() {
    while(!components.empty()) {
        removeComponent(*(components.begin()));
    }
    while(!children.empty())
        removeChild(*(children.begin()));
    scene = nullptr;
}

void GameObject::setName(const std::string &name_) {
    GameObject::name = name_;
}


std::string GameObject::getName() {
    return name;
}

const std::vector<CollisionHandler*>& GameObject::getCollisionHandlers() const {
    return collisionHandlers;
}

void GameObject::removeComponent(const std::shared_ptr<Component>& ptr) {
    auto iter = std::find(components.begin(), components.end(), ptr);
    if(iter != components.end()) {
        auto ch = dynamic_cast<CollisionHandler*>(ptr.get());
        if (ch && !collisionHandlers.empty())
            collisionHandlers.erase(std::remove(collisionHandlers.begin(), collisionHandlers.end(), ch), collisionHandlers.end());
        // remove from scene
        scene->removeComponent(ptr);
        // remove from array (auto-releases the object!)
        components.erase(std::remove(components.begin(),components.end(), ptr), components.end());
    }
}


void GameObject::removeChild(GameObject* ptr) {
    auto iter = std::find(children.begin(), children.end(), ptr);
    if(iter != children.end()) {
        ptr->setParent(nullptr);
        children.erase(std::remove(children.begin(), children.end(), ptr), children.end());
    }
}


const std::vector<std::shared_ptr<Component>> & GameObject::getComponents() const{
    return components;
}

Scene *GameObject::getScene() {
    return scene;
}

GameObject* GameObject::getParent() const {
    return parent;
}

void GameObject::setParent(GameObject *parent_) {
    if (parent) {
        auto parentChildren = parent->getChildren();
        parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), this), parentChildren.end());
    }
    parent = parent_;
    if(parent)
        parent->children.push_back(this);
}

const std::vector<GameObject*> &GameObject::getChildren() const {
    return children;
}

GameObject* GameObject::getChildByName(const std::string& childName) {
    for(const auto& c : children)
        if (c->getName() == childName)
            return c;
    return nullptr;
}

bool GameObject::isQueuedForDeletion() const {
    return queuedForDeletion;
}

void GameObject::setQueuedForDeletion(bool queuedForDeletion_) {
    GameObject::queuedForDeletion = queuedForDeletion_;
}

