//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//
#define _USE_MATH_DEFINES

#include "MovingPlatformComponent.hpp"
#include "PlatformComponent.hpp"
#include "GameObject.hpp"
#include <iostream>
#include<math.h>

MovingPlatformComponent::MovingPlatformComponent(GameObject *gameObject) : Component(gameObject)
{
    platformComponent = gameObject->getComponent<PlatformComponent>();
}

void MovingPlatformComponent::update(float deltaTime) {
    totalTime += deltaTime;
    auto t = -(cos(M_PI * totalTime) - 1) / 2;
    auto pos = glm::mix(movementStart, movementEnd, t);
    platformComponent->moveTo(pos);
}

void MovingPlatformComponent::setMovementStart(glm::vec2 pos) {
    movementStart = pos;
}

void MovingPlatformComponent::setMovementEnd(glm::vec2 pos) {
    movementEnd = pos;
}
