//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Scene.hpp"
#include "../mixins/CollisionHandler.hpp"

class Component;

class GameObject {
public:
    ~GameObject();
    template<typename C>
    std::shared_ptr<C> addComponent();
    void removeComponent(const std::shared_ptr<Component>& ptr);

    template<typename C>
    std::shared_ptr<C> getComponent();

    std::string getName();
    void setName(const std::string &name_);

    const std::vector<std::shared_ptr<Component>> & getComponents() const;
    Scene* getScene();

    const std::vector<CollisionHandler*>& getCollisionHandlers() const;

    GameObject* getParent() const;
    void setParent(GameObject *parent_);

    const std::vector<GameObject*> & getChildren() const;
    GameObject* getChildByName(const std::string& childName);
    void removeChild(GameObject* ptr);

    // TODO:  getter/setter
    bool isQueuedForDeletion() const;

    void setQueuedForDeletion(bool queuedForDeletion_);

private:
    std::string name;
    Scene* scene = nullptr;
    GameObject* parent = nullptr;
    std::vector<GameObject*> children = {};
    std::vector<std::shared_ptr<Component>> components = {};
    std::vector<CollisionHandler*> collisionHandlers = {};
    bool queuedForDeletion = false;

    GameObject(std::string name, Scene* scene);

    friend class Scene;
};

// function templates has to defined in header files
// TODO: take a look at this, make everything shared_ptr rather than random raw ptrs? depends whether we have memory leaks
template<typename T>
std::shared_ptr<T> GameObject::addComponent() {
    auto obj = new T(this);
    auto ch = dynamic_cast<CollisionHandler*>(obj);
    if (ch){
        collisionHandlers.push_back(ch);
    }
    std::shared_ptr<T> ptr(obj);
    scene->addComponent(ptr);
    components.push_back(ptr);
    return ptr;
}

template<typename C>
std::shared_ptr<C> GameObject::getComponent() {
    for (const auto& c : components){
        auto castPtr = std::dynamic_pointer_cast<C>(c);
        if (castPtr)
            return castPtr;
    }
    // not found
    return nullptr;
}