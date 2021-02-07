//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "GameObject.hpp"
#include <utility>
#include "Transform.hpp"

GameObject::GameObject(std::string name_, Scene* scene_) : name(std::move(name_)), scene(scene_) {
    addComponent<Transform>();
}

GameObject::~GameObject() {
    while(!components.empty())
        removeComponent(*(components.begin()));
    components.clear();

    while(!children.empty())
        scene->deleteGameObject(*children.begin());
    children.clear();

    if (parent) {
        auto parentChildren = GameObject::parent->children;
        if(!parentChildren.empty())
            parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), shared_from_this()), parentChildren.end());
        parent = nullptr;
    }
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
        parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), shared_from_this()), parentChildren.end());
    }
    parent = parent_;
    if(parent)
        parent->children.push_back(shared_from_this());
}

const std::vector<std::shared_ptr<GameObject>> &GameObject::getChildren() const {
    return children;
}

std::shared_ptr<GameObject> GameObject::getChildByName(const std::string& childName) {
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

