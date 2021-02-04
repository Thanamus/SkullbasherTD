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
    std::cerr << std::endl << "Here we go go go!" << std::endl;
    for(const auto& co : components)
        removeComponent(co);
    components.clear();
    std::cerr << std::endl << components.size() << " components left." << std::endl;

    if(!children.empty())
        for(const auto& ch : children)
            scene->deleteGameObject(ch);
    children.clear();
    std::cerr << std::endl << children.size()<< " children left." << std::endl;

    if (parent) {
        auto parentChildren = GameObject::parent->children;
        if(!parentChildren.empty())
            parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), shared_from_this()), parentChildren.end());
        parent = nullptr;
    }
    scene = nullptr;
    std::cerr << "Here GameObject " << name << " is destroyed with " << components.size() << std::endl;
}

void GameObject::setName(const std::string &name_) {
    GameObject::name = name_;
}


std::string GameObject::getName() {
    return name;
}

const std::vector<CollisionHandler*>& GameObject::getCollisionHandlers(){
    return collisionHandlers;
}

bool GameObject::removeComponent(const std::shared_ptr<Component>& ptr) {
    for (const auto& c : components) {
        if (c.get() == ptr.get()) {
            auto ch = dynamic_cast<CollisionHandler*>(ptr.get());
            if (ch && !collisionHandlers.empty())
                collisionHandlers.erase(std::remove(collisionHandlers.begin(), collisionHandlers.end(), ch), collisionHandlers.end());
            // remove from scene
            scene->removeComponent(c);
            // remove from array (auto-releases the object!)
            components.erase(std::remove(components.begin(),components.end(), c), components.end());
            return true;
        }
    }
    std::cerr << std::endl << ptr.get() << std::endl;
    return false;
}

// bool GameObject::removeComponent(std::shared_ptr<Component> ptr) {
//     for (auto iter = components.begin();iter != components.end(); iter++){
//         if (*iter == ptr){
//             auto ch = dynamic_cast<CollisionHandler*>(ptr.get());
//             if (ch) {
//                 collisionHandlers.erase(std::find(collisionHandlers.begin(), collisionHandlers.end(), ch));
//             }
//             scene->removeComponent((iter).get());
//             components.erase(iter);
//             return true;
//         }
//     }
//     return false;
// }

const std::vector<std::shared_ptr<Component>> & GameObject::getComponents() {
    return components;
}

Scene *GameObject::getScene() {
    return scene;
}

GameObject* GameObject::getParent() const {
    return parent;
}

void GameObject::setParent(GameObject *parent_) {
    if (parent_) {
        auto parentChildren = GameObject::parent->children;
        parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), shared_from_this()), parentChildren.end());
    }
    parent = parent_;
    if(parent)
        parent->children.push_back(shared_from_this());
}

const std::vector<std::shared_ptr<GameObject>> &GameObject::getChildren() {
    return children;
}

std::shared_ptr<GameObject> GameObject::getChildByName(const std::string& childName) {
    for(const auto& c : children)
        if (c->getName() == childName)
            return c;
    return nullptr;
}

