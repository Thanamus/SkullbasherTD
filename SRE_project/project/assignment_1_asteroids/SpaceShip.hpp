#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class SpaceShip : public GameObject, public Collidable
{
public:
    SpaceShip(const sre::Sprite &_sprite, const sre::Sprite &_deadSprite);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    void onKey(SDL_Event &keyEvent) override;

    void shootLaser();

    glm::vec2 velocity;

private:
    bool alive = true;
    sre::Sprite deadSprite;
    bool rotateCW = false;
    bool rotateCCW = false;
    bool thrust = false;
    float drag = 3.20f;
    float maxSpeed = 450.0f;
    float thrustPower = 600.0f;
    float rotationSpeed = 150.0f;
    glm::vec2 winSize;

    std::shared_ptr<GameObject> detectCollision();
};
