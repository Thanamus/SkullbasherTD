//
// Created by Morten Nobel-Jørgensen on 11/6/17.
//

#include <iostream>
#include "BirdMovementComponent.hpp"
#include "GameObject.hpp"

BirdMovementComponent::BirdMovementComponent(GameObject *gameObject) : Component(gameObject) {}

void BirdMovementComponent::update(float deltaTime) {
    time += deltaTime;
    gameObject->setPosition(computePositionAtTime(time));
}

glm::vec2 BirdMovementComponent::computePositionAtTime(float time) {
    int segment = (int)fmod(time, getNumberOfSegments());
    float t = fmod(time,1.0f);
    glm::vec2 p0 = positions[segment*2];
    glm::vec2 p1 = positions[segment*2 + 1];
    glm::vec2 p2 = positions[segment*2 + 2];
    return p0*pow((1-t), 2) + 2.f*p1*(1-t)*t + p2*pow(t, 2);

}

const std::vector<glm::vec2> &BirdMovementComponent::getPositions() {
    return positions;
}

void BirdMovementComponent::setPositions(std::vector<glm::vec2> positions) {
    this->positions = std::move(positions);
}

int BirdMovementComponent::getNumberOfSegments() {
    return positions.size()/2;
}

