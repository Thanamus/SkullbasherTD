//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "Component.hpp"
#include "GameObject.hpp"

#include <utility>

Component::Component(GameObject* gameObject) : gameObject(gameObject) {}

Component::~Component() {
    gameObject = nullptr;
}

GameObject *Component::getGameObject() {
    return gameObject;
}

void Component::debugGUI() {
}

void Component::onStart() {
}

void Component::onDestroy() {
}
