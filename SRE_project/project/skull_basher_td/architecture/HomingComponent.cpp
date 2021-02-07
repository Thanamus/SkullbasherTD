//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include "HomingComponent.hpp"
#include "physics/RigidBody.hpp"
#include "Transform.hpp"
#include "glm/glm.hpp"

HomingComponent::HomingComponent(GameObject *gameObject_) : Component(gameObject_){}

void HomingComponent::update(float deltaTime) {
    target;
//    if(getTarget()) {
//        auto rigidBody = gameObject->getComponent<RigidBody>();
//        auto transform = gameObject->getComponent<Transform>();
//        auto position = transform->globalPosition();
//        auto targetPosition = target->getComponent<Transform>()->globalPosition();
//        auto direction = targetPosition - position;
//        direction = glm::normalize(direction);
//        auto newPosition = position + direction * speed * deltaTime;
//
//        if(rigidBody && rigidBody->getRigidBody()) {
//            btVector3 nextBtPosition = { newPosition.x, newPosition.y, newPosition.z };
//            btTransform btTransform = rigidBody->getRigidBody()->getWorldTransform();
//            btTransform.setOrigin(nextBtPosition);
//        } else
//            transform->position = newPosition;
//        transform->lookAt(targetPosition, glm::vec3(0,1,0));
//    }
}

float HomingComponent::getSpeed() const {
    return speed;
}

void HomingComponent::setSpeed(float speed) {
    HomingComponent::speed = speed;
}
