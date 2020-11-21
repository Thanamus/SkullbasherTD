//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Laser.hpp"
#include "sre/Renderer.hpp"
#include "ctime"
#include <vector>
#include <algorithm>

Laser::Laser(const sre::Sprite &sprite, SpaceShip *source) : GameObject(sprite)
{
    scale = glm::vec2(0.5f, 0.5f);
    radius = 1.5;
    winSize = sre::Renderer::instance->getDrawableSize();
    glm::vec2 direction = glm::rotateZ(glm::vec3(0, 100, 0), glm::radians(source->getRotation()));
    rotation = source->getRotation();
    position = source->getPosition();
    velocity = source->velocity + direction;
    float currSpeed = glm::length(velocity);
    if (currSpeed < speed)
        velocity *= (speed / currSpeed);
}

void Laser::update(float deltaTime)
{
    expTime += deltaTime;
    if (expTime > 1)
        AsteroidsGame::getInstance()->queueToDel(shared_from_this());
    position += velocity * deltaTime;
    if (position.x < 0)
    {
        position.x += winSize.x;
    }
    else if (position.x > winSize.x)
    {
        position.x -= winSize.x;
    }
    if (position.y < 0)
    {
        position.y += winSize.y;
    }
    else if (position.y > winSize.y)
    {
        position.y -= winSize.y;
    }
}

void Laser::onCollision(std::shared_ptr<GameObject> other)
{
}
