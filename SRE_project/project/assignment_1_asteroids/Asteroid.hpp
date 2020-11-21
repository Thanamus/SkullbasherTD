#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Asteroid : public GameObject, public Collidable
{
public:
    enum Size
    {
        BIG,
        MEDIUM,
        SMALL
    };

    enum Color
    {
        BROWN,
        GREY
    };

    Asteroid(const sre::Sprite &_sprite, Size _size, glm::vec2 _position);

    void update(float deltaTime) override;

    void onCollision(std::shared_ptr<GameObject> other) override;

    glm::vec2 velocity;

private:
    bool rotateCW = false;
    bool rotateCCW = false;
    float minSpeed = 5.f;
    float maxSpeed = 20.f;
    float rotationSpeed = 20.0f;
    Size size;
    Color color;
    glm::vec2 winSize;

    std::shared_ptr<GameObject> detectCollision();
};
