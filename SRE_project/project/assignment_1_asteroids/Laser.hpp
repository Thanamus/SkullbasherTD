#pragma once

#include "SpaceShip.hpp"
#include "Collidable.hpp"

class Laser : public GameObject, public Collidable
{
public:
    Laser(const sre::Sprite &sprite, SpaceShip *source);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    glm::vec2 velocity;

private:
    float expTime = 0;
    float speed = 200.f;
    glm::vec2 winSize;
};
